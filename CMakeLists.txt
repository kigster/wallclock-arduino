## ARDUINO Boards:
##                   menu:
##                    yun: Arduino Y
##                    uno: Arduino/Genuino Uno
##              diecimila: Arduino Duemilanove or Diecimila
##                   nano: Arduino Nano
##                   mega: Arduino/Genuino Mega or Mega 2560
##                megaADK: Arduino Mega ADK
##               leonardo: Arduino Leonardo
##            leonardoeth: Arduino Leonardo ETH
##                  micro: Arduino/Genuino Micro
##                esplora: Arduino Esplora
##                   mini: Arduino Mini
##               ethernet: Arduino Ethernet
##                    fio: Arduino Fio
##                     bt: Arduino BT
##             LilyPadUSB: LilyPad Arduino USB
##                lilypad: LilyPad Arduino
##                    pro: Arduino Pro or Pro Mini
##               atmegang: Arduino NG or older
##           robotControl: Arduino Robot Control
##             robotMotor: Arduino Robot Motor
##                  gemma: Arduino Gemma
##     circuitplay32u4cat: Adafruit Circuit Playground
##                yunmini: Arduino Y
##                chiwawa: Arduino Industrial 101
##                    one: Linino One
##                unowifi: Arduino Uno WiFi
##
## ARDUINO Programmers:
##            avrisp: AVR ISP
##        avrispmkii: AVRISP mkII
##        usbtinyisp: USBtinyISP
##        arduinoisp: ArduinoISP
##     arduinoisporg: ArduinoISP.org
##            usbasp: USBasp
##          parallel: Parallel Programmer
##      arduinoasisp: Arduino as ISP
##          usbGemma: Arduino Gemma
##         buspirate: BusPirate as ISP
##            stk500: Atmel STK500 development board
##          jtag3isp: Atmel JTAGICE3 (ISP mode)
##             jtag3: Atmel JTAGICE3 (JTAG mode)
##         atmel_ice: Atmel-ICE (AVR)
##

cmake_minimum_required(VERSION 3.2.2)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
set(PROJECT_DIR ${CMAKE_CURRENT_SOURCE_DIR})

if (NOT DEFINED ENV{ARDUINO_SDK_PATH})
    set(ARDUINO_SDK_PATH /Applications/Arduino.app/Contents/Java)
else ()
    set(ARDUINO_SDK_PATH $ENV{ARDUINO_SDK_PATH})
endif ()

if (NOT DEFINED $ENV{ARDUINO_CUSTOM_LIBRARY_PATH})
    set(ARDUINO_CUSTOM_LIBRARY_PATH /Documents/Arduino/Libraries)
else ()
    set(ARDUINO_CUSTOM_LIBRARY_PATH $ENV{ARDUINO_CUSTOM_LIBRARY_PATH})
endif ()

set(ARDUINO_SDK_LIBRARY_PATH "${ARDUINO_SDK_PATH}/libraries")
set(ARDUINO_SDK_HARDWARE_LIBRARY_PATH "${ARDUINO_SDK_PATH}/hardware/arduino/avr/libraries")
set(ARDUINO_CUSTOM_LIBRARY_PATH $ENV{ARDUINO_CUSTOM_LIBRARY_PATH})

if (NOT DEFINED ENV{BOARD_NAME})
    set(ENV{BOARD_NAME} "uno")
endif ()
message(STATUS "Board name is set with ENV[BOARD_NAME], currently: [$ENV{BOARD_NAME}]")

# Include Custom Functions
include(cmake/Arli.cmake )
# Special flags for some particular boards
set(nano.build.mcu atmega328p)
set(nano.upload.speed 57600)

# Auto-detect from /dev
detect_arduino_device()

set(BOARD_DEVICE $ENV{BOARD_DEVICE})
message(STATUS "Device [${BOARD_DEVICE}] have been selected.")

# Import Arduino Toolchain
set(CMAKE_TOOLCHAIN_FILE ${PROJECT_DIR}/cmake/ArduinoToolchain.cmake)

# These are Arduino provided libs we depend on
set(ARDUINO_LIBRARIES Wire)

# Let's load libraries from 'arli.json'
arli_libraries(${CMAKE_CURRENT_SOURCE_DIR})

set(CUSTOM_LIBRARIES "$ENV{ARLI_LIBRARIES}")
separate_arguments(CUSTOM_LIBRARIES)

# Define various special flags on some libs
set(ENV{Wire_HEADERS} utility/twi.h)
set(ENV{Wire_SOURCES} utility/twi.c)
set(ENV{Adafruit_Sensor_ONLY_HEADER} 1)

project(WallClock3 C CXX)

if ($ENV{DEBUG})
    print_board_list()
    print_programmer_list()
endif ()

add_subdirectory(src)   #add the example directory into build
