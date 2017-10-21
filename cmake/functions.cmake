
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

FUNCTION(arli_libraries SOURCE_DIR)
    execute_process(
            COMMAND "/usr/bin/ruby" "-e" "require \"json\"; JSON.load(File.read(\"arli.json\"))[\"dependencies\"].map{ |k| k[\"name\"]}.each {|l| printf l + \";\"}"
            OUTPUT_VARIABLE ARLI_LIBRARIES_STDOUT
            ERROR_VARIABLE ARLI_LIBRARIES_STDERR
            OUTPUT_STRIP_TRAILING_WHITESPACE
            WORKING_DIRECTORY ${SOURCE_DIR}
    )
    #STRING(REGEX REPLACE "\n" ";" ARLI_LIBRARIES ${ARLI_LIBRARIES_STDOUT})
    set(ENV{ARLI_LIBRARIES} "${ARLI_LIBRARIES_STDOUT}")
    message(STATUS "Auto-loaded ARLI Libraries: $ENV{ARLI_LIBRARIES}")
ENDFUNCTION(arli_libraries)

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


FUNCTION(prepend var prefix)
    SET(listVar "")
    FOREACH (f ${ARGN})
        LIST(APPEND listVar "${prefix}/${f}")
    ENDFOREACH (f)
    SET(${var} "${listVar}" PARENT_SCOPE)
ENDFUNCTION(prepend)
