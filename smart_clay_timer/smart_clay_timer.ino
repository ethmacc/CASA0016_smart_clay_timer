#include <Adafruit_MLX90640.h>
#include <Adafruit_NeoPixel.h>
#include <math.h>
#include <ezTime.h>
#include <map>
#include <DHT.h>
#include <DHT_U.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266WebServer.h>

#define DHTTYPE DHT22

//Define pins
#define LEDPin 14
#define trigPin 15
#define echoPin 13
#define DHTPin 2

// DHT Sensor setup
DHT dht(DHTPin, DHTTYPE); // Initialize DHT sensor
float temp;
float hum;

float perc_dry = 0;
float clay_temp = 0;

//Neopixel setup
#define NUMPIXELS 8
Adafruit_NeoPixel pixels(NUMPIXELS, LEDPin);
uint32_t yellow = pixels.Color(20, 20, 0);
uint32_t green = pixels.Color(0, 40, 0);
uint32_t orange = pixels.Color(30, 10, 0);
uint32_t blue = pixels.Color(0, 0, 40);
uint32_t colour;

//Thermal camera setup
Adafruit_MLX90640 mlx; 
float frame[32*24]; // buffer for full frame of temperatures

//Time delay setup
bool status = false;
long t_now = millis();
long t_last = millis();
long t_now2 = millis();
long t_last2 = millis();

//WiFi
#include "arduino_secrets.h"
const char* ssid = SECRET_SSID;
const char* password = SECRET_PASS;
const char* ssid2 = SECRET_SSID2;
const char* password2 = SECRET_PASS2;

// Construct WiFi client instances
ESP8266WebServer server(80);

ESP8266WiFiMulti wifiMulti;
const uint32_t connectTimeout = 5000;

void setup() {
  Serial.begin(115200);
  delay(5000);

  Serial.println("MLX90640 Smart Clay Timer");
  if (! mlx.begin(MLX90640_I2CADDR_DEFAULT, &Wire)) {
    Serial.println("MLX90640 not found!");
  }
  
  // start DHT sensor
  pinMode(DHTPin, INPUT);
  dht.begin();
  temp = dht.readTemperature();
  hum = dht.readHumidity();

  //Start WiFi and server
  startWifi();
  startWebserver();
  
  //Thermal camera settings
  mlx.setMode(MLX90640_CHESS);
  mlx.setResolution(MLX90640_ADC_18BIT);
  mlx.setRefreshRate(MLX90640_2_HZ);

  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pixels.begin();
}

void loop() {
  server.handleClient();
  delay(500);
  pixels.clear();

  // Clear trigPin
  digitalWrite(trigPin, LOW);
  delayMicroseconds(10); 

  // Set trigPin on HIGH for 10 microsecs
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // Read echoPin
  long duration = pulseIn(echoPin, HIGH);
  float distance = 34400*duration/2000000;

  Serial.print("Distance: ");
  Serial.println(distance);

  // Conditional, only start (and continue) taking readings if an object is in front for longer than 5s
  if (distance <= 15 && !status){
    pixels.fill(green, 0, 8);
    pixels.show();
    delay(250);
    pixels.clear();
    pixels.show();
    t_now = millis();
    if (t_now - t_last > 5000){
      status = true;
    }
  }
  else if (distance <= 15 && status){
    readTemp();
  }

  else{
    pixels.clear();
    pixels.show();
    status = false;
    t_last = millis();
  }

  if (mlx.getFrame(frame) != 0) {
    Serial.println("Failed");
    return;
  }
}

