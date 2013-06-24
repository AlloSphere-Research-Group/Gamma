# Gamma
### Generic Synthesis C++ Library


1. About
========================================
Gamma is a cross-platform, C++ library for doing generic synthesis and 
filtering of signals. It contains helpful mathematical functions, 
types, such as vectors and complex numbers, an assortment of sequence 
generators, and many other objects for signal processing tasks. 
It is oriented towards real-time sound and graphics synthesis, but is 
equally useful for non-real-time tasks.



2. Compilation Instructions
========================================
The source code can either be built into a library or directly compiled from source into an application. In the following, the base directory is where this README file is located.



2.1 Building a Library
----------------------------------------

Gamma uses Cmake to build. You should be able to build Gamma by running

	cmake .
	make

will build the library and the examples with automatically detected platform settings. You will need to install dependencies detailed in section 2.3 below.

On OS X an Xcode project can be produced by running Cmake like this:

	cmake -G Xcode .

(You may need to delete old CMakeCache.txt and the CMakeFiles directory manually)

Binaries are located in the directory ./build .

2.2 Compiling Direct From Source
----------------------------------------
Gamma can easily be compiled directly from source into an existing project.

Make sure to pass in the following flags to the compiler:

	-D__STDC_CONSTANT_MACROS
	-finline-functions (or -O3)
	-fpeel-loops



2.3 Dependencies
----------------------------------------
Gamma depends on PortAudio v19 and libsndfile for performing audio and sound file i/o, respectively. They are required only if using certain Gamma classes. PortAudio is required ONLY if you are using AudioIO (AudioIO.h). libsndfile is required ONLY for SoundFile i/o and playback (SoundFile.h, Player.h).

You will also need Cmake to build Gamma. 

You can get these through standard package managers on Linux, or through MacPorts or HomeBrew on OS X.

