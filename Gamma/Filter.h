#ifndef GAMMA_FILTER_H_INC
#define GAMMA_FILTER_H_INC

/*	Gamma - Generic processing library
	See COPYRIGHT file for authors and license information */


/// \defgroup Filters
/// <A HREF="https://ccrma.stanford.edu/~jos/filters/What_Filter.html">
/// When you think about it, everything is a filter.</A>

#include "Gamma/ipl.h"
#include "Gamma/scl.h"

#include "Gamma/Containers.h"
//#include "Gamma/Strategy.h"
#include "Gamma/Domain.h"
#include "Gamma/Types.h"

namespace gam{


/// Filter types
enum FilterType{
	LOW_PASS,			/**< Low-pass */
	HIGH_PASS,			/**< High-pass */
	BAND_PASS,			/**< Band-pass */
	BAND_PASS_UNIT,		/**< Band-pass with unit gain */
	BAND_REJECT,		/**< Band-reject */
	ALL_PASS			/**< All-pass */
//	COMB_FBK_EVEN,
//	COMB_FBK_ODD,
//	COMB_FFD_EVEN,
//	COMB_FFD_ODD
};



/// Returns pole radius given a bandwidth and sampling interval
template <class T>
inline T poleRadius(T bw, double ups){ return ::exp(-M_PI * bw * ups); }
//return (T)1 - (M_2PI * bw * ups); // linear apx for fn < ~0.02

/// Convert domain frequency to radians clipped to interval [0, pi).
template <class T>
inline T freqToRad(T freq, double ups){ return scl::clip(freq * ups, 0.499) * M_PI; }



/// First-order all-pass filter

/// This filter has the transfer function H(z) = (a + z^-1) / (1 + a z^-1).
/// It is essentially a "phase" filter in that its only effect is to 
/// shift frequencies from 0 to Nyquist from 0 to -180 degress, respectively.
/// The cutoff frequency is the frequency where the phase is shifted by -90 
/// degrees.  A lowpass or highpass filter can be constructed by adding or 
/// subtracting, respectively, the output of the filter to or from the input.
/// When the center frequency is fs/4, the filter acts as a unit delay.
/// When the center frequency is 0, the filter acts as a no-op.
/// When the center frequency is fs/2, the filter acts as an inverter.
/// \tparam Tv	Value (sample) type
/// \tparam Tp	Parameter type
/// \tparam Td	Domain observer type
/// \ingroup Filters
template<class Tv=gam::real, class Tp=gam::real, class Td=DomainObserver>
class AllPass1 : public Td {
public:
	///
	/// \param[in]	frq		Center frequency
	AllPass1(Tp frq=1000);

	void freq (Tp v);		///< Set cutoff frequency
	void freqF(Tp v);		///< Faster, but slightly less accurate than freq()	
	void zero(){ d1=Tv(0); }
	
	Tv operator()(Tv input);///< Filters sample
	
	Tv high(Tv input);		///< High-pass filters sample
	Tv low (Tv input);		///< Low-pass filters sample
	
	Tp freq();				///< Get current cutoff frequency
	
	virtual void onDomainChange(double r);
	
protected:
	Tv d1;		// once delayed value
	Tp c;		// feed coefficient
	Tp mFreq;
};


/// 2-pole/2-zero IIR filter

/// The biquadratic filter contains 2 zeros and 2 poles in its transfer
/// function. The zeros provide a better response near the DC and Nyquist
/// frequencies than an all-pole filter would. Second-order IIRs have a 12 
/// dB/octave cutoff slope and are normally cascaded (run in series) to obtain
/// a sharper response. This particular implementation utilizes the RBJ 
/// Sallen-Key/Butterworth design from:
///
/// http://www.musicdsp.org/files/Audio-EQ-Cookbook.txt
///
/// \tparam Tv	Value (sample) type
/// \tparam Tp	Parameter type
/// \tparam Td	Domain observer type
/// \ingroup Filters
template <class Tv=gam::real, class Tp=gam::real, class Td=DomainObserver>
class Biquad : public Td{
public:

