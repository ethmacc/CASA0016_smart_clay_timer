# CASA0016_smart_clay_timer
## About
This repository is for a smart clay timer project with arduino for the CASA0016 course. It uses a Feather Huzzah to measure temperature non-invasively using an infrared thermal camera and uses the change in temperature of clay as it dries as an indicator of progress of the drying process. This progress is displayed tangibly using a strip of RGB LEDs, much like a loading bar. With some calibration, this project may also be suitable for measuring the drying of other materials such as paint or as an automatic contactless thermometer.

![IMG_6635](https://github.com/ethmacc/CASA0016_smart_clay_timer/assets/60006290/8892c229-0019-418b-b67e-2db27e8d7fab)

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
- **MLX 90640 Thermal camera** - the MLX90640 uses I2C to communicate with the ESP8266 (green wires) so connections to the Feather Huzzah should be straightforward as the SDA and SCL pins that make up the I2C connection are clearly labelled on the microcontroller's pinouts. Note that the illustration used here is for Adafruit's version of the MLX90640 board and therefore has some minor differences to Pimoroni's version. In particular, the Adafruit version has a 3V output pin which can be used to connect another load in series on the 3V circuit, whereas the Pimoroni version actually used in the making of this project has an unused pin that serves no function. 

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

_Testing the device with a cold and moist object such as a glass of water straight from the tap in winter_

### Software

## Prototyping the board

![IMG_6621](https://github.com/ethmacc/CASA0016_smart_clay_timer/assets/60006290/c670622b-afc2-4fd6-87d4-5d68f26da965)

![IMG_6624](https://github.com/ethmacc/CASA0016_smart_clay_timer/assets/60006290/45e6d16f-c744-4ba2-8627-0867a37a305b)

![IMG_6631](https://github.com/ethmacc/CASA0016_smart_clay_timer/assets/60006290/542c5e1c-0caa-4eaf-976d-3aee22248199)

![IMG_6630](https://github.com/ethmacc/CASA0016_smart_clay_timer/assets/60006290/c246ca08-078d-48ed-b230-d804779aac5a)

## Enclosure

![IMG_6629](https://github.com/ethmacc/CASA0016_smart_clay_timer/assets/60006290/1e3b3046-d373-4fd8-bcec-70253299c321)

## Visulizing Data

### Using the ESP8266 to serve a webpage

<img height="500" alt="ESP8266 webpage" src="https://github.com/ethmacc/CASA0016_smart_clay_timer/assets/60006290/51db1c8b-e9ad-4b48-aa8c-53d2612b09ee">

## Future implementations


