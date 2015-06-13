/*	Gamma - Generic processing library
	See COPYRIGHT file for authors and license information
	
	Example:		Tutorial / Reson_tutorial by Josh Dickinson, 2012
	Description:	Using Reson to filter pink noise. 
*/

#include "Gamma/SimpleGamma.h"

// Bring to the global namespace the simple Gamma functions with double presicion
using namespace gamd;
using namespace std;

Accum tmr(3.0);// Timer to change the filter's center frequency

//sound source
NoisePink src;

//Reson is a Two-pole resonator. 
Reson filter;

float fundamentalFrequency = 200.0;

int mode = 0;


void audioCB(AudioIOData& io){
    
	while(io()){
        if(tmr()){
            //good way to get random numbers in gamma.  Uniform distribution between two numbers
            int tempHarmonic = rnd::uni(1,6);
 
            //set the center frequency by calling freq(freqYouWant)
            filter.freq(tempHarmonic*fundamentalFrequency);
            
            float tempWidth = (tempHarmonic*fundamentalFrequency)/rnd::uni(2,100);
            
            //set the filter's width (related to Q) by calling width(widthYouWant).
            filter.width(tempWidth);
            
            //print the filter's center frequency and width
            cout<< "frequency: " << (tempHarmonic*fundamentalFrequency) << endl << "width: " << tempWidth << endl;
        }          
        
        //this is how we filter our source.  
		float s = filter(src())*2.0;
        
		io.out(0) = io.out(1) = s; 
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
