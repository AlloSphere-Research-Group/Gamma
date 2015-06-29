/*	Gamma - Generic processing library
	See COPYRIGHT file for authors and license information
	
	Example:		Tutorial / Noise_tutorial by Josh Dickinson, 2012
	Description:	Using NoiseWhite, NoisePink, and NoiseBrown as signal generators
*/

#include <iostream>				// for printing

#include "Gamma/SimpleGamma.h"
using namespace gamd;
using namespace std;

Accum tmr(0.5);// Timer to change the noise type

//White noise has a uniform power spectrum.
NoiseWhite mrWhite;

//Pink noise has a power spectrum of 1/f. In this implementation, it is produced by summing together 12 octaves of downsampled white noise.
NoisePink mrPink;

//Brownian noise has a power spectrum of 1/f^2. It is produced by integrating white (uniform) noise. The output value is clipped within a specified interval.
NoiseBrown mrBrown;

//you can also initialize any of these classes with a seed, but you don't have to
//NoiseWhite mrWhite(3);

int mode = 0;

void audioCB(AudioIOData& io){
    
	while(io()){
        if(tmr()){
            std::cout<< "current mode: " << mode << std::endl;
            mode = (mode+1)%3;
        }        
   
        float s = 0.0;
        switch(mode){
            case 0:
                s = mrWhite();
                break;
            case 1: 
                s = mrPink();
                break;
            case 2:
                s = mrBrown();
                break;
        }        
    
		io.out(0) = io.out(1) = s * 0.2; 
	}
}


int main()
{
	if (!startAudio(44100, audioCB)) {
		cout << "Error starting audio." << endl;
		return -1;
	}
	cout << "Press 'enter' to quit..." << endl;
	getchar();
	return 0;
}
