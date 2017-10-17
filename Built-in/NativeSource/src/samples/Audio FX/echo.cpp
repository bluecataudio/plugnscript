// C++ scripting support-----------------------------
#include "dspapi.h"
#include "cpphelpers.h"

DSP_EXPORT double  sampleRate=0;
DSP_EXPORT uint    audioInputsCount=0;

// extra system headers
#include <math.h>

/** \file
*   Analog-style echo.
*   Produces an echo with a feedback loop and adjustable delay inertia.
*/

#include "../library/utils.h"

/*  Effect Description
*
*/
DSP_EXPORT string name="Analog-Echo";
DSP_EXPORT string author="Blue Cat Audio";
DSP_EXPORT string description="analog style echo";

/* Parameters Description.
*/
DSP_EXPORT array<string> inputParametersNames={"Mix","Delay","Feedback","Inertia"};
DSP_EXPORT array<double> inputParameters(inputParametersNames.length);
DSP_EXPORT array<double> inputParametersDefault={.5,.5,.6,.5};

/* Internal Variables.
*
*/
double** buffers=NULL;
uint buffersLength=0;
int maxDelay=0;
double delay=0;
double actualDelay=0;
double mix=0;
double feedback=0; 				
double delayCoeff=0;
int mask=0; 				
int currentIndex=0; 				
double timeConstant=0;

/* Initialization
*
*/
DSP_EXPORT bool initialize()
{
    // initialize variables
    maxDelay=int(sampleRate*1); // 1 second max delay
    timeConstant=-log(10.0)/sampleRate;
    
    // allocate buffers array
    if(audioInputsCount>0)
    {
        buffers= new double*[audioInputsCount];
    }
    
    // create buffers with 2^x size
    buffersLength=KittyDSP::Utils::nextPowerOfTwo(maxDelay);
    mask=buffersLength-1;
    for(uint channel=0, count=audioInputsCount;channel<count;channel++)
    {
        if(buffersLength>0)
            buffers[channel]= new double[buffersLength];
        else
            buffers[channel]=null;
    };
    return true;
}

/** cleanup allocated resources
 *
 */
DSP_EXPORT void shutdown()
{
    for(uint channel=0, count=audioInputsCount;channel<count;channel++)
    {
        delete []  buffers[channel];
    };
    delete [] buffers;
    buffers=null;
}

/* reset the state of the echo.
*
*/
DSP_EXPORT void reset()
{
    // reset buffers with silence
    for(uint channel=0, count=audioInputsCount;channel<count;channel++)
    {
        // init with zeros
        for(uint i=0;i<buffersLength;i++)
        {
            buffers[channel][i]=0;
        }
    };

    // reset delay value
    actualDelay=delay;
}

DSP_EXPORT int getTailSize()
{
    // limit feedback (to avoid infinite tail size)
    if(feedback>.9)
        feedback=.9;

    // decayed value is (feedback^n) => for 60 dB attenuation, exp(-60/20)=f^n => n=-3/log(f)
    int tail=int(-3*double(maxDelay)/log(feedback));
    return tail;
}

/* per-sample processing function: called for every sample with updated parameters values.
*
*/
DSP_EXPORT void processSample(double ioSample[])
{
    // update delay time continuously
    actualDelay+=delayCoeff*(delay-actualDelay);
    uint integerDelay=uint(actualDelay);
    for(uint channel=0, count=audioInputsCount;channel<count;channel++)
    {
        double* channelBuffer=buffers[channel];
        double input=ioSample[channel];

        // compute output
        double output=input+feedback*channelBuffer[(currentIndex-1-integerDelay)&mask];

        // update buffer
        channelBuffer[currentIndex]=output;

        // copy to output with mix
        ioSample[channel]+=mix*(output-input);
    }
    currentIndex++;
    currentIndex&=mask;
}

/* update internal parameters from inputParameters array.
*   called every sample before processSample method or every buffer before process method
*/
DSP_EXPORT void updateInputParameters()
{
    mix=inputParameters[0];
    delay=sampleRate*inputParameters[1];
    feedback=inputParameters[2];
    delayCoeff=1-exp(timeConstant/(.03+inputParameters[3])); // from .03 sec to 1 second for inertia
}
