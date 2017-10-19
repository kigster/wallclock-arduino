cmake_minimum_required(VERSION 3.0)
message("./src/CMakeLists.txt")


FUNCTION(prepend var prefix)
   SET(listVar "")
   FOREACH(f ${ARGN})
      LIST(APPEND listVar "${prefix}/${f}")
   ENDFOREACH(f)
   SET(${var} "${listVar}" PARENT_SCOPE)
ENDFUNCTION(prepend)

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

    message("LIB_SOURCES: ${LIB_SOURCES} | HEADERS: ${LIB_HEADERS}")

    GENERATE_ARDUINO_LIBRARY(${LIB}
            SRCS ${LIB_SOURCE_PATH}/${LIB}.cpp ${LIB_SOURCE_PATH}/${LIB}.h ${LIB_SOURCES} ${LIB_HEADERS}
            HDRS ${LIB_SOURCE_PATH}/${LIB}.h ${LIB_HEADERS}
            BOARD uno)
ENDFUNCTION(build_library)



FOREACH(LIB ${CUSTOM_LIBRARIES})
    build_library(${LIB} "${ARDUINO_CUSTOM_LIBRARY_PATH}/${LIB}")
ENDFOREACH(LIB)

FOREACH(LIB ${ARDUINO_LIBRARIES})
    build_library(${LIB} "${ARDUINO_SDK_HARDWARE_LIBRARY_PATH}/${LIB}/src/")
ENDFOREACH(LIB)

add_subdirectory("application")

set(SOURCE_FILES WallClock3.cpp)

generate_arduino_firmware(wallclock
        SRCS ${SOURCE_FILES}
        LIBS application ${CUSTOM_LIBRARIES} ${ARDUINO_LIBRARIES}
        BOARD uno
        PORT /dev/cu.usbserial-DN01AF0T
        )
