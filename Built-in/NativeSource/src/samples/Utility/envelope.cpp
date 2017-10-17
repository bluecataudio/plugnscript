// C++ scripting support-----------------------------
#include "dspapi.h"
#include "cpphelpers.h"
#include <math.h>

// variables filled by host
DSP_EXPORT uint    audioInputsCount = 0;
DSP_EXPORT double  sampleRate = 0;

DSP_EXPORT string name="Envelope";
DSP_EXPORT string author="Blue Cat Audio";

// Input Parameters
DSP_EXPORT array<string> inputParametersNames={"Scale","Decay"};
DSP_EXPORT array<double> inputParametersDefault={1.0,.3};
DSP_EXPORT array<double> inputParametersMax={1.0};
DSP_EXPORT array<int> inputParametersSteps={2,-1};
DSP_EXPORT array<string> inputParametersEnums={"Linear;Log(dB)"};
DSP_EXPORT array<double> inputParameters(inputParametersNames.length);

// Output parameters definition
DSP_EXPORT array<string> outputParametersNames={"Level"};
DSP_EXPORT array<string> outputParametersUnits={"%"};
DSP_EXPORT array<double> outputParameters(outputParametersNames.length);
DSP_EXPORT array<double> outputParametersMin={0};
DSP_EXPORT array<double> outputParametersMax={1};

// meter decay
double decay=0;

// the measured level
double level=0;

/* per-sample processing.
 *
 */
DSP_EXPORT void processSample(double ioSample[])
{
    double maxValue=0;
    for(uint channel=0;channel<audioInputsCount;channel++)
    {
        double value=fabs(ioSample[channel]);
        if(value>maxValue)
            maxValue=value;
    }
    if(maxValue>level)
        level=maxValue;
    else
        level+=decay*(maxValue-level);
}

DSP_EXPORT int getTailSize()
{
    // 1000 milliseconds to let meters reach -60 dB
    return int(1*sampleRate);
}

DSP_EXPORT void reset()
{
    // reset level to 0
    level=0;
}

DSP_EXPORT void updateInputParametersForBlock(const TransportInfo* info)
{
    decay=(1-exp(-log(10.0)/(sampleRate*.1*(.01+5*inputParameters[1]))));
}

DSP_EXPORT void computeOutputData()
{
    // linear mode
    if(inputParameters[0]<.5)
        outputParameters[0]=level;
    else
    {
        // Log scale (dB)
        if(level>pow(10.0,-3)) // -60 dB limit
            outputParameters[0]=(20.0*log10(level)+60.0)/60.0;
        else
            outputParameters[0]=0;
    }
}