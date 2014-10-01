/** \file
*   Sine wave generator.
*   Generates a sine wave using the sin() function.
*/

#include "../library/Constants.hxx"

string name="Sine Wave";
string description="Sine wave generator";

array<string> inputParametersNames={"Amplitude", "Frequency"};
array<string> inputParametersUnits={"%", "Hz"};
array<double> inputParameters(inputParametersNames.length);
array<double> inputParametersMin={0,0};
array<double> inputParametersDefault={50,200};
array<double> inputParametersMax={100,5000};

double amplitude=0;
double omega=0;
double currentPhase=0;
const double period=2*PI;
const double omegaFactor=period/sampleRate;

void processSample(array<double>& ioSample)
{
    // compute sample value
    double sampleValue=amplitude*sin(currentPhase);
        
    // update phase
    currentPhase+=omega;
        
    // copy to all audio outputs
    for(uint channel=0;channel<audioOutputsCount;channel++)
    {
        ioSample[channel]=sampleValue;
    }
    
    // to avoid overflow, reduce phase
    while(currentPhase>period)
        currentPhase-=period;
}

void updateInputParameters()
{
   amplitude=inputParameters[0]*.01;
   omega=omegaFactor*inputParameters[1];
}

int getTailSize()
{
    return -1;
}