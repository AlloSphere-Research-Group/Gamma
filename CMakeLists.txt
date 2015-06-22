cmake_minimum_required(VERSION 2.8)
project(Gamma)

message(STATUS "==== Configuring Gamma.")

if(NOT DEFINED BUILD_ROOT_DIR)
  set(BUILD_ROOT_DIR "${CMAKE_SOURCE_DIR}")
endif(NOT DEFINED BUILD_ROOT_DIR)

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
  src/SoundFile.cpp

  src/fftpack++.h)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})
file(GLOB GAMMA_INSTALL_HEADERS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} Gamma/*.h)
# Copy headers to build directory (only if header has changed)
foreach(header ${GAMMA_INSTALL_HEADERS})
	execute_process(COMMAND ${CMAKE_COMMAND} -E
					copy_if_different "${header}" "${BUILD_ROOT_DIR}/build/include/${header}"
					WORKING_DIRECTORY  "${CMAKE_CURRENT_SOURCE_DIR}")
endforeach()


add_library(Gamma ${GAMMA_SRC} ${GAMMA_INSTALL_HEADERS})
set(LIBRARY_OUTPUT_PATH "${BUILD_ROOT_DIR}/build/lib")
set(EXECUTABLE_OUTPUT_PATH "${BUILD_ROOT_DIR}/build/bin")
list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake_modules")

# TODO: allow choosing static or dynamic libraries

if(USE_CPP_11)
  message(STATUS "Using c++11..")
  if(USE_LIB_CPP)
    message(STATUS "Using libc++..")
    set(CMAKE_CXX_FLAGS "-std=c++11 -stdlib=libc++")
  else(USE_LIB_CPP)
    set(CMAKE_CXX_FLAGS "-std=c++11")
  endif(USE_LIB_CPP)
endif(USE_CPP_11)

# Dependencies

find_package(Portaudio REQUIRED)
find_package(LibSndFile REQUIRED)
find_package(Threads REQUIRED)
message(STATUS "Portaudio: ${PORTAUDIO_LIBRARY}  ${PORTAUDIO_DEFINITIONS}")
message(STATUS "libsndfile: ${SNDFILE_LIBRARY}")
message(STATUS "threading: ${CMAKE_THREAD_LIBS_INIT}")

include_directories(${PORTAUDIO_INCLUDE_DIRS} ${SNDFILE_INCLUDE_DIRS})

if(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
  # Needed by portaudio
  find_library(AUDIOUNIT_FM AudioUnit)
  find_library(COREAUDIO_FM CoreAudio)
  find_library(CORESERVICES_FM CoreServices)
  find_library(AUDIOTOOLBOX_FM AudioToolbox)
  set(PA_FRAMEWORKS ${AUDIOUNIT_FM} ${COREAUDIO_FM} ${CORESERVICES_FM} ${AUDIOTOOLBOX_FM})
endif(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")

if(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
  set_target_properties(Gamma PROPERTIES COMPILE_FLAGS "-fPIC") #Needed to use Gamma with certain frameworks, e.g. JUCE
endif(${CMAKE_SYSTEM_NAME} MATCHES "Linux")

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

install(TARGETS Gamma DESTINATION "${CMAKE_INSTALL_PREFIX}/lib")
install(FILES ${GAMMA_INSTALL_HEADERS} DESTINATION "${CMAKE_INSTALL_PREFIX}/include")

# Run from path
if((GAMMA_BUILD_DIR) OR (GAMMA_BUILD_APP_FILE))
    set(BUILD_DIR ${GAMMA_BUILD_DIR})
    set(BUILD_APP_FILE ${GAMMA_BUILD_APP_FILE})
    include(${CMAKE_CURRENT_SOURCE_DIR}/cmake_modules/CMakeRunTargets.cmake)
    set(BUILD_EXAMPLES 0)
endif((GAMMA_BUILD_DIR) OR (GAMMA_BUILD_APP_FILE))

# Examples -----------------------------------------------

if(BUILD_EXAMPLES)
  set(EXAMPLE_DIRS algorithmic curves effects filter function generator io spectral synths)
  foreach(dir ${EXAMPLE_DIRS})
	set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/build/bin/examples/${dir}")
	file(GLOB EXAMPLE_FILES RELATIVE "${CMAKE_CURRENT_SOURCE_DIR} examples/${dir}/*.cpp")
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
enable_testing()

add_executable(gammaUnitTests tests/unitTests.cpp)
target_link_libraries(gammaUnitTests Gamma ${GAMMA_LINK_LIBRARIES})
add_test(NAME gammaUnitTests
	 COMMAND $<TARGET_FILE:gammaUnitTests> ${TEST_ARGS})
