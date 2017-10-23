[![Build Status](https://travis-ci.org/kigster/wallclock-arduino.svg?branch=master)](https://travis-ci.org/kigster/wallclock-arduino)

## Wall Clock 

This is a repo of a complex Arduino project, which relies on several custom and built-in libraries.

This project originated from [arduino-cmake](https://github.com/arduino-cmake/arduino-cmake), but the author could not figure out how to properly build libraries, neither Arduino built-in libraries, or the custom user-provided libraries.

So a CMake extension was born out of desperation, and a few goodies were added along the way: 

 * You can change the board type by setting `BOARD_NAME` environment variable to, say, "nano"
 * You can force the port/device using `BOARD_DEVICE` environment variable. If not set, device it attempted to be automatically detected by listing various ports under `/dev`. Note, that if you are relying on auto-detection, be sure not to have more than one board plugged in. If CMake function detects more than one device, it gives up, and requires you to set it externally.


## Building the Project

To build the project, you need a recent version of the Arduino IDE (or SDK), and a version of CMake later than 3.2.2. You also need a version of ruby installed, and if you are using the "system" ruby, please run the following command prior to the build:

```bash
sudo gem install arli
```

Once prerequisites are satisfied:

```bash
$ git clone https://... wallclock-arduino
$ cd wallclock-arduino
$ arli install 
```

This next step requires you to have your board plugged in, and a serial USB device available under `/dev`. 

If you don't have anything plugged in, set `export BOARD_DEVICE=/dev/null` before the build.

If have more than one board plugged in, set it to the appropriate device for the board you are building — `export BOARD_DEVICE=/dev/cu.XXXX`.

```bash
$ cd src && cmake ..
$ make 
$ make upload # to laod firmware
```

The default board is 'Nano', but you can change it in the `CMakeLists.txt` file, or by setting `BOARD_NAME` environment variable before runnung `cmake`.

### Schematic

Here is an approximate schematic using Arduino Nano:

![WallClock-Schematic.png](doc/WallClock-Schematic.png)


### Dependencies

This project depends on the following libraries:

 * DS1307RTC
 * Adafruit_LEDBackpack
 * Adafruit_GFX
 * Adafruit_NeoPixel
 * Adafruit_Sensor
 * DHT
 * OneButton
 * SimpleTimer
 * Time
 
## Contributing

Bug reports and pull requests are welcome on GitHub at [https://github.com/kigster/wallclock-arduino](https://github.com/kigster/wallclock-arduino).

## Author

<p>&copy; 2014-2017 Konstantin Gredeskoul, all rights reserved.</p>

## License

This project is distributed under the [MIT License](https://raw.githubusercontent.com/kigster/wallclock-arduino/master/LICENSE).
