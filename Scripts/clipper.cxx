/**  
*   Made by Michael Donovan 
*   (https://github.com/michaeldonovan)
*/

/** Script metadata.
*/
string name="Clipper";
string description="A very simple hard clipper";

/** Define our parameters.
*/
array<string> inputParametersNames={"Input", "Clip Level", "Output"};
array<string> inputParametersUnits={"dB", "", "dB"};
array<double> inputParameters(inputParametersNames.length);
array<double> inputParametersMin={-20, 0, -20};
array<double> inputParametersMax={20, 10, 20};
array<double> inputParametersDefault={0, 0 , 0};

/** Define our internal variables.
*
*/
double preGain=0;
double cliplevel=1;
double postGain=0;

/** per-sample processing function: called for every sample with updated parameters values.
*
*/
void processSample(array<double>& ioSample)
{  
    for(uint channel=0;channel<audioInputsCount;channel++)
    {
        double sample=ioSample[channel];

        sample*=preGain;

        if(sample>cliplevel)
            sample=cliplevel;
        else if(sample<-1*cliplevel)
            sample=-1*cliplevel;

        sample*=postGain;

        ioSample[channel]=sample;
    }
}

/** update internal parameters from inputParameters array.
*   called every sample before processSample method or every buffer before process method
*/
void updateInputParameters()
{
    preGain=pow(10, inputParameters[0]/20);
    cliplevel=pow(10, -2*inputParameters[1]/20);
    postGain=pow(10,inputParameters[2]/20);
}
