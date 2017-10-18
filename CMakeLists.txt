set(ARDUINO_SDK_PATH /Applications/Arduino.app/Contents/Java)
set(CMAKE_TOOLCHAIN_FILE cmake/ArduinoToolchain.cmake) # Arduino Toolchain
set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -std=c++11" )

cmake_minimum_required(VERSION 3.0)
project(WallClock3 C CXX)

# print_board_list()
# print_programmer_list()

add_subdirectory(src)   #add the example directory into build
