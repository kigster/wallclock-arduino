cmake_minimum_required(VERSION 3.8)
message("src/application/CMakeLists.txt")

set(SOURCES
        Encoder.cpp
        RotaryEncoderWithButton.cpp
        App.cpp
        NeoPixelEffects.cpp
        NeoPixelManager.cpp
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
        BOARD uno)
