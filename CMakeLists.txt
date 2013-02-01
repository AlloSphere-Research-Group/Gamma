cmake_minimum_required (VERSION 2.8)
project (Gamma)

# Gamma Library
set(GAMMA_SRC src/arr.cpp
  src/AudioIO.cpp
  src/Conversion.cpp
  src/DFT.cpp
  src/FFT_fftpack.cpp
  src/fftpack++1.cpp
  src/fftpack++2.cpp
  src/File.cpp
  src/Print.cpp
  src/scl.cpp
  src/Recorder.cpp
  src/Scheduler.cpp
  src/SoundFile.cpp
  src/Sync.cpp)

include_directories(../Gamma/)
add_library(Gamma ${GAMMA_SRC})

set(LIBRARY_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/build/lib)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/build/bin)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")

# TODO: install targets
# TODO: release/debug builds
# TODO: allow choosing static or dynamic libraries
# TODO: allow choosing architecture 64 vs. 32 bits on 64 bit platforms (e.g. OS X)
# TODO make examples optional
# TODO make unit tests optional

# Dependencies

find_package(Portaudio REQUIRED)
message("-- Portaudio headers: ${PORTAUDIO_INCLUDE_DIRS}   ${PORTAUDIO_DEFINITIONS}")
find_package(LibSndFile REQUIRED)
message("-- libsndfile headers: ${SNDFILE_INCLUDE_DIRS}")

get_filename_component(PORTAUDIO_LIB_DIR ${PORTAUDIO_LIBRARIES} PATH)
get_filename_component(SNDFILE_LIB_DIR ${SNDFILE_LIBRARIES} PATH)

include_directories(${PORTAUDIO_INCLUDE_DIRS} ${SNDFILE_INCLUDE_DIRS})
link_directories(${PORTAUDIO_LIB_DIR} ${SNDFILE_LIB_DIR})

if(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
# Needed by portaudio
   find_library(AUDIOUNIT_FM AudioUnit)
   find_library(COREAUDIO_FM CoreAudio)
   find_library(CORESERVICES_FM CoreServices)
   find_library(AUDIOTOOLBOX_FM AudioToolbox)
   set(PA_FRAMEWORKS ${AUDIOUNIT_FM} ${COREAUDIO_FM} ${CORESERVICES_FM} ${AUDIOTOOLBOX_FM})
endif(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")

target_link_libraries(Gamma portaudio sndfile ${PA_FRAMEWORKS})

# Examples
set(EXAMPLE_DIRS algorithmic curves effects filter function generator io spectral)
foreach(dir ${EXAMPLE_DIRS})
  file(GLOB EXAMPLE_FILES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} examples/${dir}/*.cpp)
  foreach(example ${EXAMPLE_FILES})
    get_filename_component(EXAMPLE_NAME ${example} NAME_WE) # Get name w/o extension
    add_executable(${dir}_${EXAMPLE_NAME} ${example})
    target_link_libraries(${dir}_${EXAMPLE_NAME} Gamma portaudio sndfile ${PA_FRAMEWORKS})
    if (apple)
      find_library(AUDIOUNIT_FM AudioUnit)
      target_link_libraries(${dir}_${EXAMPLE_NAME} ${AUDIOUNIT_FM})
    endif()
  endforeach(example)
endforeach(dir) 

# Unit tests
add_executable(unitTests tests/unitTests.cpp)
target_link_libraries(unitTests Gamma portaudio sndfile ${PA_FRAMEWORKS})
