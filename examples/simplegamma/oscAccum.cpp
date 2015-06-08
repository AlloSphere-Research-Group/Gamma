/*	Gamma - Generic processing library
	See COPYRIGHT file for authors and license information
	
	Example:		Generator / Oscillator / Accumulator
	Description:	Using an accumulator as a timer to trigger impulses
*/

#include <iostream>

#include "Gamma/SimpleGamma.h"

using namespace gamf;

Accum tmr(10);			// timer to switch between LFO types
Sine sinosc;

void audioCB(AudioIOData& io){

	while(io()){
	
		float s = 0;
	
		if(tmr()){
			tmr.period(rnd::pick(0.4, 0.2));
			s = 0.2;
		}
		
		io.out(0) = s;
		io.out(1) = sinosc() * 0.1;
	}
}


int main(int argc, char* argv[]){\
	AudioIO io(256, 44100, audioCB, NULL, 2);
	Domain::master().spu(io.framesPerSecond());
	io.start();
	std::cout << "Press 'enter' to quit...\n" << std::endl;
	std::getwchar();
}
