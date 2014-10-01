/** \file
*   Tremolo effect.
*   Classic tremolo effect (amplitude modulation).
*/

#include "../library/Constants.hxx"

string name="Tremolo";
string description="tremolo effect";

/* Define our parameters.
*/
array<string>  inputParametersNames={"Rate","Mix"};
array<double> inputParameters(inputParametersNames.length); 
array<double> inputParametersDefault={.5,.5,.6,.5};

// internal variables
double currentPhase=0;
double omega=0;
double mix=0;

// constants
const double maxOmega=10*2*PI/sampleRate; // up to 10 hz
const double period=2*PI;

/* per-sample processing function: called for every sample.
*
*/
void processSample(array<double>& ioSample)
{
    // compute amplitude value once
    double coeff=.5*(1+sin(currentPhase));
    coeff=(1+(coeff-1)*mix); // apply dry-wet

    // multiply all channels
    for(uint channel=0;channel<audioInputsCount;channel++)
    {
        ioSample[channel]*=coeff;
    }
    
    // update phase
    currentPhase+=omega;
    if(currentPhase>period)
        currentPhase-=period;
}

void updateInputParameters()
{
    omega=maxOmega*inputParameters[0];
    mix=inputParameters[1];
}

void reset()
{
    currentPhase=0;
}
