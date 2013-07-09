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

include_directories(${CMAKE_CURRENT_SOURCE_DIR})
add_library(Gamma ${GAMMA_SRC})
set(LIBRARY_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/build/lib)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/build/bin)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake_modules")

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

include_directories(${PORTAUDIO_INCLUDE_DIRS} ${SNDFILE_INCLUDE_DIRS})
#include_directories(${PORTAUDIO_INCLUDE_DIRS} ${SNDFILE_INCLUDE_DIRS})

if(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
# Needed by portaudio
   find_library(AUDIOUNIT_FM AudioUnit)
   find_library(COREAUDIO_FM CoreAudio)
   find_library(CORESERVICES_FM CoreServices)
   find_library(AUDIOTOOLBOX_FM AudioToolbox)
   set(PA_FRAMEWORKS ${AUDIOUNIT_FM} ${COREAUDIO_FM} ${CORESERVICES_FM} ${AUDIOTOOLBOX_FM})
endif(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")

target_link_libraries(Gamma ${PORTAUDIO_LIBRARIES} ${SNDFILE_LIBRARIES} ${PA_FRAMEWORKS})

# Install target
install (TARGETS Gamma DESTINATION lib)
set (GAMMA_INSTALL_HEADERS Gamma/Access.h      Gamma/File.h         Gamma/rnd.h
Gamma/Allocator.h   Gamma/Filter.h       Gamma/SamplePlayer.h
Gamma/arr.h         Gamma/FormantData.h  Gamma/Scheduler.h
Gamma/AudioIO.h     Gamma/Gamma.h        Gamma/scl.h
Gamma/Config.h      Gamma/gen.h          Gamma/SoundFile.h
Gamma/Constants.h   Gamma/ipl.h          Gamma/Strategy.h
Gamma/Containers.h  Gamma/mem.h          Gamma/Sync.h
Gamma/Conversion.h  Gamma/Node.h         Gamma/tbl.h
Gamma/Delay.h       Gamma/Noise.h        Gamma/Thread.h
Gamma/DFT.h         Gamma/Oscillator.h   Gamma/Timer.h
Gamma/Effects.h     Gamma/Print.h        Gamma/TransferFunc.h
Gamma/Envelope.h    Gamma/pstdint.h      Gamma/Types.h
Gamma/FFT.h         Gamma/Recorder.h     Gamma/UnitMaps.h)
install (FILES ${GAMMA_INSTALL_HEADERS} DESTINATION include/Gamma)

# Examples
set(EXAMPLE_DIRS algorithmic curves effects filter function generator io spectral)
foreach(dir ${EXAMPLE_DIRS})
  file(GLOB EXAMPLE_FILES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} examples/${dir}/*.cpp)
  foreach(example ${EXAMPLE_FILES})
    get_filename_component(EXAMPLE_NAME ${example} NAME_WE) # Get name w/o extension
    add_executable(${dir}_${EXAMPLE_NAME} ${example})
    target_link_libraries(${dir}_${EXAMPLE_NAME} Gamma ${PORTAUDIO_LIBRARIES} ${SNDFILE_LIBRARIES} ${PA_FRAMEWORKS})
  endforeach(example)
endforeach(dir) 

# Unit tests
# TODO make optional
add_executable(unitTests tests/unitTests.cpp)
target_link_libraries(unitTests Gamma portaudio sndfile ${PA_FRAMEWORKS})
