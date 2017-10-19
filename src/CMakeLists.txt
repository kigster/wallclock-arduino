cmake_minimum_required(VERSION 3.3)

FOREACH(LIB ${CUSTOM_LIBRARIES})
    build_library(${LIB} "${ARDUINO_CUSTOM_LIBRARY_PATH}/${LIB}")
ENDFOREACH(LIB)

FOREACH(LIB ${ARDUINO_LIBRARIES})
    build_library(${LIB} "${ARDUINO_SDK_HARDWARE_LIBRARY_PATH}/${LIB}/src")
ENDFOREACH(LIB)

add_subdirectory("application")

set(SOURCE_FILES WallClock.cpp)

generate_arduino_firmware(wallclock
        SRCS ${SOURCE_FILES}
        LIBS application ${CUSTOM_LIBRARIES} ${ARDUINO_LIBRARIES}
        BOARD $ENV{BOARD_NAME}
        PORT $ENV{BOARD_DEVICE}
        )
