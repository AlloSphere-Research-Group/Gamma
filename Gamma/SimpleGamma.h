/*	Gamma - Generic processing library
        See COPYRIGHT file for authors and license information

        File Description: Header to present Gamma classes with minimum templating
*/

#ifndef SIMPLEGAMMA_H
#define SIMPLEGAMMA_H

#ifdef GAMMA_GAMMA_H_INC
#error "Gamma.h shouldn't be included if you plan to use SimpleGamma.h"
#endif

#define GAMMA_H_INC_ALL
#include "Gamma.h"

/// \defgroup SimpleGamma


/// \ingroup SimpleGamma
namespace gam {

/// Sets the global sample rate for all Gamma generators and processors that
/// are not linked explicitly to a different Domain.
static void setGlobalSampleRate(double sampleRate)
{
    gam::Domain::master().spu(sampleRate);
}

}

namespace gamf {


// From Gamma/Oscillator.h ----------------------------------------------
/// \ingroup SimpleGamma
class Accum: public gam::Accum<> {
public:
    Accum(float frq=0.0, float phs=0.0) : gam::Accum<>(frq, phs) {}
};

//typedef gam::Sweep<> Sweep;
//typedef gam::AccumPhase<> AccumPhase;
//typedef gam::CSine<> CSine;

/// \ingroup SimpleGamma
class Osc: public gam::Osc<float> {
public:
    Osc(float frq=440, float phs=0, uint32_t size=512) : gam::Osc<float>(frq, phs, size) {}
    Osc(float frq, float phs, ArrayPow2<float>& src) : gam::Osc<float>(frq, phs, src) {}
};

/// \ingroup SimpleGamma
class Sine: public gam::Sine<> {
public:
    Sine(float frq = 440.0, float phs = 0.0) : gam::Sine<float>(frq, phs) {}
};
//typedef gam::SineR<> SineR;
//typedef gam::SineRs<> SineRs;
//typedef gam::SineD<> SineD;
//typedef gam::SineDs<> SineDs;
//typedef gam::LFO<> LFO;
//typedef gam::Buzz<> Buzz;
//typedef gam::Impulse<> Impulse;
//typedef gam::Saw<> Saw;
//typedef gam::Square<> Square;
//typedef gam::DSF<> DSF;
//typedef gam::ImpulseFast ImpulseFast;

// From Gamma/Filter.h --------------------------------------

class LowPass: public gam::Biquad<float, float> {
public:
    LowPass(float frq = 1000.0, float res = 1.0) :
        Biquad<float, float>(frq, res, gam::LOW_PASS) {}

};

/// \ingroup SimpleGamma
typedef gam::AudioDevice AudioDevice;

/// \ingroup SimpleGamma
typedef gam::AudioIO AudioIO;

/// \ingroup SimpleGamma
typedef gam::AudioIOData AudioIOData;

namespace rnd {
/// \ingroup SimpleGamma
using gam::rnd::pick;
}
/// \ingroup SimpleGamma
typedef gam::Domain Domain;
}


#endif // SIMPLEGAMMA_H

