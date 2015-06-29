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

#ifndef SIMPLEGAMMA_FLOATS
#define NAMESPACE_NAME gamd
#define REAL double
#else
#define NAMESPACE_NAME gamf
#define REAL float
#endif

namespace NAMESPACE_NAME {

/// Sets the global sample rate for all Gamma generators and processors that
/// are not linked explicitly to a different Domain.
static void setGlobalSampleRate(double sampleRate)
{
    gam::sampleRate(sampleRate);
}

/// Simple function to start running an audio thread communicating in real time
/// to an audio card
static bool startAudio(double sampleRate = 44100,
					   void (* callback)(gam::AudioIOData &) = 0,
					   void * userData = 0,
					   int framesPerBuf=64,
					   int outChannels = 2,
					   int inChannels = 2)
{
	static gam::AudioIO io_(framesPerBuf, sampleRate, callback, userData, outChannels, inChannels);
	setGlobalSampleRate(io_.framesPerSecond());
	return io_.start();
}

// From Gamma/Access.h --------------------------------------------
/// \ingroup SimpleGamma
/// \copydoc gam::IndexMap
class IndexMap: public gam::IndexMap<REAL> {
public:
	IndexMap(int idxMax=1, const REAL& posMax=1.0)
		: gam::IndexMap<REAL>(idxMax, posMax) {}
};

class Slice;

// From Gamma/Allocator.h ----------------------------------------------------

// class Allocator

// From Gamma/Analysis.h ------------------------------------------------------

/// \ingroup SimpleGamma
/// \copydoc gam::EnvFollow
class EnvFollow: public gam::EnvFollow<REAL,REAL> {
public:
	EnvFollow(REAL freq=10)
		: gam::EnvFollow<REAL,REAL>(freq) {}
};

/// \ingroup SimpleGamma
/// \copydoc gam::Threshold
class Threshold: public gam::Threshold<REAL> {
public:
	Threshold(REAL thresh, REAL freq=10)
		: gam::Threshold<REAL>(thresh, freq) {}
};

/// \ingroup SimpleGamma
/// \copydoc gam::ZeroCross
class ZeroCross: public gam::ZeroCross<REAL> {
public:
	ZeroCross(REAL prev = 0.0)
		: gam::ZeroCross<REAL>(prev) {}
};

// class ZeroCrossingRate

// From Gamma/AudioIO.h -------------------------------------------------

/// \ingroup SimpleGamma
typedef gam::AudioDevice AudioDevice;

/// \ingroup SimpleGamma
typedef gam::AudioIO AudioIO;

/// \ingroup SimpleGamma
typedef gam::AudioIOData AudioIOData;

//class AudioCallback;

// From Gamma/Config.h --------------------------------------------------

// No classes there

// From Gamma/Constants.h -----------------------------------------------

// No classes there

// From Gamma/Containers.h -----------------------------------------------

// class ArrayBase

/// \ingroup SimpleGamma
/// \copydoc gam::Array
class Array: public gam::Array<REAL> {
public:
    Array() : gam::Array<REAL>() {}
};

/// \ingroup SimpleGamma
/// \copydoc gam::ArrayPow2
class ArrayPow2: public gam::ArrayPow2<REAL> {
public:
    ArrayPow2() : gam::ArrayPow2<REAL>() {}
	explicit ArrayPow2(uint32_t size): gam::ArrayPow2<REAL>(size){}
	ArrayPow2(uint32_t size, const REAL& initial): gam::ArrayPow2<REAL>(size, initial){}
};

// From Gamma/Conversion.h ----------------------------------------------

//class Twiddle


// From Gamma/Delay.h -------------------------------------

/// \ingroup SimpleGamma
/// \copydoc gam::Delay
class Delay: public gam::Delay<REAL, gam::ipl::Cubic  > {
public:
	Delay(REAL delay) :
		gam::Delay<REAL, gam::ipl::Cubic >(delay) {}
	Delay(REAL maxDelay, REAL delay) :
		gam::Delay<REAL, gam::ipl::Cubic >(maxDelay, delay) {}
};

//class Multitap
//class DelayShift

/// \ingroup SimpleGamma
/// \copydoc gam::Delay1
class Delay1: public gam::Delay1<REAL> {
public:
	Delay1(REAL initialValue = 0.0) :
		gam::Delay1<REAL>(initialValue) {}
};

//class Delay2

/// \ingroup SimpleGamma
/// \copydoc gam::Comb
class Comb: public gam::Comb<REAL, gam::ipl::Cubic, REAL> {
public:
	Comb() :
		gam::Comb<REAL, gam::ipl::Cubic, REAL>() {}
	Comb(float delay, const REAL& ffd = 0.0, const REAL& fbk = 0.0) :
		gam::Comb<REAL, gam::ipl::Cubic, REAL>(delay, ffd, fbk) {}
	Comb(float maxDelay, float delay, const REAL& ffd = 0.0, const REAL& fbk = 0.0) :
		gam::Comb<REAL, gam::ipl::Cubic, REAL>(delay, ffd, fbk) {}
};

// From Gamma/DFT.h ----------------------------------------------
/// \ingroup SimpleGamma
/// \copydoc gam::SlidingWindow
class SlidingWindow: public gam::SlidingWindow<REAL> {
public:
	SlidingWindow(unsigned int winSize, unsigned int hopSize)
		: gam::SlidingWindow<REAL>(winSize, hopSize) {}
};

// Class DFTBase

using gam::SpectralType;
using gam::COMPLEX;	/**< Complex number */
using gam::MAG_PHASE;	/**< Magnitude and phase */
using gam::MAG_FREQ;	/**< Magnitude and frequency */

/// \ingroup SimpleGamma
/// \copydoc gam::DFT
class DFT: public gam::DFT {
public:
	DFT(unsigned int winSize, unsigned int padSize=0,
		SpectralType specType=COMPLEX,
		unsigned numAux=0)
		: gam::DFT(winSize, padSize, specType, numAux) {}
};

using gam::WindowType;
using gam::BARTLETT;			/**< Bartlett (Triangle) */
using gam::BLACKMAN;			/**< Blackman */
using gam::BLACKMAN_HARRIS;	/**< Blackman-Harris */
using gam::BLACKMAN_NUTTALL;   /**< Blackman-Nuttall */
using gam::FLATTOP;            /**< Flat-Top */
using gam::HAMMING;			/**< Hamming */
using gam::HANN;				/**< von Hann */
using gam::NUTTALL;            /**< Nuttall */
using gam::WELCH;				/**< Welch */
using gam::NYQUIST;			/**< Nyquist */
using gam::RECTANGLE;			/**< Rectangle (no window) */

/// \ingroup SimpleGamma
/// \copydoc gam::STFT
class STFT: public gam::STFT {
public:
	STFT(unsigned int winSize=1024, unsigned int hopSize=256, unsigned int padSize=0,
		 WindowType winType = RECTANGLE,
		 SpectralType specType = COMPLEX,
		 unsigned numAux=0)
		: gam::STFT(winSize, hopSize, padSize, winType, specType, numAux) {}
};

//class SlidingDFT;

// From Gamma/Effects.h ----------------------------------------

//class AM
//class Biquad3
//class Burst

/// \ingroup SimpleGamma
/// \copydoc gam::Chirp
class Chirp : public gam::Chirp<REAL> {
public:
	Chirp(REAL freq1=220, REAL freq2=0, REAL decay60=0.2)
		: gam::Chirp<REAL>(freq1, freq2, decay60) {}
};

//class ChebyN

/// \ingroup SimpleGamma
/// \copydoc gam::Chorus
class Chorus : public gam::Chorus<REAL> {
public:
	Chorus(float delay=0.0021, float depth=0.002, float freq=1, float ffd=0.9, float fbk=0.1)
		: gam::Chorus<REAL>(delay, depth, freq, ffd, fbk) {}
};

/// \ingroup SimpleGamma
/// \copydoc gam::FreqShift
class FreqShift : public gam::FreqShift<REAL> {
public:
	FreqShift(REAL shift = 1) : gam::FreqShift<REAL>(shift) {}
};

//class MonoSynth

/// \ingroup SimpleGamma
/// \copydoc gam::Pan
class Pan: public gam::Pan<REAL> {
public:
	Pan(REAL pos = 0) : gam::Pan<REAL>(pos) {}
};

/// \ingroup SimpleGamma
/// \copydoc gam::Pluck
class Pluck: public gam::Pluck {
public:
	Pluck(REAL freq=440, REAL decay=0.99) : gam::Pluck(freq, decay) {}
};

/// \ingroup SimpleGamma
/// \copydoc gam::Quantizer
class Quantizer: public gam::Quantizer<REAL> {
public:
	Quantizer(REAL freq=2000, REAL step=0) : gam::Quantizer<REAL>(freq, step) {}
};

// From Gamma/Envelope.h -------------------------------------

/// \ingroup SimpleGamma
/// \copydoc gam::Curve
class Curve : public gam::Curve<REAL, REAL> {
public:
	Curve() : gam::Curve<REAL, REAL>() {}
	Curve(REAL length, REAL curve, REAL start= 1.0, REAL end= 0.0) :
		gam::Curve<REAL, REAL>(length, curve, start, end)
	{}
};

/// \ingroup SimpleGamma
/// \copydoc gam::Env
//class Env {
//public:
//	Env(int numSegments = N, REAL start = 0.0) :
//		gam::Env<numSegments>(att, dec, sus, rel, amp, crv) {}
//};

/// \ingroup SimpleGamma
/// \copydoc gam::ADSR
class ADSR : public gam::ADSR<REAL, REAL> {
public:
	ADSR(REAL att = 0.01, REAL dec = 0.1, REAL sus = 0.7, REAL rel = 1.,
		 REAL amp = 1.0, REAL crv = -4.0) :
		gam::ADSR<REAL, REAL>(att, dec, sus, rel, amp, crv) {}
};

// class AD

/// \ingroup SimpleGamma
/// \copydoc gam::Decay
class Decay : public gam::Decay<REAL> {
public:
	Decay(REAL decay = 1.0, REAL val = 1.0) :
		gam::Decay<REAL>(decay, val)
	{}
};


/// \ingroup SimpleGamma
/// \copydoc gam::Gate
class Gate : public gam::Gate<REAL> {
public:
	Gate(REAL closingDelay=0, REAL threshold=0.001) :
		gam::Gate<REAL>(closingDelay, threshold)
	{}
};

//class Seg;

// class SegExp;

// From Gamma/FFT.h ----------------------------------------------

//class CFFT;
//class RFFT;

// From Gamma/File.h ----------------------------------------------

//class File;

// From Gamma/Filter.h --------------------------------------

//class AllPass1
//class Biquad

/// \ingroup SimpleGamma
/// \copydoc gam::BlockDC
class BlockDC: public gam::BlockDC<REAL, REAL> {
public:
	BlockDC(REAL width=35) :
		gam::BlockDC<REAL, REAL>(width) {}
};

//class BlockNyq
//class Filter2
//class AllPass2
//class Notch

// TODO: Is res required in low pass, hi pass and all pass biquads here?
/// \ingroup SimpleGamma
/// A Second-order Low pass filter
class LowPass: public gam::Biquad<REAL, REAL> {
public:
	LowPass(REAL frq = 1000.0, REAL res = 1.0) :
		gam::Biquad<REAL, REAL>(frq, res, gam::LOW_PASS) {}

};

/// \ingroup SimpleGamma
/// A Second-order High pass filter
class HiPass: public gam::Biquad<REAL, REAL> {
public:
	HiPass(REAL frq = 1000.0, REAL res = 1.0) :
		gam::Biquad<REAL, REAL>(frq, res, gam::HIGH_PASS) {}

};

/// \ingroup SimpleGamma
/// A Second-order All pass filter
class AllPass: public gam::Biquad<REAL, REAL> {
public:
	AllPass(REAL frq = 1000.0, REAL res = 1.0) :
		gam::Biquad<REAL, REAL>(frq, res, gam::ALL_PASS) {}

};

/// \ingroup SimpleGamma
/// \copydoc gam::Reson
class Reson: public gam::Reson<REAL, REAL> {
public:
	Reson(REAL frq = 1000.0, REAL width = 1.0) :
		gam::Reson<REAL, REAL>(frq, width) {}

};

//class Hilbert
//class Integrator
//class MovingAvg
//class OnePole

// From Gamma/FormantData.h --------------------------------------


/// \ingroup SimpleGamma
/// \copydoc gam::Vowel
class Vowel: public gam::Vowel{
};

// From Gamma/Node.h --------------------------------------

//class Node2;
//class Node3;

// From Gamma/Noise.h ----------------------------------------

/// \ingroup SimpleGamma
/// \copydoc gam::NoiseBrown
class NoiseBrown: public gam::NoiseBrown<> {
public:
	NoiseBrown() : gam::NoiseBrown<>() {} // TODO: should seed from current time
	NoiseBrown(uint32_t seed) : gam::NoiseBrown<>(seed) {}
};

/// \ingroup SimpleGamma
/// \copydoc gam::NoisePink
class NoisePink: public gam::NoisePink<> {
public:
	NoisePink() : gam::NoisePink<>() {} // TODO: should seed from current time
	NoisePink(uint32_t seed) : gam::NoisePink<>(seed) {}
};

/// \ingroup SimpleGamma
/// \copydoc gam::NoiseWhite
class NoiseWhite: public gam::NoiseWhite<> {
public:
	NoiseWhite() : gam::NoiseWhite<>() {} // TODO: should seed from current time
	NoiseWhite(uint32_t seed) : gam::NoiseWhite<>(seed) {}
};


// From Gamma/Oscillator.h ----------------------------------------------
/// \ingroup SimpleGamma
/// \copydoc gam::Accum
class Accum: public gam::Accum<> {
public:
    Accum(REAL frq=0.0, REAL phs=0.0) : gam::Accum<>(frq, phs) {}
};

//typedef gam::Sweep<> Sweep;
//typedef gam::AccumPhase<> AccumPhase;
//typedef gam::CSine<> CSine;

/// \ingroup SimpleGamma
/// \copydoc gam::Osc
class Osc: public gam::Osc<REAL> {
public:
    Osc(REAL frq=440, REAL phs=0, uint32_t size=512) : gam::Osc<REAL>(frq, phs, size) {}
    Osc(REAL frq, REAL phs, ArrayPow2<REAL>& src) : gam::Osc<REAL>(frq, phs, src) {}
};

/// \ingroup SimpleGamma
/// \copydoc gam::Sine
class Sine: public gam::Sine<REAL> {
public:
    Sine(REAL frq = 440.0, REAL phs = 0.0) : gam::Sine<REAL>(frq, phs) {}
};

//typedef gam::SineR<> SineR;
//typedef gam::SineRs<> SineRs;
//typedef gam::SineD<> SineD;
//typedef gam::SineDs<> SineDs;
//typedef gam::Buzz<> Buzz;

/// \ingroup SimpleGamma
/// \copydoc gam::LFO
class LFO: public gam::LFO<REAL> {
public:
	LFO(REAL frq = 440.0, REAL phs = 0.0, REAL mod = 0.5)
		: gam::LFO<REAL>(frq, phs, mod) {}
};

/// \ingroup SimpleGamma
/// \copydoc gam::Impulse
class Impulse: public gam::Impulse<REAL> {
public:
	Impulse(REAL frq = 440.0, REAL phs = 0.0) : gam::Impulse<REAL>(frq, phs) {}
};

/// \ingroup SimpleGamma
/// \copydoc gam::Saw
class Saw: public gam::Saw<REAL> {
public:
	Saw(REAL frq = 440.0, REAL phs = 0.0) : gam::Saw<REAL>(frq, phs) {}
};

/// \ingroup SimpleGamma
/// \copydoc gam::Square
class Square: public gam::Square<REAL> {
public:
	Square(REAL frq = 440.0, REAL phs = 0.0) : gam::Square<REAL>(frq, phs) {}
};

/// \ingroup SimpleGamma
/// \copydoc gam::DSF
class DSF: public gam::DSF<REAL> {
public:
	DSF(REAL frq=440, REAL freqRatio=1, REAL ampRatio=0.5, REAL harmonics=8) :
		gam::DSF<REAL>(frq, freqRatio, ampRatio, harmonics) {}
};

//typedef gam::ImpulseFast ImpulseFast;

// From Gamma/Print.h ----------------------------------------------

// No classes

// From Gamma/Recorder.h ----------------------------------------------

/// \ingroup SimpleGamma
/// \copydoc gam::Recorder
class Recorder: public gam::Recorder {
public:
	Recorder(int channels, int frames=8192) : gam::Recorder(channels, frames) {}
};

// From Gamma/SamplePlayer.h ----------------------------------------------

/// \ingroup SimpleGamma
/// \copydoc gam::SamplePlayer
class SamplePlayer: public gam::SamplePlayer<REAL, gam::ipl::Cubic> {
public:
	SamplePlayer() : gam::SamplePlayer<REAL, gam::ipl::Cubic>() {}
	SamplePlayer(Array<REAL>& src, double smpRate, double rate=1)
		: gam::SamplePlayer<REAL, gam::ipl::Cubic>(src, smpRate, rate) {}
	explicit SamplePlayer(const char * pathToSoundFile, double rate=1)
		: gam::SamplePlayer<REAL, gam::ipl::Cubic>(pathToSoundFile, rate) {}
};


/// \ingroup SimpleGamma
/// \copydoc gam::SamplePlayer
class SampleLooper: public gam::SamplePlayer<REAL, gam::ipl::Cubic, gam::phsInc::Loop > {
public:
	SampleLooper() : gam::SamplePlayer<REAL, gam::ipl::Cubic, gam::phsInc::Loop >() {}
	SampleLooper(Array<REAL>& src, double smpRate, double rate=1)
		: gam::SamplePlayer<REAL, gam::ipl::Cubic, gam::phsInc::Loop >(src, smpRate, rate) {}
	explicit SampleLooper(const char * pathToSoundFile, double rate=1)
		: gam::SamplePlayer<REAL, gam::ipl::Cubic, gam::phsInc::Loop >(pathToSoundFile, rate) {}
};

// From Gamma/Sync.h ----------------------------------------------
// Old header for compatibility, nothing useful here


// From Gamma/Scheduler.h ----------------------------------------------

//class Func
//using gam::SchedulerAudioIOData;
// class ProcessNode
// class Process
//class ControlFunc

/// \ingroup SimpleGamma
/// \copydoc gam::Scheduler
class Scheduler: public gam::Scheduler {
public:
	Scheduler() : gam::Scheduler() {}
};

// From Gamma/Strategy.h ----------------------------------------------

// Interpolation strategies, probably not useful in SimpleGamma

// From Gamma/Thread.h ----------------------------------------------

//class Thread;

// From Gamma/Timer.h ----------------------------------------------

using gam::sleep;
using gam::sleepSec;
using gam::sleepUntil;
using gam::timeNow;
using gam::toSec;
using gam::toNsec;

/// \ingroup SimpleGamma
/// \copydoc gam::Timer
class Timer: public gam::Timer {
public:
	Timer() : gam::Timer() {}
};

// From Gamma/Types.h ----------------------------------------------

//class Polar;
using gam::Complex;
//class Vec;

// From Gamma/UnitMaps.h ----------------------------------------------

//class LookupTable;
//class TablePow2
//class CSinTable
//class UnitMapper;

// Other classes ---------------------------------------------

namespace rnd {
/// \ingroup SimpleGamma
using gam::rnd::pick;
/// \ingroup SimpleGamma
using gam::rnd::uni;
}

namespace scl {
/// \ingroup SimpleGamma
using gam::scl::clip;
}

/// \ingroup SimpleGamma
typedef gam::Domain Domain;
}

using gam::WaveformType;
using gam::SINE;				/**< Sine wave */
using gam::COSINE;				/**< Cosine wave */
using gam::TRIANGLE;			/**< Triangle wave */
using gam::PARABOLIC;			/**< Parabolic wave */
using gam::SQUARE;				/**< Square wave */
using gam::SAW;				/**< Saw wave */
using gam::IMPULSE;				/**< Impulse wave */

/// \ingroup SimpleGamma
/// \copydoc gam::addWave
using gam::addWave;
/// \ingroup SimpleGamma
/// \copydoc gam::addSine
using gam::addSine;
/// \ingroup SimpleGamma
/// \copydoc gam::addSines
using gam::addSines;


#endif // SIMPLEGAMMA_H

