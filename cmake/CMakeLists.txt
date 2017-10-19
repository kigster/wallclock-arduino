#set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -std=c++11" )
#set(CMAKE_TOOLCHAIN_FILE ${CMAKE_CURRENT_LIST_DIR}/cmake/ArduinoToolchain.cmake)
#
#cmake_minimum_required(VERSION 3.0)
#
#include_directories(
#        ${ARDUINO_LIBRARY_HOME}
#        ${ARDUINO_SDK_LIBRARY_PATH}
#        ${ARDUINO_SDK_HARDWARE_LIBRARY_PATH}
#)
#
#link_directories(
#        ${ARDUINO_LIBRARY_HOME}
#        ${ARDUINO_SDK_LIBRARY_PATH}
#        ${ARDUINO_SDK_HARDWARE_LIBRARY_PATH}
#)
#
#set(ARDUINO_LIBRARIES
#        Wire
#)
#
#set(CUSTOM_LIBRARIES
#        Encoder
#        RotaryEncoderWithButton
#        DS1307RTC
#        Adafruit_LEDBackpack
#        Adafruit_GFX
#        Adafruit_NeoPixel
#        OneButton
#        SimpleTimer
#        Time
#        )
#
#foreach(LIB ${CUSTOM_LIBRARIES})
#    include_directories(${ARDUINO_LIBRARY_HOME}/${LIB})
#    link_directories(${ARDUINO_LIBRARY_HOME}/${LIB})
#
#    GENERATE_ARDUINO_LIBRARY(${LIB}
#            SRCS ${ARDUINO_LIBRARY_HOME}/${LIB}/${LIB}.cpp
#            HDRS ${ARDUINO_LIBRARY_HOME}/${LIB}/${LIB}.h
#            BOARD uno)
#endforeach(LIB)
#
#foreach(LIB ${ARDUINO_LIBRARIES})
#    include_directories(${ARDUINO_LIBRARY_HOME}/${LIB}/src)
#    link_directories(${ARDUINO_LIBRARY_HOME}/${LIB}/src)
#
#    GENERATE_ARDUINO_LIBRARY(${LIB}
#            SRCS ${ARDUINO_LIBRARY_HOME}/src/${LIB}/${LIB}.cpp
#            HDRS ${ARDUINO_LIBRARY_HOME}/src/${LIB}/${LIB}.h
#            BOARD uno)
#endforeach(LIB)
