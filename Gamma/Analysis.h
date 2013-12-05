#ifndef GAMMA_ANALYSIS_H_INC
#define GAMMA_ANALYSIS_H_INC

/*	Gamma - Generic processing library
	See COPYRIGHT file for authors and license information */

#include "Gamma/Filter.h"

namespace gam{

///\defgroup Analysis


/// Envelope follower

/// This object produces an estimate of the amplitude envelope of a signal
/// by feeding a full-wave rectification of the signal through a low-pass filter.
///\ingroup Filters, Envelopes, Analysis
template <class Tv=real, class Tp=real, class Td=DomainObserver>
class EnvFollow{
public:

	/// \param[in] freq		Cutoff frequency of smoothing filter
	EnvFollow(Tp freq=10)
	:	lpf(freq){}


	/// Filter next sample
	Tv operator()(Tv i0){ return lpf(scl::abs(i0)); }

	/// Returns current amplitude estimate
	Tv value() const { return lpf.last(); }
	
	bool done(Tv eps=0.001) const { return value() < eps; }

	OnePole<Tv,Tp,Td> lpf;	///< Low-pass filter
};



/// Compares signal magnitude to a threshold

/// This filter compares the input magnitude to a threshold and returns 1 if 
/// it's greater than the threshold and 0 otherwise.  The output is sent through
/// a one-pole low-pass filter.
///\ingroup Analysis
template <class T=gam::real>
class Threshold{
public:
	/// \param[in] thresh	Comparing threshold
	/// \param[in] freq		Cutoff frequency of output smoother
	Threshold(T thresh, T freq=10):lpf(freq), thresh(thresh){}
	
	/// Returns 0 if less than threshold, 1 otherwise
	T operator()(T i0){ return lpf(scl::abs(i0) > thresh ? T(1) : T(0)); }
	
	/// Returns 1 if less than threshold, 0 otherwise
	T inv(T i0){ return lpf(scl::abs(i0) > thresh ? T(0) : T(1)); }
	
	OnePole<T> lpf;	///< Output smoother
	T thresh;		///< Threshold value
};



/// Zero-crossing detector

/// This object determines when a zero crossing occurs. It can distinguish 
/// between both positive (rising) and negative (falling) zero crossings.
///\ingroup Analysis
template<class Tv=gam::real>
class ZeroCross{
public:

	ZeroCross(Tv prev = Tv(0)): mPrev(prev){}

	/// Detect zero crossing

	/// \returns 
	///		 0 if no zero crossing,
	///		-1 if a negative (falling) zero crossing, and
	///		 1 if a positive (rising) zero crossing.
	int operator()(Tv input){
		int pzc = int((input > Tv(0)) && (mPrev <= Tv(0)));
		int nzc =-int((input < Tv(0)) && (mPrev >= Tv(0)));
		mPrev = input;
		return pzc + nzc;
	}

private:
	Tv mPrev;
};



/// Computes the zero-crossing rate of an input signal

/// The zero-crossing rate (ZCR) is a measure proportional to how many times a 
/// signal crosses zero over a given number of samples. It can be used to 
/// distinguish between noise (high ZCR) and tones (low ZCR).
///\ingroup Analysis
class ZeroCrossRate{
public:

	/// \param[in] winSize		size of analysis window
	ZeroCrossRate(int winSize=64)
	:	mRate(0), mWinSize(winSize), mCrosses(0), mCount(0)
	{}

	/// Get the current zero-crossing rate, in [0, 0.5]
	float rate() const { return mRate; }

	/// Input next sample and return current zero-crossing rate
	float operator()(float input){
		if(0 != mDetector(input)){
			++mCrosses;
		}
		++mCount;
		if(mCount >= mWinSize){
			mRate = float(mCrosses) / mWinSize;
			mCrosses = 0;
			mCount = 0;
		}
		return mRate;
	}

private:
	ZeroCross<float> mDetector;
	float mRate;
	unsigned mWinSize;
	unsigned mCrosses;
	unsigned mCount;
};

} // gam::
#endif
