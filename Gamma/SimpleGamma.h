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
namespace gamf {
// From Gamma/Oscillator.h
/// \ingroup SimpleGamma
class Accum: public gam::Accum<> {
public:
    Accum(float frq=0, float phs=0) : gam::Accum<>(frq, phs) {}
};

//typedef gam::Sweep<> Sweep;
//typedef gam::AccumPhase<> AccumPhase;
//typedef gam::CSine<> CSine;

/// \ingroup SimpleGamma
class Osc: public gam::Osc<> {
public:
    Osc(float frq=440, float phs=0, uint32_t size=512) : gam::Osc<>(frq, phs, size) {}
    Osc(float frq, float phs, ArrayPow2<float>& src) : gam::Osc<>(frq, phs, src) {}
};

/// \ingroup SimpleGamma
class Sine: public gam::Sine<> {
public:
    Sine(gam::real frq = 440.0, gam::real phs = 0.0) : gam::Sine<>(frq, phs) {}
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