	/// \param[in]	frq		Center frequency
	/// \param[in]	res		Resonance (Q) amount in [1, inf)
	/// \param[in]	type	Type of filter
	Biquad(Tp frq = Tp(1000), Tp res = Tp(1), FilterType type = LOW_PASS);


	/// Get array of 3 feedforward coefficients
	Tp * a(){ return mA; }
	
	/// Get array of 3 feedback coefficients (first element, b0, is not used)
	Tp * b(){ return mB; }

	const Tp * a() const { return mA; }
	const Tp * b() const { return mB; }

	/// Set feedforward (a) and feedback (b) coefficients directly
	void coef(Tp a0, Tp a1, Tp a2, Tp b1, Tp b2);


	void freq(Tp v);					///< Set center frequency
	void res(Tp v);						///< Set resonance (Q)
	void set(Tp frq, Tp res);			///< Set filter center frequency and resonance
	void set(Tp frq, Tp res, FilterType type);	///< Set all filter params
	void type(FilterType type);			///< Set type of filter
	void zero();						///< Zero internal delays

	Tv operator()(Tv i0);				///< Filter next sample
	Tv nextBP(Tv i0);					///< Optimized for band-pass types
	
	Tp freq() const { return mFreq; }	///< Get center frequency
	Tp res() const { return mRes; }		///< Get resonance (Q)
	FilterType type() const { return mType; }	///< Get filter type
	
	virtual void onDomainChange(double r);

protected:
	Tp mA[3];			// feedforward coefs
	Tp mB[3];			// feedback coefs (first element used to scale coefs)
	Tv d1, d2;			// inner sample delays
	Tp mFreq, mRes;		// center frequency, resonance
	FilterType mType;
	Tp mReal, mImag;	// real, imag components of center frequency
	Tp mAlpha;
	Tp mFrqToRad;
};



/// DC frequency blocker

/// \tparam Tv	Value (sample) type
/// \tparam Tp	Parameter type
/// \tparam Td	Domain observer type
/// \ingroup Filters
template <class Tv=gam::real, class Tp=gam::real, class Td=DomainObserver>
class BlockDC : public Td{
public:

	/// \param[in] width	Bandwidth of pole
	BlockDC(Tp width=35): d1(0), mWidth(width){ Td::refreshDomain(); }

	/// Filter sample
	Tv operator()(Tv i0){		
		i0 += d1*mB1;
		Tv o0 = i0-d1;
		d1 = i0;
		return o0;
	}

	/// Set bandwidth of pole
	void width(Tp v){
		mWidth = v;
		mB1 = poleRadius(v, Td::ups());
	}

	void zero(){ d1=0; }

	virtual void onDomainChange(double r){ width(mWidth); }

protected:
	Tv d1;
	Tp mWidth, mB1;
};



/// Nyquist frequency blocker

/// \tparam Tv	Value (sample) type
/// \tparam Tp	Parameter type
/// \tparam Td	Domain observer type
/// \ingroup Filters  
template <class Tv=gam::real, class Tp=gam::real, class Td=DomainObserver>
class BlockNyq : public BlockDC<Tv,Tp,Td>{
public:

	/// \param[in] width	Bandwidth of pole
	BlockNyq(Tp width=35): Base(width){}

	/// Filter sample
	Tv operator()(Tv i0){		
		i0 += d1*mB1;
		Tv o0 = i0-d1;
		d1 =-i0;
		return o0;
	}

protected:
	typedef BlockDC<Tv,Tp,Td> Base;
	using Base::d1; using Base::mB1;
};



/// Abstract base class for 2-pole or 2-zero filter

/// \tparam Tv	Value (sample) type
/// \tparam Tp	Parameter type
/// \tparam Td	Domain observer type
/// \ingroup Filters
template <class Tv=gam::real, class Tp=gam::real, class Td=DomainObserver>
class Filter2 : public Td{
public:

