cmake_minimum_required(VERSION 2.8)
project(Gamma)

message("==== Configuring Gamma.")
set(CMAKE_BUILD_TYPE Release)

string(REGEX MATCH ".*\\*.*" match "${CMAKE_CURRENT_SOURCE_DIR}")

IF(NOT ${match} STREQUAL "")
  message(FATAL_ERROR "Error: Please remove '*' from path!" )
ENDIF()

# Gamma Library
set(GAMMA_SRC src/arr.cpp
  src/AudioIO.cpp
  src/Conversion.cpp
  src/DFT.cpp
  src/Domain.cpp
  src/FFT_fftpack.cpp
  src/fftpack++1.cpp
  src/fftpack++2.cpp
  src/File.cpp
  src/Print.cpp
  src/scl.cpp
  src/Recorder.cpp
  src/Scheduler.cpp
  src/SoundFile.cpp)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})
add_library(Gamma ${GAMMA_SRC})
set(LIBRARY_OUTPUT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/build/lib)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/build/bin)
list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake_modules")

# TODO: allow choosing static or dynamic libraries

if(USE_CPP_11)
  message("Using c++11..")
  if(USE_LIB_CPP)
    message("Using libc++..")
    set(CMAKE_CXX_FLAGS "-std=c++11 -stdlib=libc++")
  else(USE_LIB_CPP)
    set(CMAKE_CXX_FLAGS "-std=c++11")
  endif(USE_LIB_CPP)
endif(USE_CPP_11)

# Dependencies

find_package(Portaudio REQUIRED)
find_package(LibSndFile REQUIRED)
find_package(Threads REQUIRED)
message("-- Portaudio: ${PORTAUDIO_LIBRARY}  ${PORTAUDIO_DEFINITIONS}")
message("-- libsndfile: ${SNDFILE_LIBRARY}")
message("-- threading: ${CMAKE_THREAD_LIBS_INIT}")

include_directories(${PORTAUDIO_INCLUDE_DIRS} ${SNDFILE_INCLUDE_DIRS})

if(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
  # Needed by portaudio
  find_library(AUDIOUNIT_FM AudioUnit)
  find_library(COREAUDIO_FM CoreAudio)
  find_library(CORESERVICES_FM CoreServices)
  find_library(AUDIOTOOLBOX_FM AudioToolbox)
  set(PA_FRAMEWORKS ${AUDIOUNIT_FM} ${COREAUDIO_FM} ${CORESERVICES_FM} ${AUDIOTOOLBOX_FM})
endif(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")

set(GAMMA_LINK_LIBRARIES ${CMAKE_THREAD_LIBS_INIT}
  ${PORTAUDIO_LIBRARIES}
  ${SNDFILE_LIBRARIES}
  ${PA_FRAMEWORKS}
  ${CMAKE_THREAD_LIBS_INIT})

target_link_libraries(Gamma  ${GAMMA_LINK_LIBRARIES})

set_target_properties(Gamma
  PROPERTIES
  GAMMA_LINK_LIBRARIES "${GAMMA_LINK_LIBRARIES}"
  )
set_target_properties(Gamma
  PROPERTIES
  GAMMA_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}
  )

# Install target

install(TARGETS Gamma DESTINATION lib)
set(GAMMA_INSTALL_HEADERS Gamma/Access.h      Gamma/File.h         Gamma/rnd.h
  Gamma/Allocator.h   Gamma/Filter.h       Gamma/SamplePlayer.h
  Gamma/arr.h         Gamma/FormantData.h  Gamma/Scheduler.h
  Gamma/AudioIO.h     Gamma/Gamma.h        Gamma/scl.h
  Gamma/Config.h      Gamma/gen.h          Gamma/SoundFile.h
  Gamma/Constants.h   Gamma/ipl.h          Gamma/Strategy.h
  Gamma/Containers.h  Gamma/mem.h          Gamma/Domain.h
  Gamma/Conversion.h  Gamma/Node.h         Gamma/tbl.h
  Gamma/Delay.h       Gamma/Noise.h        Gamma/Thread.h
  Gamma/DFT.h         Gamma/Oscillator.h   Gamma/Timer.h
  Gamma/Effects.h     Gamma/Print.h        Gamma/TransferFunc.h
  Gamma/Envelope.h    Gamma/pstdint.h      Gamma/Types.h
  Gamma/FFT.h         Gamma/Recorder.h     Gamma/UnitMaps.h Gamma/Sync.h)
install(FILES ${GAMMA_INSTALL_HEADERS} DESTINATION include/Gamma)

# Run from path
if(${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_CURRENT_SOURCE_DIR}) # use only if cmake was run from the Gamma directory
    if((BUILD_DIR) OR (BUILD_APP_FILE))
      include(cmake_modules/CMakeRunTargets.cmake)
      set(BUILD_EXAMPLES 0)
    endif((BUILD_DIR) OR (BUILD_APP_FILE))
endif(${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_CURRENT_SOURCE_DIR})

# Examples -----------------------------------------------

if(BUILD_EXAMPLES)
  set(EXAMPLE_DIRS algorithmic curves effects filter function generator io spectral synths)
  foreach(dir ${EXAMPLE_DIRS})
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/build/bin/examples/${dir})
    file(GLOB EXAMPLE_FILES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} examples/${dir}/*.cpp)
    foreach(example ${EXAMPLE_FILES})
      get_filename_component(EXAMPLE_NAME ${example} NAME_WE) # Get name w/o extension
      if (ALLOSYSTEM_BUILD)
	set(EXAMPLE_TARGET gamma_examples_${dir}_${EXAMPLE_NAME})
      else(ALLOSYSTEM_BUILD)
	set(EXAMPLE_TARGET examples_${dir}_${EXAMPLE_NAME})
      endif(ALLOSYSTEM_BUILD)
      add_executable(${EXAMPLE_TARGET} ${example})
      target_link_libraries("${EXAMPLE_TARGET}" Gamma ${PORTAUDIO_LIBRARIES} ${SNDFILE_LIBRARIES} ${PA_FRAMEWORKS})
      add_custom_target("${EXAMPLE_TARGET}_run"
	COMMAND "${EXAMPLE_TARGET}"
	DEPENDS "${EXAMPLE_TARGET}"
	WORKING_DIRECTORY "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}"
	SOURCES ${example}
	COMMENT "Running: ${EXAMPLE_TARGET}")
    endforeach(example)
  endforeach(dir) 
endif(BUILD_EXAMPLES)


# Unit tests
if(GAMMA_TEST)
  add_executable(unitTests tests/unitTests.cpp)
  target_link_libraries(unitTests Gamma portaudio sndfile ${PA_FRAMEWORKS})
endif(GAMMA_TEST)
