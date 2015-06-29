/*	Gamma - Generic processing library
	See COPYRIGHT file for authors and license information
	
	Example:		Tutorial / Sine_tutorial by Josh Dickinson, 2012
	Description:	Using Sine as a signal generator
*/

#include <iostream>				// for printing

#include "Gamma/SimpleGamma.h"

// Bring to the global namespace the simple Gamma functions with double presicion
using namespace gamd;
using namespace std;

Accum tmr(3.0);// Timer to change the frequency

//Sine uses a polynomial approximation to compute sine values instead of reading from a table.  This saves memory but costs a little bit more CPU.  Lance says that the performance is almost as good as reading from a table, so you probably don't need to worry unless you're using a whole bunch of them simultaneously.
Sine src;

float fundamentalFrequency = 200.0;

int mode = 0;

// Audio processing takes place in a single function, the audio callback
void audioCB(AudioIOData& io){

	while(io()){
		if(tmr()){ // the accumulator returns true when it "triggers"
			//good way to get random numbers in gamma.  Uniform distribution between two numbers
			int tempHarmonic = rnd::uni(1,6);

			//set the frequency by calling freq(freqYouWant)
			src.freq(tempHarmonic*fundamentalFrequency);

			// get the frequency by calling freq()
			cout<< "harmonic: " << tempHarmonic << endl << "frequency: " << src.freq() << endl;
			// You should avoid printing or opening/reading files within the audio
			// callback as this makes audio dropouts likely. But it might
			// work OK for simpler algorithms.
		}

		float s = src()*0.2;
		io.out(0) = io.out(1) = s;
	}
}

int main(){
	// The startAudio() function is a simple convenience function to start
	// audio rendering. You must give it the sample rate and the name of your
	// callback function
	if (!startAudio(44100, audioCB)) {
		cout << "Error starting audio." << endl;
		return -1;
	}
	// If audio was opened correctly then wait for user input. The audio
	// callback will be called continuouslt behind the scenes in a separate
	// thread.
	cout << "Press 'enter' to quit..." << endl;
	getchar();
	return 0;
}
