// biquad filter helper class
#include "BiquadFilter.hxx"

const double PI=3.141592653589793238462;

/** \file tpl.filter)curve.cxx
 * Helper functions to compute the response curve of the filters defined in corresponding scripts.
 * Note that we use 41000 as the sample rate since for display purposes the actual sample rate does not matter 
 * much, given the frequncy range in use (20 to 20k)
 */

// change $id$ curve to display low pass resonant filter
void $id$_setResonantLowPass(double p0,double p1)
{
    // create a temp filter to compute coefficients with the same values as in the script
    KittyDSP::Biquad::Filter filter(0);
    double freqFactor=2*PI*20.0/44100.0;
    double q=pow(10,(-3+p1*23.0)/20.0);
    filter.setResonantLowPass(freqFactor*pow(1000,p0),q);
    $id$_setFromBiquad(filter);
}

// change $id$ curve to display a high pass resonant filter
void $id$_setResonantHighPass(double p0,double p1)
{
    // create a temp filter to compute coefficients with the same values as in the script
    KittyDSP::Biquad::Filter filter(0);
    double freqFactor=2*PI*20.0/44100.0;
    double q=pow(10,(-3+p1*23.0)/20.0);
    filter.setResonantHighPass(freqFactor*pow(1000,p0),q);
    $id$_setFromBiquad(filter);
}

// change $id$ curve to display a peak filter
void $id$_setPeak(double p0,double p1,double p2)
{
    // create a temp filter to compute coefficients with the same values as in the script
    KittyDSP::Biquad::Filter filter(0);
    double freqFactor=2*PI*20.0/44100.0;
    double bwRange=1;
    double minBw=1.0/10.0;
    filter.setPeak(freqFactor*pow(1000,p0),minBw+p1*bwRange,pow(10,-.5+p2));
    $id$_setFromBiquad(filter);
}

// change $id$ curve to display a notch filter
void $id$_setNotch(double p0,double p1)
{
    // create a temp filter to compute coefficients with the same values as in the script
    KittyDSP::Biquad::Filter filter(0);
    double freqFactor=2*PI*20.0/44100.0;
    double minBw=1.0/10.0;
    double bwRange=1;
    filter.setNotch(freqFactor*pow(1000,p0),minBw+p1*bwRange);
    $id$_setFromBiquad(filter);
}

// generic function to set $id$ from filter coefficients
void $id$_setFromBiquad(const KittyDSP::Biquad::Filter& filter)
{
    // store in temp variables (easier to read below)
    const double b0=filter.inputCoeff0;
    const double b1=filter.inputCoeff1;
    const double b2=filter.inputCoeff2;
    const double a1=-filter.outputCoeff1;
    const double a2=-filter.outputCoeff2;
    
    // copy to actual curve constants
    $id$.kb0=b0*b0+b1*b1+b2*b2;
    $id$.kb1=2*(b0*b1+b1*b2);
    $id$.kb2=2*b0*b2;
    $id$.ka0=1+a1*a1+a2*a2;
    $id$.ka1=2*(a1+a1*a2);
    $id$.ka2=2*a2;
}