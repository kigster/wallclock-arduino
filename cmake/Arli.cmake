#=============================================================================#
# Author:    Konstantin Gredeskoul (kigster)
# Home:      https://github.com/kigster/arli
# License:   MIT
# Copyright: (C) 2017 Konstantin Gredeskoul
#=============================================================================#


#=============================================================================#
# detect_arduino_device
# [PUBLIC]
#
# detect_arduino_device()
#
# Automatically detects a USB Arduino Serial port by doing ls on /dev
# Errors if more than 1 port was found, or if none were found.
# Set environment variable BOARD_DEVICE to override auto-detection.
#=============================================================================#
FUNCTION(detect_arduino_device)
    if (DEFINED ENV{BOARD_DEVICE})
        message(STATUS "Using device from environment variable BOARD_DEVICE")
        set(BOARD_DEVICE $ENV{BOARD_DEVICE})
        set(BOARD_DEVICE PARENT_SCOPE)
    else ()
        message(STATUS "Auto-detecting board device from /dev")
        execute_process(
                COMMAND "/usr/bin/find" "-s" "/dev" "-name" "cu.*serial*"
                OUTPUT_VARIABLE BOARD_DEVICE
                ERROR_VARIABLE STDERR
                OUTPUT_STRIP_TRAILING_WHITESPACE)
    endif ()

    STRING(REGEX REPLACE "\n" ";" BOARD_DEVICE "${BOARD_DEVICE}")

    separate_arguments(BOARD_DEVICE)

    list(LENGTH BOARD_DEVICE NUM_DEVICES)
    message(STATUS "Total of ${NUM_DEVICES} devices have been found.")

    if (${NUM_DEVICES} EQUAL 0)
        message(FATAL_ERROR "No devices have been found, aborting!")
    elseif (${NUM_DEVICES} EQUAL 1)
        message(STATUS "Auto-detected 1 device ${BOARD_DEVICE}, continuing...")
    else ()
        message(FATAL_ERROR "Too many devices have been detected! Force device by setting 'BOARD_DEVICE' variable, or unplug one or more devices!")
    endif ()

    set(ENV{BOARD_DEVICE} ${BOARD_DEVICE})
    set(BOARD_DEVICE PARENT_SCOPE)
ENDFUNCTION(detect_arduino_device)


#=============================================================================#
# arli_libraries
# [PUBLIC]
#
# arli_libraries(SOURCE_DIR)
#
#      SOURCE_DIR   - where to look for arli.json
#
# Processes arli.json file in the main source folder to build a list of
# dependent libraries. Exports an environment variable ARLI_LIBRARIES as a
# semi-colon separated list of library names.
#=============================================================================#
FUNCTION(arli_libraries SOURCE_DIR)
    execute_process(
            COMMAND "/usr/bin/ruby" "-e" "require \"yaml\"; YAML.load(File.read(\"ArliFile.yml\"))[\"dependencies\"].map{ |k| k[\"name\"]}.each {|l| printf l + \";\"}"
            OUTPUT_VARIABLE ARLI_LIBRARIES_STDOUT
            ERROR_VARIABLE ARLI_LIBRARIES_STDERR
            OUTPUT_STRIP_TRAILING_WHITESPACE
            WORKING_DIRECTORY ${SOURCE_DIR}
    )

    message(STATUS ${ARLI_LIBRARIES_STDERR})
    #STRING(REGEX REPLACE "\n" ";" ARLI_LIBRARIES ${ARLI_LIBRARIES_STDOUT})
    set(ARLI_LIBRARIES "${ARLI_LIBRARIES_STDOUT}" PARENT_SCOPE)
    message(STATUS "Auto-loaded ARLI Libraries: ${ARLI_LIBRARIES}")
ENDFUNCTION(arli_libraries)

#=============================================================================#
# build_library
# [PUBLIC]
#
# build_library(LIB LIB_SOURCE_PATH)
#
#      LIB               - name of the library to build
#      LIB_SOURCE_PATH   - path to the top-level 'libraries' folder.
#
# Builds a library as a static .a library that can be linked by the main
# target.
#=============================================================================#
FUNCTION(build_library LIB LIB_SOURCE_PATH)
    set(${LIB}_RECURSE true)

    include_directories(${LIB_SOURCE_PATH} ${LIB_SOURCE_PATH}/utility)
    link_directories(${LIB_SOURCE_PATH})

    set(LIB_SOURCES $ENV{${LIB}_SOURCES})
    set(LIB_HEADERS $ENV{${LIB}_HEADERS})

    separate_arguments(LIB_SOURCES)
    separate_arguments(LIB_HEADERS)

    prepend(LIB_SOURCES ${LIB_SOURCE_PATH} ${LIB_SOURCES})
    prepend(LIB_HEADERS ${LIB_SOURCE_PATH} ${LIB_HEADERS})

    if (NOT DEFINED ENV{${LIB}_ONLY_HEADER})
        list(APPEND LIB_SOURCES ${LIB_SOURCE_PATH}/${LIB}.cpp)
    endif ()

    GENERATE_ARDUINO_LIBRARY(${LIB}
            SRCS ${LIB_SOURCE_PATH}/${LIB}.h ${LIB_SOURCES} ${LIB_HEADERS}
            HDRS ${LIB_SOURCE_PATH}/${LIB}.h ${LIB_HEADERS}
            BOARD $ENV{BOARD_NAME})
ENDFUNCTION(build_library)

#=============================================================================#
# prepend
# [PUBLIC]
#
# prepend(var prefix)
#
#      var      - variable containing a list
#      prefix   - string to prepend to each list item
#
#=============================================================================#
FUNCTION(prepend var prefix)
    SET(listVar "")
    FOREACH (f ${ARGN})
        LIST(APPEND listVar "${prefix}/${f}")
    ENDFOREACH (f)
    SET(${var} "${listVar}" PARENT_SCOPE)
ENDFUNCTION(prepend)
