
set(ARDUINO_LIBRARY_HOME /Users/kig/Dropbox/Workspace/oss/arduino/libraries)

include_directories(
        ${ARDUINO_SDK_PATH}/libraries
        ${ARDUINO_LIBRARY_HOME}/Encoder
        ${ARDUINO_LIBRARY_HOME}/RotaryEncoderWithButton
        ${ARDUINO_LIBRARY_HOME}/DS1307RTC
        ${ARDUINO_LIBRARY_HOME}/Adafruit_LEDBackpack
        ${ARDUINO_LIBRARY_HOME}/Adafruit_GFX
        ${ARDUINO_LIBRARY_HOME}/Adafruit_NeoPixel
        ${ARDUINO_LIBRARY_HOME}/OneButton
        ${ARDUINO_LIBRARY_HOME}/SimpleTimer
        ${ARDUINO_LIBRARY_HOME}/time
)

link_directories(${ARDUINO_LIBRARY_HOME}
        ${ARDUINO_SDK_PATH}/libraries
        ${ARDUINO_LIBRARY_HOME}/Encoder
        ${ARDUINO_LIBRARY_HOME}/RotaryEncoderWithButton
        ${ARDUINO_LIBRARY_HOME}/DS1307RTC
        ${ARDUINO_LIBRARY_HOME}/Adafruit_LEDBackpack
        ${ARDUINO_LIBRARY_HOME}/Adafruit_GFX
        ${ARDUINO_LIBRARY_HOME}/Adafruit_NeoPixel
        ${ARDUINO_LIBRARY_HOME}/OneButton
        ${ARDUINO_LIBRARY_HOME}/SimpleTimer
        ${ARDUINO_LIBRARY_HOME}/time
        )

add_subdirectory("application")

generate_arduino_firmware(wallclock
        SRCS WallClock3.cpp
        LIBS application
        ARDLIBS libWire OneButton Adafruit_NeoPixel  
        BOARD uno
        PORT /dev/cu.usbserial-DN01AF0T)

# GENERATE_ARDUINO_FIRMWARE(serial_lib
#         SRCS serial.cpp
#         ARDLIBS SoftwareSerial
#         BOARD uno)
#
