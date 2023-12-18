# CASA0016_smart_clay_timer (PotteryProgress)
## About
This repository is for a smart clay timer project with arduino for the CASA0016 course. It uses a Feather Huzzah to measure temperature non-invasively using an infrared thermal camera and uses the change in temperature of clay as it dries as an indicator of progress of the drying process. This progress is displayed tangibly using a strip of RGB LEDs, much like a loading bar. With some calibration, this project may also be suitable for measuring the drying of other materials such as paint or as an automatic contactless thermometer.

![IMG_6635](https://github.com/ethmacc/CASA0016_smart_clay_timer/assets/60006290/8892c229-0019-418b-b67e-2db27e8d7fab)

_PotteryProgress: The Smart Clay Timer/Sensor_

![IMG_6637](https://github.com/ethmacc/CASA0016_smart_clay_timer/assets/60006290/0089a92f-b200-4ee0-b7b1-24000745c1bd)

_Inital concept sketch. Note the differences in final colours used and the original position of the DHT22 sensor_

## Materials and dependencies
### Materials
The following hardware was used in this project:
- **Adafruit Feather Huzzah** (or similar ESP8266-based board)
- **SR04** Ultrasonic distance sensor / range finder
- **MLX90640 thermal camera** (Pimoroni or similar with integrated surface-mount voltage regulation components)
- **DHT22** temperature and humidity sensor
- **Adafruit Neopixel stick** (with 8 LEDs)
- **Copper wires** (in black, red, etc. sleeves)
- 3 **10k Ohm resistors**

### Dependencies
The following dependencies must be installed for this project to work:
- **ESP8266 board package** to ensure the Arduino IDE can communicate and write code to the Feather Huzzah
- **Adafruit MLX90640 library** I found this simpler to use than the Sparkfun libraries and sketches suggested by Pimoroni 
- **Adafruit Neopixel library** to control the Neopixels
- **Math library** for the round function to round floating point numbers to integers
- **Adafruit DHT22 sensor library** to interface and read from the DHT22 sensor
- **ESP8266WifiMulti.h** to connect to multiple WiFi networks, including my home and university networks
- **ESP8266WebServer** to use the ESP8266 as a web server capable of hosting simple webpages

## Method
### Wiring and circuitry
The ultrasonic sensor and Neopixel LED strip require a 5V circuit to be powered, while the other sensors can work at 3.3V. The logic of the Feather Huzzah is 3.3V, hence it makes sense to maintain most of the load on the 3.3V circuit and provide a seperate circuit from the USB pin (5V) of the Huzzah to power the ultrasonic sensor and the LEDs. The disadvantage of taking such an approach with the Feather Huzzah is that a 5V power supply must be connected to the USB port to provide 5V to the USB output pin. This means that the device must be plugged into a 5V wall adapter or into a 5V power bank, which limits the settings in which it may be deployed. However, as this device is envisioned to be used within an indoor pottery studio setting, it is expected to be plugged into mains power most of the time.

For the various sensors, the following wiring was used:
- **Ultrasonic sensor** - pins 13 and 15 for the Echo (input, receiving the returning ultrasonic pulse) and Trigger (output, producing the ultrsonic pulse) pins respectively. 2 10k Ohm resistors are necessary to create a voltage divider to return a signal back to the Huzzah at 2.5V, within the logic level of the microcontroller
- **DHT22** - pin 2 for the input. A 10k Ohm resistor is used as pull-up for this pin.
- **Neopixel** - pin 14 for the data_in (output signal from the ESP8266)
- **MLX 90640 Thermal camera** - the MLX90640 uses I2C to communicate with the ESP8266 (green wires) so connections to the Feather Huzzah should be straightforward as the SDA and SCL pins that make up the I2C connection are clearly labelled on the microcontroller's pinouts. Note that the illustration used here is for Adafruit's version of the MLX90640 board and therefore has some minor differences to Pimoroni's version. In particular, the Adafruit version has a 3V output pin which can be used to connect another load in series on the 3V circuit, whereas the Pimoroni version (used in this project) has an unused pin that serves no function. 

![CASA0016_smart_clay_timer_bb](https://github.com/ethmacc/CASA0016_smart_clay_timer/assets/60006290/426314c8-7e47-4111-8254-98b834e040ec)

_Fritzing breadboard diagram_


![CASA0016_smart_clay_timer_schem](https://github.com/ethmacc/CASA0016_smart_clay_timer/assets/60006290/63205188-b591-4609-833f-8ae326127e08)

_Fritzing circuit diagram_

### Sensor testing and evalution
The Arduino built-in, DHT sensor and Adafruit MLX90640 libraries provide useful example sketches which may be used to test and understand the sensors. For the ultrasonic sensor, you can use the ```Ping``` sketch from the built-in examples to test and trouble shoot the sensor, and ensure that the pins that you have selected to produce and receive the ultrasonic pulse are behaving as expected. 

For the DHT22 sensor, the ```DHT_Unified_Sensor``` exmple sketch provides a great overview of how to define and instantiate a DHT sensor object and then take regular readings from the sensor, which should be fairly straightfoward.

The MLX90640 thermal camera forms the heart of the device and this project and therefore is worth experimenting with in a bit further detail. Adafruit provide the ```MLX90640_simpletest``` example sketch, which provides a nice and simple way to test and visualise outputs from the camera. 

### Testing with real objects

![IMG_6636](https://github.com/ethmacc/CASA0016_smart_clay_timer/assets/60006290/5c0705c4-bb31-4a7b-be17-ee3b48bb2714)

_Testing the device with a cold and moist object such as a glass of water straight from the tap_

### Software

## Prototyping the board

A PCB or maker strip board was considered essential to the project as the final device was envisioned to be quite small and compact as a potential consumer product. To achieve this, the mess of wires and the breadboard had to be miniaturised to fit into a relatively small enclosure. The most reasonable way to prototype this is to use a strip board, which has copper connections running in parallel strips along a board. New connections can be made by soldering wires from one strip to another and individual lengths of each strip can also be isolated by breaking the copper channel using a drill bit in a pin vise or another similar tool. In the image below for example, the pins on each side of the Feather Huzzah are isolated from each other using this technique:

![IMG_6621](https://github.com/ethmacc/CASA0016_smart_clay_timer/assets/60006290/c670622b-afc2-4fd6-87d4-5d68f26da965)

![IMG_6624](https://github.com/ethmacc/CASA0016_smart_clay_timer/assets/60006290/45e6d16f-c744-4ba2-8627-0867a37a305b)

The more expensive components, such as the Feather Huzzah and the MLX90640, were connected using male/female header pins so that they could be removed and reused, while components that had to be attached at specific positions below the front face of the enclosure were attached with breadboard connectors to allow some flexibility over their final placement.

![IMG_6631](https://github.com/ethmacc/CASA0016_smart_clay_timer/assets/60006290/542c5e1c-0caa-4eaf-976d-3aee22248199)

![IMG_6630](https://github.com/ethmacc/CASA0016_smart_clay_timer/assets/60006290/c246ca08-078d-48ed-b230-d804779aac5a)

## Enclosure

The enclosure was based off a simple box design generated using the MakerCase website (https://en.makercase.com/#/)

![IMG_6629](https://github.com/ethmacc/CASA0016_smart_clay_timer/assets/60006290/1e3b3046-d373-4fd8-bcec-70253299c321)

_The rear, side and bottom faces of the enclosure were to form a single piece, so these were glue together with Gorilla wood glue_

The openings for the various sensors were measured and drawn into the .dxf file using Fusion360. The .dxf file was then used to lasercut 3mm plywood to make the box. Where errors were made, more precise measurements were taken with a pair of vernier calipers and used to correct the drawings.

![IMG_6648](https://github.com/ethmacc/CASA0016_smart_clay_timer/assets/60006290/1d0b83af-2656-45d1-b8ad-90b68dab840f)

_Discarded plywood pieces of the enclosure with hand-written markings showing corrections to be made for the next cut_

To secure the ultrasonic sensor and the Neopixel strip to the enclosure body, 2M screws and nuts were used. The SR04 has holes that are sufficiently tight that nuts will probably not be necessary to hold it securely if 2M screws are used.

## Visualizing Data

### Using the ESP8266 to host a webpage

The ESP8266 can be set up as a server to host a simple webpage (https://lastminuteengineers.com/esp8266-dht11-dht22-web-server-tutorial/).

<img height="500" alt="ESP8266 webpage" src="https://github.com/ethmacc/CASA0016_smart_clay_timer/assets/60006290/51db1c8b-e9ad-4b48-aa8c-53d2612b09ee">

The HTML code for the webpage can be found in the main arduino file itself:

```
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
```
The Google fonts website (https://fonts.google.com/) was linked into the HTML file to provide the Silkscreen font, which felt like it would work well visually with the robot face formed by the sensor ports in the front face of the enclosure. To provide some illustration and visual interest, a simple drawing of a pot, drawn by the author, was also embedded in the webpage.

### Reading the lights

## Future implementations
- Simple LCD screen to show the thermal camera output in visual form, as well as display the device's IP address so that a user can browse to it to see the full set of data being collected.
- Vibration motor/ buzzer, which could be used to indicate when the clay is done and dry.
- A dedicated PCB so that the device USB port can be placed at a more convenient location on its side.