	/// Set frequency
	void freq(Tp v){ freqRef(v); }

	/// Set bandwidth
	void width(Tp v){
		mWidth = v;
		mRad = poleRadius(mWidth, Td::ups());
		mC[2] = -mRad * mRad;
		computeCoef1();
	}

	/// Zero delay elements
	void zero(){ d2=d1=Tv(0); }
	
	virtual void onDomainChange(double r){ freq(mFreq); width(mWidth); }

protected:

	Filter2(Tp frq, Tp wid)
	:	mFreq(frq), mWidth(wid)
	{	zero(); }

	void freqRef(Tp& v){
		mFreq = v;		
		v = scl::clip<Tp>(v * Td::ups(), 0.5);
		//mCos = scl::cosP3<Tp>(v);
		mCos = scl::cosT8<Tp>(v * M_2PI);
		computeCoef1();
	}
	
	void computeCoef1(){ mC[1] = Tp(2) * mRad * mCos; }
	void delay(Tv v){ d2=d1; d1=v; }
	Tp& gain(){ return mC[0]; }
	
	Tp mFreq, mWidth;
	Tp mC[3];			// coefficients
	Tp mCos, mRad;
	Tv d2, d1;			// 2- and 1-sample delays
};


#define INHERIT_FILTER2 \
typedef Filter2<Tv,Tp,Td> Base;\
using Base::mFreq;\
using Base::mWidth;\
using Base::gain;\
using Base::mC;\
using Base::mRad;\
using Base::mCos;\
using Base::d2;\
using Base::d1


/// Second-order all-pass filter

/// This all-pass filter shifts phases from 0 to 2 pi from 0 to Nyquist. The
/// center frequency controls where the phase is shifted by pi.
/// \tparam Tv	Value (sample) type
/// \tparam Tp	Parameter type
/// \tparam Td	Domain observer type
/// \ingroup Filters
template <class Tv=gam::real, class Tp=gam::real, class Td=DomainObserver>
class AllPass2 : public Filter2<Tv,Tp,Td>{
public:

	/// \param[in] frq	Center frequency
	/// \param[in] wid	Bandwidth
	AllPass2(Tp frq=1000, Tp wid=100): Base(frq, wid){ Td::refreshDomain(); }

	/// Filter sample
	Tv operator()(Tv i0){
		i0 += d1*mC[1] + d2*mC[2];
		Tv o0 = i0*-mC[2] - d1*mC[1] + d2;
		this->delay(i0);
		return o0;
	}

protected:
	INHERIT_FILTER2;
};



/// Two-zero notch

/// \tparam Tv	Value (sample) type
/// \tparam Tp	Parameter type
/// \tparam Td	Domain observer type
/// \ingroup Filters
template <class Tv=gam::real, class Tp=gam::real, class Td=DomainObserver>
class Notch : public Filter2<Tv,Tp,Td>{
public:
	
	/// \param[in] frq	Center frequency
	/// \param[in] wid	Bandwidth
	Notch(Tp frq=1000, Tp wid=100): Base(frq, wid){ Td::refreshDomain(); }

	/// Set center frequency
	void freq(Tp v){ Base::freq(v); computeGain(); }

	/// Set bandwidth
	void width(Tp v){ Base::width(v); computeGain(); }

	/// Filter sample
	Tv operator()(Tv i0){
		i0 *= gain();
		Tv o0 = i0 - d1*mC[1] - d2*mC[2];
		this->delay(i0);
		return o0;
	}

	virtual void onDomainChange(double r){ freq(mFreq); width(mWidth); }

protected:
	INHERIT_FILTER2;