void readTemp() {
  std::map<int, int> histogram; //Create histogram map for counting unique temperature values
  int mode_count = 0;

  for (uint8_t h=0; h<24; h++) {
    for (uint8_t w=0; w<32; w++) {
      float t = frame[h*32 + w];
      int T = round(t*2); // multiply by two to maintain some granularity (to nearest .5) after rounding

      histogram[T]++; //increment element(temperature reading) count
      mode_count = std::max(mode_count, histogram[T]);
    }
  }

  t_now2 = millis();
  if (t_now2 - t_last2 > 10000) {
    temp = dht.readTemperature();
    hum = dht.readHumidity(); 
    Serial.print("Temp: ");
    Serial.println(dht.readTemperature());
    Serial.print("Hum: ");
    Serial.println(dht.readHumidity());
    t_last2 = millis();
  }

  if (hum < 20){ 
    colour = orange;
  }

  else if (hum >= 20 && hum < 40){
    colour = yellow;
  }

  else if (hum >= 40) {
    colour = blue;
  }
  
    // Retrieve the mode(s) 
  for (auto T: histogram){
    if (T.second == mode_count) {
      clay_temp = T.first/2; 

      int T_constrain = constrain(T.first, round((temp-6)*2), round(temp*2) );
      perc_dry = (T_constrain - round(temp - 6) * 2) / 12 * 100;
      int i = map(T_constrain, round((temp-6)*2), round(temp*2), 1, 8);

      pixels.fill(colour, 0, i);
      pixels.show();

      Serial.print("The mode temperature is: ");
      Serial.println(T.first/2);      
    }
  }
}

void startWifi() {

  // Ensure wifi is in station mode not ap mode
  WiFi.mode(WIFI_STA);

  //Register multiple networks
  wifiMulti.addAP(ssid, password);
  wifiMulti.addAP(ssid2, password2);

  // check to see if connected and wait until you are
  while (wifiMulti.run(connectTimeout) != WL_CONNECTED) {
    delay(500);
    Serial.println("WiFi not connected!");
    Serial.println("Attempting connection to any known networks...");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}

void startWebserver() {
  // when connected and IP address obtained start HTTP server
  server.on("/", handle_OnConnect);
  server.onNotFound(handle_NotFound);
  server.begin();
  Serial.println("HTTP server started");
}

// Send HTML to server
void handle_OnConnect() {
  server.send(200, "text/html", SendHTML(temp, clay_temp, hum, perc_dry));
}

// Handle webpage not found error
void handle_NotFound() {
  server.send(404, "text/plain", "Not Found");
}

// Format HTML with plant data as a single long string to be hosted online
String SendHTML(float AmbTemp, float ClayTemp, float Hum, int PercDry) {
  String ptr = "<!DOCTYPE html> <html>\n";
  ptr += "<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0, user-scalable=no\" >\n";
  ptr += "<link href=\"https://fonts.googleapis.com/css?family=Silkscreen:400\" rel=\"stylesheet\">\n";
  ptr += "<meta http-equiv=\"refresh\" content=\"10\" >\n";
  ptr += "<title>Pottery Progress</title>\n";
  ptr += "<style>html { font-family: Silkscreen; display: inline-block; margin: 0px auto; text-align center;}\n";
  ptr += "body{margin-top: 50px;} h1 {color: #444444;margin: 50px auto 30px;}\n";
  ptr += "p {font-size: 24px;color: #444444;margin-bottom: 10px;}\n";
  ptr += "</style>\n";
  ptr += "</head>\n";
  ptr += "<body>\n";
  ptr += "<div id=\"webpage\">\n";
  ptr += "<h1>Pottery Progress</h1>\n";
  ptr += "<img src=\"https://github.com/ethmacc/CASA0016_smart_clay_timer/blob/main/pot_drawing.png?raw=true\" width=\"300\">\n";

  ptr += "<p>Air Temperature: ";
  ptr += (int)AmbTemp;
  ptr += " C</p>";
  ptr += "<p>Humidity: ";
  ptr += (int)Hum;
  ptr += "%</p>";
  ptr += "<p>Clay Temperature: ";
  ptr += (int)ClayTemp;
  ptr += " C</p>";
  ptr += "<p>Your clay is ";
  ptr += PercDry;
  ptr += " % dry</p>";

  ptr += "</div>\n";
  ptr += "</body>\n";
  ptr += "</html>\n";
  return ptr;
}