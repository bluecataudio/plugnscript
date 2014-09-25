#include "../library/Constants.hxx"

string description="ring modulator";

/** Define our parameters.
*/
array<string> inputParametersNames={"Frequency","Mix"};
array<double> inputParameters(inputParametersNames.length);
array<double> inputParametersDefault={.5,.5};

// internal variables
int index=0;
double omega=.01;
int period=0;

void reset()
{
    index=0;
}

/** per-sample processing function: called for every sample with updated parameters values.
*
*/
void processSample(array<double>& ioSample)
{
    // compute value once
    double coeff=sin(omega*index);
    index++;
    index%=period;
    coeff=(1+(coeff-1)*inputParameters[1]); // dry/wet

    // multiply all channels
    for(uint channel=0;channel<audioInputsCount;channel++)
    {
        ioSample[channel]*=coeff;
    }
}

void updateInputParameters()
{
    omega=.001+inputParameters[0];
    period=int(2*PI/omega+.5);
}