	// compute constant gain factor
	void computeGain(){ gain() = Tp(1) / (Tp(1) + scl::abs(mC[1]) - mC[2]); }
};



/// Two-pole resonator

/// \tparam Tv	Value (sample) type
/// \tparam Tp	Parameter type
/// \tparam Td	Domain observer type
/// \ingroup Filters
template <class Tv=gam::real, class Tp=gam::real, class Td=DomainObserver>
class Reson : public Filter2<Tv,Tp,Td>{
public:

	/// \param[in] frq	Center frequency
	/// \param[in] wid	Bandwidth	
	Reson(Tp frq=440, Tp wid=100): Base(frq, wid){ Td::refreshDomain(); }

	/// Set center frequency
	void freq(Tp v){
		Base::freqRef(v);
		mSin = scl::cosP3<Tp>(scl::foldOnce<Tp>(v - Tp(0.25), Tp(0.5)));
		computeGain();
	}

	/// Set bandwidth
	void width(Tp v){ Base::width(v); computeGain(); }

	void set(Tp frq, Tp wid){ Base::width(wid); freq(frq); }

	/// Filter sample
	Tv operator()(Tv i0){
		i0 *= gain();
		i0 += d1*mC[1] + d2*mC[2];
		this->delay(i0);
		return i0; 
	}

	void onDomainChange(double r){ freq(mFreq); width(mWidth); }

protected:
	INHERIT_FILTER2;
	Tp mSin;

	// compute constant gain factor
	void computeGain(){ gain() = (Tp(1) - mRad*mRad) * mSin; }
};



/// Hilbert transform filter

/// This filter converts a real signal into a complex (analytic) signal.
/// Corresponding harmonics of the real and imaginary parts of the output are
/// 90 degrees out of phase.
/// The analytic signal can be used for frequency shifting or determining the
/// amplitude envelope of the input signal.
/// Technically speaking, a Hilbert transform converts a real signal into its
/// harmonic conjugate. The input and output of the Hilbert transform, comprise
/// the real and imaginary components of a complex (analytic) signal.
///
/// \tparam Tv	Value (sample) type
/// \tparam Tp	Parameter type
/// \ingroup Filters
template <class Tv=gam::real, class Tp=gam::real>
class Hilbert {
public:
	#define SR 44100.
	Hilbert()
	:	cf0(   5.4135/SR), cf1(  41.118 /SR), cf2(  167.3595/SR),	/* for SR=44100 */
		cf3( 671.3715/SR), cf4(2694.363 /SR), cf5(11976.867 /SR),
		sf0(  18.786 /SR), sf1(  83.5065/SR), sf2(  335.1345/SR), 
		sf3(1344.4065/SR), sf4(5471.871 /SR), sf5(41551.671 /SR)
	{}
	#undef SR

	/// Convert input from real to complex
	Complex<Tv> operator()(const Tv& i){
		return Complex<Tv>(
			 cf0(cf1(cf2(cf3(cf4(cf5(i)))))),
			-sf0(sf1(sf2(sf3(sf4(sf5(i))))))
		);
	}

protected:
	AllPass1<Tv, Tp, Domain1> cf0,cf1,cf2,cf3,cf4,cf5, sf0,sf1,sf2,sf3,sf4,sf5;
};



/// Leaky integrator

/// \tparam Tv	Value (sample) type
/// \tparam Tp	Parameter type
/// \ingroup Filters
template <class Tv=double, class Tp=double>
class Integrator{
public:

	/// \param[in] leakCoef		Leak coefficient, in [0,1)
	/// \param[in] v			Initial value
	Integrator(const Tp& leakCoef=Tp(1), const Tv& v=Tv(0)){
		mo[0]=v;
		leak(leakCoef);
	}

	/// Filter input value
	Tv operator()(const Tv& i0) const { return mo[0]=mo[0]*mb[0] + i0; }
	
