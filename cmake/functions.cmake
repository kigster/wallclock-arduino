
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
    else()
        message(FATAL_ERROR "Too many devices have been detected! Force device by setting 'BOARD_DEVICE' variable, or unplug one or more devices!")
    endif ()

    set(ENV{BOARD_DEVICE} ${BOARD_DEVICE})
    set(BOARD_DEVICE PARENT_SCOPE)
ENDFUNCTION(detect_arduino_device)



FUNCTION(build_library LIB LIB_SOURCE_PATH)
    set(${LIB}_RECURSE true)

    include_directories(${LIB_SOURCE_PATH} ${LIB_SOURCE_PATH}/utility)
    link_directories(${LIB_SOURCE_PATH})

    set(LIB_SOURCES $ENV{${LIB}_SOURCES})
    set(LIB_HEADERS $ENV{${LIB}_HEADERS})

    separate_arguments(LIB_SOURCES)
    separate_arguments(LIB_HEADERS)

#    list(LENGTH LIB_SOURCES num_sources)
#    list(LENGTH LIB_HEADERS num_headers)
#    message(STATUS "total ${num_sources} sources, and ${num_headers} extra headers")
#
    prepend(LIB_SOURCES ${LIB_SOURCE_PATH} ${LIB_SOURCES})
    prepend(LIB_HEADERS ${LIB_SOURCE_PATH} ${LIB_HEADERS})

    message("LIB_SOURCES: ${LIB_SOURCES}\nLIB_HEADERS: ${LIB_HEADERS}")

    if(NOT DEFINED ENV{${LIB}_ONLY_HEADER})
        list(APPEND LIB_SOURCES ${LIB_SOURCE_PATH}/${LIB}.cpp)
    endif()

    GENERATE_ARDUINO_LIBRARY(${LIB}
            SRCS ${LIB_SOURCE_PATH}/${LIB}.h ${LIB_SOURCES} ${LIB_HEADERS}
            HDRS ${LIB_SOURCE_PATH}/${LIB}.h ${LIB_HEADERS}
            BOARD $ENV{BOARD_NAME})
ENDFUNCTION(build_library)


FUNCTION(prepend var prefix)
   SET(listVar "")
   FOREACH(f ${ARGN})
      LIST(APPEND listVar "${prefix}/${f}")
   ENDFOREACH(f)
   SET(${var} "${listVar}" PARENT_SCOPE)
ENDFUNCTION(prepend)
