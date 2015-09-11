/** \file
*   Analog-style echo.
*   Produces an echo with a feedback loop and adjustable delay inertia.
*/

#include "../library/utils.hxx"

/*  Effect Description
*
*/
string name="Analog-Echo";
string description="analog style echo";

/* Parameters Description.
*/
array<string> inputParametersNames={"Mix","Delay","Feedback","Inertia"};
array<double> inputParameters(inputParametersNames.length);
array<double> inputParametersDefault={.5,.5,.6,.5};

/* Internal Variables.
*
*/
array<array<double>> buffers(audioInputsCount);
int maxDelay=int(sampleRate*1); // 1 second max delay
double delay=0;
double actualDelay=0;
double mix=0;
double feedback=0; 				
double delayCoeff=0;
int mask=0; 				
int currentIndex=0; 				

/* Initialization
*
*/
void initialize()
{
    // create buffer with 2^x size
    int lengthPow2=KittyDSP::Utils::nextPowerOfTwo(maxDelay);
    mask=lengthPow2-1;
    for(uint channel=0, count=buffers.length;channel<count;channel++)
    {
        buffers[channel].resize(lengthPow2);
    };
}

/* reset the state of the echo.
*
*/
void reset()
{
    // reset buffers with silence
    for(uint channel=0, count=buffers.length;channel<count;channel++)
    {
        // init with zeros
        for(uint i=0;i<buffers[channel].length;i++)
        {
            buffers[channel][i]=0;
        }
    };

    // reset delay value
    actualDelay=delay;
}

int getTailSize()
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
void processSample(array<double>& ioSample)
{
    // update delay time continuously
    actualDelay+=delayCoeff*(delay-actualDelay);
    uint integerDelay=uint(actualDelay);
    for(uint channel=0, count=audioInputsCount;channel<count;channel++)
    {
        array<double>@ channelBuffer=buffers[channel];
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

const double timeConstant=-log(10)/sampleRate;

/* update internal parameters from inputParameters array.
*   called every sample before processSample method or every buffer before process method
*/
void updateInputParameters()
{
    mix=inputParameters[0];
    delay=sampleRate*inputParameters[1];
    feedback=inputParameters[2];
    delayCoeff=1-exp(timeConstant/(.03+inputParameters[3])); // from .03 sec to 1 second for inertia
}
