#include <Adafruit_MLX90640.h>
#include <Adafruit_NeoPixel.h>
#include <math.h>
#include <ezTime.h>
#include <map>
#include <DHT.h>
#include <DHT_U.h>

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

void setup() {
  Serial.begin(115200);
  delay(5000);

  Serial.println("Adafruit MLX90640 Simple Test");
  if (! mlx.begin(MLX90640_I2CADDR_DEFAULT, &Wire)) {
    Serial.println("MLX90640 not found!");
  }
  
  // start DHT sensor
  pinMode(DHTPin, INPUT);
  dht.begin();
  temp = dht.readTemperature();
  hum = dht.readHumidity();
  
  //Thermal camera settings
  mlx.setMode(MLX90640_CHESS);
  mlx.setResolution(MLX90640_ADC_18BIT);
  mlx.setRefreshRate(MLX90640_2_HZ);

  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pixels.begin();
}

void loop() {
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
      int T = round(t);

      histogram[T]++; //increment element(temperature reading) count
      mode_count = std::max(mode_count, histogram[T]);
    }
  }

  t_now2 = millis();
  if (t_now2 - t_last2 > 20000) {
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
      int T_constrain = constrain(T.first, 18, 24);
      int i = map(T_constrain, 18, 24, 1, 8);
      Serial.println(i);
      Serial.println(T_constrain);

      pixels.fill(colour, 0, i);
      pixels.show();

      Serial.print("The mode temperature is: ");
      Serial.println(T.first);      
    }
  }
}