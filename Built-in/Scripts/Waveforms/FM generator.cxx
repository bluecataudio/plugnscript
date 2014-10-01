/** \file
*   Freequency Modulation (FM) waveform generator.
*   Generates an audio waveform using frequency modulation on a sine wave.
*/

#include "../library/Constants.hxx"

string name="FM Generator";
string description="Frequency Modulation generator";

array<string> inputParametersNames={"Amplitude", "Frequency","FM Index","Ratio"};
array<string> inputParametersUnits={"%", "Hz","",""};
array<double> inputParameters(inputParametersNames.length);
array<double> inputParametersMin={0,0,-1,0};
array<double> inputParametersDefault={50,200,0};
array<double> inputParametersMax={100,5000,-1,10};

double amplitude=0;
double omega=0;
double currentPhase=0;

double modulatorOmega=0;
double modulatorCurrentPhase=0;
double modulationIndex=0;

const double period=2*PI;
const double omegaFactor=period/sampleRate;

void processSample(array<double>& ioSample)
{
    // compute sample value
    double sampleValue=amplitude*sin(currentPhase+modulationIndex*sin(modulatorCurrentPhase));
        
    // update phase
    currentPhase+=omega;
    modulatorCurrentPhase+=modulatorOmega;
        
    // copy to all audio outputs
    for(uint channel=0;channel<audioOutputsCount;channel++)
    {
        ioSample[channel]=sampleValue;
    }
    
    // to avoid overflow, reduce phase
    while(currentPhase>period)
        currentPhase-=period;
    while(modulatorCurrentPhase>period)
        modulatorCurrentPhase-=period;

}

void updateInputParameters()
{
   amplitude=inputParameters[0]*.01;
   omega=omegaFactor*inputParameters[1];
   modulationIndex=10*inputParameters[2];
   modulatorOmega=omega*inputParameters[3];
}

int getTailSize()
{
    return -1;
}