cmake_minimum_required(VERSION 3.0)
set(ARDUINO_LIBRARY_HOME /Users/kig/Dropbox/Workspace/oss/arduino/libraries)

set(DEPENDENCIES
        Encoder
        RotaryEncoderWithButton
        DS1307RTC
        Adafruit_LEDBackpack
        Adafruit_GFX
        Adafruit_NeoPixel
        OneButton
        SimpleTimer
        Time
        )


foreach(LIB ${DEPENDENCIES})
    include_directories(${ARDUINO_LIBRARY_HOME}/${LIB})
    link_directories(${ARDUINO_LIBRARY_HOME}/${LIB})

    generate_arduino_library(${LIB}
            SRCS ${ARDUINO_LIBRARY_HOME}/${LIB}/${LIB}.cpp
            HDRS ${ARDUINO_LIBRARY_HOME}/${LIB}/${LIB}.h
            BOARD uno)
    set(${LIB}_RECURSE true)
endforeach(LIB)

#include_directories(
#        ${ARDUINO_LIBRARY_HOME}/Encoder
#        ${ARDUINO_LIBRARY_HOME}/RotaryEncoderWithButton
#        ${ARDUINO_LIBRARY_HOME}/DS1307RTC
#        ${ARDUINO_LIBRARY_HOME}/Adafruit_LEDBackpack
#        ${ARDUINO_LIBRARY_HOME}/Adafruit_GFX
#        ${ARDUINO_LIBRARY_HOME}/Adafruit_NeoPixel
#        ${ARDUINO_LIBRARY_HOME}/OneButton
#        ${ARDUINO_LIBRARY_HOME}/SimpleTimer
#        ${ARDUINO_LIBRARY_HOME}/time
#)

#link_directories(${ARDUINO_LIBRARY_HOME})
#
#
#link_directories(${ARDUINO_LIBRARY_HOME}
#        ${ARDUINO_LIBRARY_HOME}/Encoder
#        ${ARDUINO_LIBRARY_HOME}/RotaryEncoderWithButton
#        ${ARDUINO_LIBRARY_HOME}/DS1307RTC
#        ${ARDUINO_LIBRARY_HOME}/Adafruit_LEDBackpack
#        ${ARDUINO_LIBRARY_HOME}/Adafruit_GFX
#        ${ARDUINO_LIBRARY_HOME}/Adafruit_NeoPixel
#        ${ARDUINO_LIBRARY_HOME}/OneButton
#        ${ARDUINO_LIBRARY_HOME}/SimpleTimer
#        ${ARDUINO_LIBRARY_HOME}/time
#        )

#
#generate_arduino_library(Encoder
#        SRCS ${ARDUINO_LIBRARY_HOME}/Encoder/Encoder.cpp
#        HDRS ${ARDUINO_LIBRARY_HOME}/Encoder/Encoder.h
#        BOARD uno)
#
#generate_arduino_library(Encoder
#        SRCS ${ARDUINO_LIBRARY_HOME}/Encoder/Encoder.cpp
#        HDRS ${ARDUINO_LIBRARY_HOME}/Encoder/Encoder.h
#        BOARD uno)
#
#generate_arduino_library(Adafruit_NeoPixel
#        SRCS ${ARDUINO_LIBRARY_HOME}/Adafruit_NeoPixel/Adafruit_NeoPixel.cpp
#        HDRS ${ARDUINO_LIBRARY_HOME}/Adafruit_NeoPixel/Adafruit_NeoPixel.h
#        BOARD uno)
#
#generate_arduino_library(RotaryEncoderWithButton
#        SRCS ${ARDUINO_LIBRARY_HOME}/RotaryEncoderWithButton/RotaryEncoderWithButton.cpp
#        HDRS ${ARDUINO_LIBRARY_HOME}/RotaryEncoderWithButton/RotaryEncoderWithButton.h
#        BOARD uno)
#


set(Wire_RECURSE True)
set(Encoder_RECURSE True)
set(RotaryEncoderWithButton_RECURSE True)
set(DS1307RTC_RECURSE True)
set(Adafruit_LEDBackpack_RECURSE True)
set(Adafruit_GFX_RECURSE True)
set(Adafruit_NeoPixel_RECURSE True)

set(SOURCES
        App.cpp
        NeoPixelEffects.cpp
        NeoPixelManager.cpp
        SetTimeHelper.cpp
        SetTimeMenu.cpp
        )

set(HEADERS
        App.h
        GaugedValue.h
        NeoPixelEffects.h
        NeoPixelManager.h
        SetTimeHelper.h
        SetTimeMenu.h
        State.h
        Wallock.h
        )

generate_arduino_library(application
        SRCS ${SOURCES}
        HDRS ${HEADERS}
        LIBS ${DEPENDENCIES}
#        LIBS Encoder RotaryEncoderWithButton DS1307RTC Adafruit_LEDBackpack Adafruit_GFX Adafruit_NeoPixel OneButton SimpleTimer time
        BOARD uno)
