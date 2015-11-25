/** \file
*   Mini compressor with side chain - uses the side chain signal to drive compression.   
*
*/

// C++ scripting support-----------------------------
#include "dspapi.h"
#include "cpphelpers.h"

DSP_EXPORT double  sampleRate = 0;
DSP_EXPORT uint    auxAudioInputsCount = 0; 
DSP_EXPORT uint    audioInputsCount = 0;

// extra system headers
#include <math.h>

DSP_EXPORT string name="Mini Comp SC";
DSP_EXPORT string description="mini side chain compressor";

/* Define our parameters.
*/
DSP_EXPORT array<string>  inputParametersNames={"Threshold","Ratio", "Attack", "Release","Make Up"};
DSP_EXPORT array<double> inputParameters(inputParametersNames.length);
DSP_EXPORT array<double> inputParametersDefault={1,0,.1,.5,0};
DSP_EXPORT array<string>  outputParametersNames={"GR"};
DSP_EXPORT array<double> outputParameters(outputParametersNames.length);

// internal variables
double threshold=1;
double ratio=1;
double level=0;
double attackCoeff=0;
double releaseCoeff=0;
double gain=0;
double makeUpGain=0;

DSP_EXPORT void processSample(double ioSample[])
{
    // compute average absolute value
    double absValue=0;
    if(auxAudioInputsCount>0)
    {
        for(uint channel=0;channel<auxAudioInputsCount;channel++)
        {
            uint ch=channel+audioInputsCount;
            absValue+=fabs(ioSample[ch]);
        }
        absValue/=double(auxAudioInputsCount);
    }

    // update level
    if(absValue>level)
        level+=attackCoeff*(absValue-level);
    else
        level+=releaseCoeff*(absValue-level);

    // apply ratio if level above threshold
    gain=1;
    if(level>threshold)
    {
        gain=(ratio*(level-threshold)+threshold)/level;
    }
    double actualGain=gain*makeUpGain;
    for(uint channel=0;channel<audioInputsCount;channel++)
    {
        ioSample[channel]*=actualGain;
    }
}

DSP_EXPORT void updateInputParameters()
{
    threshold=pow(10,3*(inputParameters[0]-1)); // -60 to 0 dB
    ratio=1-log(1+100*inputParameters[1])/log(101.0);
    attackCoeff=pow(10,1.0/(50+.5*sampleRate*inputParameters[2]))-1;
    releaseCoeff=pow(10,1.0/(50+.5*sampleRate*inputParameters[3]))-1;
    makeUpGain=pow(10,1.5*inputParameters[4]); // 0 to 30dB
}

DSP_EXPORT void computeOutputData()
{
    // display compression from 0 to 60 dB
    double compGain=0;
    if(gain>0)
    {
        compGain=20*log(gain);
        if(compGain<-60)
            compGain=-60;
        compGain/=-60;
    }
    outputParameters[0]=compGain;
}