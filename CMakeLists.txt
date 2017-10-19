cmake_minimum_required(VERSION 3.8)
message("./CMakeLists.txt")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
set(PROJECT_DIR ${CMAKE_CURRENT_SOURCE_DIR})

set(ARDUINO_SDK_PATH /Applications/Arduino.app/Contents/Java)
set(ARDUINO_SDK_LIBRARY_PATH "${ARDUINO_SDK_PATH}/libraries")
set(ARDUINO_SDK_HARDWARE_LIBRARY_PATH "${ARDUINO_SDK_PATH}/hardware/arduino/avr/libraries")
set(ARDUINO_CUSTOM_LIBRARY_PATH /Users/kig/Dropbox/Workspace/oss/arduino/libraries)

set(CMAKE_TOOLCHAIN_FILE ${PROJECT_DIR}/cmake/ArduinoToolchain.cmake)

set(ARDUINO_LIBRARIES
        Wire
        )

set(CUSTOM_LIBRARIES
        DS1307RTC
        Adafruit_LEDBackpack
        Adafruit_GFX
        Adafruit_NeoPixel
        OneButton
        SimpleTimer
        Time
        )

set(ENV{Encoder_HEADERS} "utility/interrupt_config.h utility/interrupt_pins.h")
set(ENV{Encoder_SOURCES} "utility/interrupt_config.h utility/interrupt_pins.h")
set(ENV{Wire_HEADERS} utility/twi.h)
set(ENV{Wire_SOURCES} utility/twi.c)

project(WallClock3 C CXX)

#print_board_list()
#print_programmer_list()

add_subdirectory(src)   #add the example directory into build
