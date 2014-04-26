# Gamma
### Generic Synthesis C++ Library


1. About
===
Gamma is a cross-platform, C++ library for doing generic synthesis and 
filtering of signals. It contains helpful mathematical functions, 
types, such as vectors and complex numbers, an assortment of sequence 
generators, and many other objects for signal processing tasks. 
It is oriented towards real-time sound and graphics synthesis, but is 
equally useful for non-real-time tasks.


2. Compilation Instructions
===
The source code can either be built into a library or directly compiled from source into an application.
In the following, the base directory is where this README file is located.


2.1 Building a Library
---

Gamma uses Cmake to build. You should be able to build Gamma by running

	cmake .
	make

will build the library and the examples with automatically detected platform settings.
You will need to install dependencies detailed in section 2.3 below.

On OS X an Xcode project can be produced by running Cmake like this:

        cmake  .

(You may need to delete old CMakeCache.txt and the CMakeFiles directory manually or using the distclean script, see below.)

Gamma is built as a static library and the binaries are located in the build/ directory.

Some useful options to pass to cmake include:

 * `-DBUILD_EXAMPLES=1` : To generate projects with the examples as targets
 * `-DCMAKE_INSTALL_PREFIX:PATH=/usr` : To set install prefix

To generate an Xcode project where you can try out the Gamma examples,
you should run cmake like this:

        cmake . -DBUILD_EXAMPLES=1 -G Xcode

2.2 Compiling Direct From Source
---
Individual Gamma files can easily be added and compiled directly from source into an existing project.
This way there is no need to build Gamma before using, but it may require
adding to your project a set of files as some Gamma files depend on others.

Make sure to pass in the following flags to the compiler:

	-D__STDC_CONSTANT_MACROS
	-finline-functions (or -O3)
	-fpeel-loops


2.3 Dependencies
----------------------------------------
Gamma depends on PortAudio v19 and libsndfile for performing audio and sound file i/o, respectively.
They are required only if using certain Gamma classes.
PortAudio is required ONLY if you are using AudioIO (AudioIO.h).
Libsndfile is required ONLY for SoundFile i/o and playback (SoundFile.h, Player.h).

You will also need Cmake to build Gamma. 

You can get these through standard package managers on Linux, or through MacPorts or HomeBrew on OS X.

2.4 Build issues
---

If you are having build issues (e.g. you want to use a Makefile but have used the Xcode generator, and cmake is giving you grief), running the `distclean` script will remove all Cmake cache and temporary files:

	./distclean
	
3. Easy building and running
===

The simplest way to run Gamma applications is using the included run script like this:

        ./run.sh path/to/source.cpp

You can also build all the cpp files in a particular directory into a single application passing a directory name:

        ./run.sh path/to/dir

Running this script will build Gamma and the application, and will run it.

4. Using the template
===
To build larger Gamma applications:

1. Copy and rename the template/ directory
2. Add source files to the CMakeLists.txt file there
3. Make sure you have Gamma installed in your system or that it is inside the project directory along side it and that it has been built.
4. Run cmake with the desired options inside the new project directory and build.
 
