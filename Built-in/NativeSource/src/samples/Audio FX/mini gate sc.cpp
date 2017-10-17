/** \file
*   Mini gate effect with side chain -  uses the side chain signal to trigger the gate. 
*/

// C++ scripting support-----------------------------
#include "dspapi.h"
#include "cpphelpers.h"

// extra system headers
#include <math.h>

DSP_EXPORT double  sampleRate = 0;
DSP_EXPORT uint    auxAudioInputsCount = 0;
DSP_EXPORT uint    audioInputsCount = 0;

DSP_EXPORT string name="Mini Gate SC";
DSP_EXPORT string author="Blue Cat Audio";
DSP_EXPORT string description="mini side chain gate";

/* Define our parameters.
*/
DSP_EXPORT array<string>  inputParametersNames={"Threshold","Ratio", "Attack", "Release"};
DSP_EXPORT array<double> inputParameters(inputParametersNames.length);
DSP_EXPORT array<double> inputParametersDefault={0,0,.1,.5};
DSP_EXPORT array<string>  outputParametersNames={"GR"};
DSP_EXPORT array<double> outputParameters(outputParametersNames.length);

// internal variables
double threshold=1;
double ratio=1;
double level=0;
double attackCoeff=0;
double releaseCoeff=0;
double gain=0;

DSP_EXPORT void processSample(double ioSample[])
{
	// compute average absolute value
	double absValue = 0;
	if (auxAudioInputsCount > 0)
	{
		for (uint channel = 0; channel < auxAudioInputsCount; channel++)
		{
			uint ch = channel + audioInputsCount;
			absValue += fabs(ioSample[ch]);
		}
		absValue /= double(audioInputsCount);
	}

	// update level
	if (absValue > level)
		level += attackCoeff*(absValue - level);
	else
		level += releaseCoeff*(absValue - level);

	if (level < 1e-10)
		level = 1e-10;

	// apply ratio if level below threshold
	gain = 1;
	if (level < threshold)
	{
		gain = (ratio*(level - threshold) + threshold) / level;
		if (gain < 0)
			gain = 0;
		for (uint channel = 0; channel < audioInputsCount; channel++)
		{
			ioSample[channel] *= gain;
		}
	}
}

DSP_EXPORT void updateInputParameters()
{
    threshold=pow(10,3*(inputParameters[0]-1)); // -60 to 0 dB
    ratio=(1+9*(exp(log(101.0)*inputParameters[1])-1)/100);
    attackCoeff=pow(10,1.0/(50+.5*sampleRate*inputParameters[2]))-1;
    releaseCoeff=pow(10,1.0/(50+.5*sampleRate*inputParameters[3]))-1;
}

DSP_EXPORT void computeOutputData()
{
    // display compression from 0 to 60 dB
    double compGain=1;
    if(gain>0)
    {
        compGain=20*log(gain);
        if(compGain<-60)
            compGain=-60;
        compGain/=-60;
    }
    outputParameters[0]=compGain;
}