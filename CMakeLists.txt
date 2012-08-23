cmake_minimum_required (VERSION 2.6)
project (Gamma)

FILE(GLOB_RECURSE GAMMA_SRC ${CMAKE_SOURCE_DIR}/src/*.cpp)

include_directories(../Gamma/)
add_library(Gamma ${GAMMA_SRC})

SET(LIBRARY_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/build/lib)
SET(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/build/lib)

# TODO: install targets
# TODO: tests and examples
# TODO: release/debug builds
# TODO: allow choosing static or dynamic libraries
# TODO: allow choosing architecture 64 vs. 32 bits on 64 bit platforms (e.g. OS X)

# IF(WANT_DEBUG)
#     SET(CMAKE_BUILD_TYPE Debug)
#     SET(GAMMA_DEBUG TRUE)
# ELSE()
#     SET(CMAKE_BUILD_TYPE Release)
#     SET(GAMMA_DEBUG FALSE)
# ENDIF()

# IF(APPLE)
# 	#Compile for 32bit and 64bit intel macs
# 	SET (CMAKE_OSX_ARCHITECTURES "i386;x86_64;")
# ENDIF()

# ------------
# Dependencies

SET(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake ${CMAKE_MODULE_PATH}) 
INCLUDE(FindHelper)
FIND_HELPER(PORTAUDIO portaudio-2.0 portaudio.h portaudio)
FIND_HELPER(LIBSNDFILE sndfile sndfile.h sndfile)