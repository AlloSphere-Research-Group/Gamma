/*	Gamma - Generic processing library
	See COPYRIGHT file for authors and license information
	
	Example:		Tutorial / Reson_tutorial by Josh Dickinson, 2012
	Description:	Using Reson to filter pink noise. 
*/

#include "Gamma/SimpleGamma.h"

// Bring to the global namespace the simple Gamma functions with double presicion
using namespace gamd;
using namespace std;

Accum tmr(5); // Frequency of RMS printing

double maximumSample;

void audioCB(AudioIOData& io){
    
	while(io()){
		if(tmr()){
			//print the maximum sample for the measurement period
			cout<< "Maximum: " << maximumSample << endl;
			maximumSample = 0;
        }          
		float in_0 = io.in(0);
		float in_1 = io.in(1);
		if (maximumSample < fabs(in_0)) { maximumSample = in_0; }
		if (maximumSample < fabs(in_1)) { maximumSample = in_1; }
        
		io.out(0) = io.out(1) = 0.0;  // It's a good idea to clear the output buffer
	}
}

int main(){
	maximumSample = 0;
	if (!startAudio(44100, audioCB)) {
		cout << "Error starting audio." << endl;
		return -1;
	}
	cout << "Press 'enter' to quit..." << endl;
	getchar();
	return 0;
}
