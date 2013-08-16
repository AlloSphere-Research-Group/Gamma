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
The source code can either be built into a library or directly compiled from source into an application. In the following, the base directory is where this README file is located.


2.1 Building a Library
---

Gamma uses Cmake to build. You should be able to build Gamma by running

	cmake .
	make

will build the library and the examples with automatically detected platform settings. You will need to install dependencies detailed in section 2.3 below.

On OS X an Xcode project can be produced by running Cmake like this:

	cmake -G Xcode .

(You may need to delete old CMakeCache.txt and the CMakeFiles directory manually or using the distclean script, see below.)

Gamma is built as a static library and the binaries are located in the build/ directory.

Some useful options to pass to cmake include:

 * `-DNO_EXAMPLES=1` : To avoid building examples
 * `-DNO_PROJECTS=1` : TO avois building projects
 * `-DNO_AUTOCOMPLETE=1` : To avoid creating empty files for target autocomplete (OS X only)
 * `-DCMAKE_INSTALL_PREFIX:PATH=/usr` : To set install prefix

You should run cmake like this:

	cmake . -DNO_EXAMPLES=1 -DNO_PROJECTS=1 -DNO_AUTOCOMPLETE=1

2.2 Compiling Direct From Source
---
Individual Gamma files can easily be added and compiled directly from source into an existing project.

Make sure to pass in the following flags to the compiler:

	-D__STDC_CONSTANT_MACROS
	-finline-functions (or -O3)
	-fpeel-loops


2.3 Dependencies
----------------------------------------
Gamma depends on PortAudio v19 and libsndfile for performing audio and sound file i/o, respectively. They are required only if using certain Gamma classes. PortAudio is required ONLY if you are using AudioIO (AudioIO.h). libsndfile is required ONLY for SoundFile i/o and playback (SoundFile.h, Player.h).

You will also need Cmake to build Gamma. 

You can get these through standard package managers on Linux, or through MacPorts or HomeBrew on OS X.

2.4 Build issues
---

If you are having build issues (e.g. you want to use a Makefile but have used the Xcode generator, and cmake is giving you grief), running the `distclean` script will remove all Cmake cache and temporary files:

	./distclean
	
3. Examples and projects
===

Examples and projects are built by default when running make, and a special target to run them is also created. To run an example, you should use:

	make examples_effects_echo

This will build and run the example code in examples/effects/echo.cpp.

Any files and resources placed in the `projects/` folder will generate build targets. This is a quick and easy way to use Gamma without having to worry about the build system and the dependencies. Any files that are not source files will be copied to the build directories, so relative paths can be used in code to reference resource files, e.g. input audio files. This mechanism assumes that projects consist of a single .cpp source file. For larger projects, you should build upon the project contained in the `template/` directory.

For examples, for the file `projects/sonfication/resonance.cpp` a target called `projects_sonification_resonance` will be added, and you can build and run your project with:

	make projects_sonification_resonance

If you have a file called `audio.wav` in the `projects/sonfication/` directory, it will be copied to the

Projects are configured when running Cmake, so you will need to run cmake if you have added new code or resource files. If you have just modified existing files, running make is enough.

4. Using the template
===
To build larger Gamma applications:

1. Copy and rename the template/ directory
2. Add source files to the CMakeLists.txt file there
3. Make sure you have Gamma installed in your system or that it is inside the project directory along side it and that it has been built.
4. Run cmake with the desired options inside the new project directory and build.
 
