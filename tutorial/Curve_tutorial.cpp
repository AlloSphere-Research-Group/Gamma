/*	Gamma - Generic processing library
	See COPYRIGHT file for authors and license information
	
	Example:		Generator / curve
	Description:	Curve used to interpolate between two numbers
 */

#include "Gamma/SimpleGamma.h"
using namespace gamd;

//length, curve, end, start
Curve curveShape(10, -1, 10.0, -10.0);
Curve env(44100,10.0, 1.0, 0.0);

// Noise source
NoiseWhite noise;

void audioCB(AudioIOData& io){
    double e;
    while(io()){
        //every frame we check if the envelope is done to reset it
        if(env.done()){
            // every time we need to start the envelope again we set a different
            // value for the shape that we get from a separate Curve object
            double shape = curveShape();
            env.set(44100, shape, 1.0, 0.0);
            std::cout<< "New shape value: " << shape <<std::endl;
            if (curveShape.done()) {
                curveShape.set(10, -5, 10.0, -10.0);
            }
        }
        e = env();

        //multiply constant white noise times our curve every frame
        float s = e * noise();

        //left channel = right channel = s
        io.out(0) = io.out(1) = s;
    }
    //print the value of the curve every block
//    std::cout<< "current curve value: " << e <<std::endl;
}

int main(){
    AudioIO io(256, 44100, audioCB, NULL, 2);
    setGlobalSampleRate(io.framesPerSecond());
    io.start();
    printf("Press 'enter' to quit...\n"); getchar();
    return 0;
}

