# CASA0016_smart_clay_timer
## About
This repository is for a smart clay timer project with arduino for the CASA0016 course. It uses a Feather Huzzah to measure temperature non-invasively using an infrared thermal camera and uses the change in temperature of clay as it dries as an indicator of progress of the drying process. This progress is displayed tangibly using a strip of RGB LEDs, much like a loading bar. With some calibration, this project may also be suitable for measuring the drying of other materials such as paint or as an automatic contactless thermometer.

## Materials and dependencies
### Materials
The following hardware was used in this project:
- Adafruit Feather Huzzah (but you can use any ESP8266-based board)
- SR04 Ultrasonic distance sensor / range finder
- MLX90640 thermal camera (on Pimoroni's board)
- DHT22 temperature and humidity sensor
- Adafruit Neopixel stick (with 8 LEDs)
- Copper wires (in black, red, etc. sleeves)
- 3 10k Ohm resistors

### Dependencies
The following dependencies must be installed for this project to work:
- ESP8266 drivers for Arduino IDE
- Adafruit MLX90640 library
- Adafruit Neopixel library
- Math library
- DHT22 libraries

## Circuitry
The ultrasonic sensor and Neopixel LED strip require a 5V circuit to be powered, while the other sensors can work at 3.3V. The logic of the Feather Huzzah is 3.3V, hence it makes sense to maintain most of the load on the 3.3V circuit and provide a seperate circuit from the USB pin (5V) of the Huzzah to power the ultrasonic sensor and the LEDs. For the ultrasonic sensor, 2 10k Ohm resistors are necessary to create a voltage divider to return a signal back to the Huzzah at 2.5V, within the logic level of the microcontroller. A further 10k Ohm resistor is also used as pull-up for the DHT22 sensor output pin.

![CASA0016_smart_clay_timer_bb](https://github.com/ethmacc/CASA0016_smart_clay_timer/assets/60006290/426314c8-7e47-4111-8254-98b834e040ec)

![CASA0016_smart_clay_timer_schem](https://github.com/ethmacc/CASA0016_smart_clay_timer/assets/60006290/63205188-b591-4609-833f-8ae326127e08)
