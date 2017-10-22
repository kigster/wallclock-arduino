cmake_minimum_required(VERSION 3.2.2)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCES
        Encoder.cpp
        RotaryEncoderWithButton.cpp
        App.cpp
        SetTimeHelper.cpp
        SetTimeMenu.cpp
        )

set(HEADERS
        RotaryEncoderWithButton.h
        Encoder.h
        utility/interrupt_config.h
        utility/interrupt_pins.h
        App.h
        GaugedValue.h
        SetTimeHelper.h
        SetTimeMenu.h
        State.h
        Configuration.h
        )

generate_arduino_library(application
        SRCS ${SOURCES}
        HDRS ${HEADERS}
        BOARD $ENV{BOARD_NAME})
