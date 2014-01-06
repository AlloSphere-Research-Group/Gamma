cmake_minimum_required(VERSION 2.8)
project(Gamma)

message("==== Configuring Gamma.")
set(CMAKE_BUILD_TYPE Release)

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

target_link_libraries(Gamma 
  ${CMAKE_THREAD_LIBS_INIT}
  ${PORTAUDIO_LIBRARIES}
  ${SNDFILE_LIBRARIES}
  ${PA_FRAMEWORKS}
  ${CMAKE_THREAD_LIBS_INIT}
  )

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
install(FILES ${GAMMA_INSTALL_HEADERS} DESTINATION include/Gamma)

# Examples -----------------------------------------------

if(NOT NO_EXAMPLES)
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
endif(NOT NO_EXAMPLES)


# Projects -----------------------------------------------
if(NOT NO_PROJECTS AND NOT ALLOSYSTEM_BUILD)
# macro to get subdirectories
  MACRO(SUBDIRLIST result curdir)
    FILE(GLOB children RELATIVE ${curdir} ${curdir}/*)
    SET(dirlist "")
    FOREACH(child ${children})
      IF(IS_DIRECTORY ${curdir}/${child})
	SET(dirlist ${dirlist} ${child})
      ENDIF()
    ENDFOREACH()
    SET(${result} ${dirlist})
  ENDMACRO()

  subdirlist(PROJECT_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/projects)
  #include root projects directory
  list(APPEND PROJECT_DIRS ".")
  foreach(dir ${PROJECT_DIRS})
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/build/bin/projects/${dir})
    file(GLOB PROJECT_FILES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} projects/${dir}/*.cpp)
    if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/projects/${dir}/flags.txt)
      file(READ ${CMAKE_CURRENT_SOURCE_DIR}/projects/${dir}/flags.txt EXTRA_COMPILER_FLAGS)
      message("WARNING: Using additional flags from /projects/${dir}/flags.txt: " ${EXTRA_COMPILER_FLAGS})
    endif()
    # Get a list of all files, later remove all .cpp and .h and you're left with the resources to copy
    file(GLOB PROJECT_RES_FILES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} projects/${dir}/*)

    foreach(project_src ${PROJECT_FILES})
      get_filename_component(GAMMA_PROJECT_NAME ${project_src} NAME_WE) # Get name w/o extension
      if (ALLOSYSTEM_BUILD)
	set(EXAMPLE_TARGET Gamma_projects_${dir}_${GAMMA_PROJECT_NAME})
      else(ALLOSYSTEM_BUILD)
	if(${dir} STREQUAL ".")
	  set(EXAMPLE_TARGET projects_${GAMMA_PROJECT_NAME})
	else()
	  set(EXAMPLE_TARGET projects_${dir}_${GAMMA_PROJECT_NAME})
	endif()
      endif(ALLOSYSTEM_BUILD)

      add_executable(${EXAMPLE_TARGET} ${project_src})
      message("Included project: ${EXAMPLE_TARGET}")
      target_link_libraries(${EXAMPLE_TARGET} Gamma ${PORTAUDIO_LIBRARIES} ${SNDFILE_LIBRARIES} ${PA_FRAMEWORKS})
      list(REMOVE_ITEM PROJECT_RES_FILES ${project_src})
      add_custom_target("${EXAMPLE_TARGET}_run" 
	DEPENDS "${EXAMPLE_TARGET}" 
	COMMAND "${EXAMPLE_TARGET}"
	WORKING_DIRECTORY "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}"
	SOURCES ${example}
	COMMENT "Running: ${EXAMPLE_TARGET}")
    endforeach(project_src)

    foreach(FILENAME ${PROJECT_RES_FILES})
      set(SRC "${CMAKE_CURRENT_SOURCE_DIR}/${FILENAME}")
      set(DST "${CMAKE_SOURCE_DIR}/build/${FILENAME}")
      if(NOT (IS_DIRECTORY ${SRC}))
	configure_file(${SRC} ${DST} COPY_ONLY)
	message("Copied: " ${SRC})
      endif(NOT (IS_DIRECTORY ${SRC}))
    endforeach(FILENAME)
  endforeach(dir)
endif(NOT NO_PROJECTS AND NOT ALLOSYSTEM_BUILD)

# Unit tests
if(GAMMA_TEST)
  add_executable(unitTests tests/unitTests.cpp)
  target_link_libraries(unitTests Gamma portaudio sndfile ${PA_FRAMEWORKS})
endif(GAMMA_TEST)