	Integrator& leak(const Tp& v){ mb[0]=v; return *this; }
	Integrator& zero(){ mo[0]=Tv(0); return *this; }

protected:
	mutable Tv mo[1];
	Tp mb[1];
};



/// Moving average filter

/// The moving average filter is a special case of an FIR filter whose kernel
/// is a rectangular window with a magnitude equal to the inverse of the kernel
/// size. Due to the symmetry of the window, the moving average filter can be
/// implemented efficiently using a single delay line with O(1) processing time
/// complexity.
/// \ingroup Filters
template <class Tv=gam::real>
class MovingAvg : public DelayN<Tv>{
public:

	/// \param[in] size		Kernel size, greater than 1
	explicit MovingAvg(uint32_t size)
	:	Base(size), mSum(0), mRSize(0)
	{	onResize(); }

	MovingAvg& operator=(const Tv& v){ DelayN<Tv>::operator=(v); return *this; }
	
	Tv operator()(const Tv& i0){
		mSum += i0 - Base::operator()(i0);
		return mSum * mRSize;
	}
	
	void zero(){ this->assign(Tv(0)); }

	virtual void onResize(){
		mRSize = 1./Base::size();
		//printf("mRSize = %g\n", mRSize);
	}

protected:
	typedef DelayN<Tv> Base;
	
	Tv mSum;		// the moving sum
	double mRSize;	// reciprocal of the size
};



/// One-pole smoothing filter

/// \tparam Tv	Value (sample) type
/// \tparam Tp	Parameter type
/// \tparam Td	Domain observer type
/// \ingroup Filters
template<class Tv=gam::real, class Tp=gam::real, class Td=DomainObserver>
class OnePole : public Td{ 
public:
	OnePole();

	/// \param[in]	freq	Smoothing frequency
	/// \param[in]	stored	Initial stored value
	OnePole(Tp freq, const Tv& stored=0);

	const Tp& freq() const { return mFreq; }	///< Get cutoff frequency

	void operator  = (const Tv& val);	///< Stores input value for operator()
	void operator *= (const Tv& val);	///< Multiplies stored value by value
	void freq(Tp val);					///< Set cutoff frequency (-3 dB bandwidth of pole)
	void smooth(Tp val);				///< Set smoothing coefficient directly
	void zero(){ o1=0; }				///< Zero internal delay
	void reset(const Tv& v){ o1=v; mStored=v; }

	const Tv& operator()();				///< Returns filtered output using stored value
	const Tv& operator()(const Tv& input);		///< Returns filtered output from input value
	const Tv& last() const;				///< Returns last output
	const Tv& stored() const;			///< Returns stored value
	Tv& stored();						///< Returns stored value
	bool zeroing(Tv eps=0.0001) const;	///< Returns whether the filter is outputting zeros
	
