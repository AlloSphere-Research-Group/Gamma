/*	Gamma - Generic processing library
	See COPYRIGHT file for authors and license information
	
        Example:	Tutorial / Delay1_tutorial by Josh Dickinson, 2012
	Description:	Using Delay1 - a one element (sample) delay line. 
*/

#include <iostream>

#include "Gamma/SimpleGamma.h"
using namespace gamd;

using namespace std;

Accum tmr(0.5);// Timer to change the filter's center frequency

double fundamentalFrequency = 200.0;

DSF src(fundamentalFrequency, 1, 0.5, 8);

Delay delay(1.1);

Curve env(44100,-10.0, 0.2, 0.0);

void audioCB(AudioIOData& io){
    double previousSample = 0.0;
    while(io()){
        if (env.done()) {
            env.set(44100,-10.0, 0.2, 0.0);
        }
        
        //get the next sample from our generator each frame
        double s = src() * env();
        double out = s + (delay(s + previousSample) * 0.8);
        
        io.out(0) = io.out(1) = out;
        previousSample = out;
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
