/*	Gamma - Generic processing library
	See COPYRIGHT file for authors and license information
	
	Example:		Tutorial / Reson_tutorial by Josh Dickinson, 2012
	Description:	Using Reson to filter pink noise. 
*/

#include "Gamma/SimpleGamma.h"

using namespace gamd;
using namespace std;

Accum tmr(3.0); // Frequency at which to print

// STFT (Short-Time Fourier Transform) does sliding DFT with hop and overlap.
// Using MAG_FREQ does phase vocoder, providing frequency from the phase
// value in the FFT.
STFT stft(1024, 256, 1024, HANN, MAG_FREQ);

float frequency, amp;

void audioCB(AudioIOData& io){
    
	while(io()){
		if(stft(io.in(0))){ // When there is a new analysis window
			// Gets an array of the analysis bins
			Complex<float> * b = stft.bins();
			unsigned int numBins = stft.numBins();
			float maxAmp = 0.0;
			int binIndex = -1;
			// Go through all the FFT bins to find the one with highest magnitude
			for (unsigned int i = 0; i < numBins; i++) {
				if (b[i][0] > maxAmp) {
					maxAmp = b[i][0];
					binIndex = i;
				}
			}
			// Store the amplitude and frequency for the highest bin
			amp = b[binIndex][0];
			frequency = b[binIndex][1];
		}

		if(tmr()) { // At the selected rate, print the current values fo amp and frequency
			cout << "Amp: " << amp << " Freq: " << frequency << endl;
		}
        
		io.out(0) = io.out(1) = 0.0;
	}
}

int main(){
	if (!startAudio(44100, audioCB)) {
		cout << "Error starting audio." << endl;
		return -1;
	}
	cout << "Press 'enter' to quit..." << endl;
	getchar();
	return 0;
}