	virtual void onDomainChange(double r);

protected:
	Tp mFreq, mA0, mB1;
	Tv mStored, o1;
};




// Implementation_______________________________________________________________

//---- AllPass1

template <class Tv, class Tp, class Td>
AllPass1<Tv,Tp,Td>::AllPass1(Tp frq)
:	d1(Tv(0)), mFreq(frq)
{	Td::refreshDomain(); }

template <class Tv, class Tp, class Td>
inline void AllPass1<Tv,Tp,Td>::freq(Tp v){
	mFreq = v;
	c = tan( freqToRad(v, Td::ups()) - M_PI_4); // valid ang in [-pi/4, pi/4]
}

template <class Tv, class Tp, class Td>
inline void AllPass1<Tv,Tp,Td>::freqF(Tp v){
	mFreq = v;
	c = freqToRad(v, Td::ups());
	c = Tp(1.27323954474) * c - Tp(1);		// 4/pi * c - 1, linear apx
	c = c * (Tp(0.76) + Tp(0.24) * c * c);
}

template <class Tv, class Tp, class Td> 
inline Tv AllPass1<Tv,Tp,Td>::operator()(Tv i0){
/*
	// Direct Form I
	//o0 = i0*c + i1 - o1*c;
	o0 = (i0 - o1)*c + i1;
	i1 = i0;
	o1 = o0;
*/
	i0 -= d1 * c;			// o0 = c * (i0 - c * d1) + d1
	Tv o0 = i0 * c + d1;	//    = c * i0 + (1 - c*c) * d1
	d1 = i0;
	return o0;
}

template <class Tv, class Tp, class Td>
inline Tv AllPass1<Tv,Tp,Td>::high(Tv i0){ return (i0 - operator()(i0)) * Tv(0.5); }
    
template <class Tv, class Tp, class Td>
inline Tv AllPass1<Tv,Tp,Td>::low (Tv i0){ return (i0 + operator()(i0)) * Tv(0.5); }

template <class Tv, class Tp, class Td>
inline Tp AllPass1<Tv,Tp,Td>::freq(){ return mFreq; }
    
template <class Tv, class Tp, class Td>
void AllPass1<Tv,Tp,Td>::onDomainChange(double r){ freq(mFreq); }



//---- Biquad
template <class Tv, class Tp, class Td>
Biquad<Tv,Tp,Td>::Biquad(Tp frq, Tp res, FilterType type)
:	d1(0), d2(0)
{
	Td::refreshDomain();
	set(frq, res, type);
}

template <class Tv, class Tp, class Td>
void Biquad<Tv,Tp,Td>::onDomainChange(double r){
	mFrqToRad = M_2PI * Td::ups();
	freq(mFreq);
}

template <class Tv, class Tp, class Td>
void Biquad<Tv,Tp,Td>::set(Tp freqA, Tp resA, FilterType typeA){
	mRes = resA;
	mType = typeA;
	freq(freqA);
}

template <class Tv, class Tp, class Td>
void Biquad<Tv,Tp,Td>::zero(){ d1=d2=(Tv)0; }

template <class Tv, class Tp, class Td>
void Biquad<Tv,Tp,Td>::coef(Tp a0, Tp a1, Tp a2, Tp b1, Tp b2){
	mA[0]=a0; mA[1]=a1; mA[2]=a2; mB[1]=b1; mB[2]=b2;
}

template <class Tv, class Tp, class Td>
inline void Biquad<Tv,Tp,Td>::freq(Tp v){
	mFreq = v;
	float w = scl::clip(mFreq * mFrqToRad, 3.11f);
	mReal = scl::cosT8(w);
	mImag = scl::sinT7(w);
	res(mRes);
}

template <class Tv, class Tp, class Td>
inline void Biquad<Tv,Tp,Td>::res(Tp v){
	mRes = v;
	mAlpha = mImag / mRes;

	// Note: mB[0] is not used in the difference equation since it is assumed to
	// be equal to 1. It is only used for gain control ...
	mB[0] = Tp(1) / (Tp(1) + mAlpha);	// reciprocal of b0
	mB[1] = Tp(-2) * mReal * mB[0];
	mB[2] = (Tp(1) - mAlpha) * mB[0];
	
	type(mType);
}

template <class Tv, class Tp, class Td>
inline void Biquad<Tv,Tp,Td>::set(Tp frq, Tp res){ set(frq, res, mType); }

template <class Tv, class Tp, class Td>
inline void Biquad<Tv,Tp,Td>::type(FilterType typeA){
	mType = typeA;
	
	switch(mType){
	case LOW_PASS:
		mA[1] = (Tp(1) - mReal) * mB[0];
		mA[0] = mA[1] * Tp(0.5);
		mA[2] = mA[0];
		break;
	case HIGH_PASS: // same as low-pass, but with sign flipped on odd a_k
		mA[1] =-(Tp(1) + mReal) * mB[0];
		mA[0] =-mA[1] * Tp(0.5);
		mA[2] = mA[0];
		break;
	case BAND_PASS:
		mA[0] = mImag * Tp(0.5) * mB[0];
		mA[1] = Tp(0);
		mA[2] =-mA[0];
		break;
	case BAND_PASS_UNIT:
        mA[0] = mAlpha * mB[0];
        mA[1] = Tp(0);
        mA[2] =-mA[0];
		break;
	case BAND_REJECT:
        mA[0] = mB[0];	// 1.f * a0
        mA[1] = mB[1];
        mA[2] = mB[0];	// 1.f * a0
		break;
	case ALL_PASS:
		mA[0] = mB[2];
		mA[1] = mB[1];
		mA[2] = Tp(1);
		break;
	default:;
	};
}

template <class Tv, class Tp, class Td>
inline Tv Biquad<Tv,Tp,Td>::operator()(Tv i0){
	// Direct form II
	i0 = i0 - d1*mB[1] - d2*mB[2];
	Tv o0 = i0*mA[0] + d1*mA[1] + d2*mA[2];
	d2 = d1; d1 = i0;
	return o0;
}

template <class Tv, class Tp, class Td>
inline Tv Biquad<Tv,Tp,Td>::nextBP(Tv i0){
	i0 = i0 - d1*mB[1] - d2*mB[2];	
	Tv o0 = (i0 - d2)*mA[0];
	d2 = d1; d1 = i0;
	return o0;
}


//---- OnePole
template <class Tv, class Tp, class Td>
OnePole<Tv,Tp,Td>::OnePole()
:	mFreq(10), mStored(Tv(0)), o1(Tv(0))
{	Td::refreshDomain(); }

template <class Tv, class Tp, class Td>
OnePole<Tv,Tp,Td>::OnePole(Tp frq, const Tv& stored)
:	mFreq(frq), mStored(stored), o1(stored)
{	Td::refreshDomain(); }

template <class Tv, class Tp, class Td>
void OnePole<Tv,Tp,Td>::onDomainChange(double r){ freq(mFreq); }

template <class Tv, class Tp, class Td>
inline void OnePole<Tv,Tp,Td>::operator  = (const Tv& v){ mStored  = v; }
    
template <class Tv, class Tp, class Td>
inline void OnePole<Tv,Tp,Td>::operator *= (const Tv& v){ mStored *= v; }

// f = ln(mB1) / -M_2PI * SR  ( @ 44.1 f = ln(c01) * -7018.733)
// @ 44.1 : 0.9     <=> 739.5
// @ 44.1 : 0.99    <=>  70.54
// @ 44.1 : 0.999   <=>   7.022
// @ 44.1 : 0.9999  <=>   0.702
// @ 44.1 : 0.99999 <=>   0.0702
template <class Tv, class Tp, class Td>
inline void OnePole<Tv,Tp,Td>::freq(Tp v){
	mFreq = v;	
	v = scl::max(v, Tp(0));	// ensure positive freq
	
	// freq is half the bandwidth of a pole at 0
	smooth( poleRadius(Tp(2) * v, Td::ups()) );
	//printf("%f, %f, %f\n", Td::spu(), mB1, v);
}

template <class Tv, class Tp, class Td>
inline void OnePole<Tv,Tp,Td>::smooth(Tp v){ mB1=v; mA0=Tp(1) - scl::abs(v); }

template <class Tv, class Tp, class Td>
inline const Tv& OnePole<Tv,Tp,Td>::operator()(){ return (*this)(mStored); }
    
template <class Tv, class Tp, class Td>
inline const Tv& OnePole<Tv,Tp,Td>::operator()(const Tv& i0){ o1 = o1*mB1 + i0*mA0; return o1; }

template <class Tv, class Tp, class Td>
inline const Tv& OnePole<Tv,Tp,Td>::last() const { return o1; }
    
template <class Tv, class Tp, class Td>
inline const Tv& OnePole<Tv,Tp,Td>::stored() const { return mStored; }
    
template <class Tv, class Tp, class Td>
inline Tv& OnePole<Tv,Tp,Td>::stored(){ return mStored; }
    
template <class Tv, class Tp, class Td>
inline bool OnePole<Tv,Tp,Td>::zeroing(Tv eps) const { return scl::abs(o1) < eps && mStored == Tv(0); }
} // gam::
#endif
