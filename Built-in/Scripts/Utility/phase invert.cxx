/** Define our parameters.
*/
array<string> inputParametersNames={"Invert"};
array<string> inputParametersEnums={"Off;On"};
array<double> inputParameters(inputParametersNames.length);
array<int> inputParametersSteps={2};
array<double> inputParametersMin={0};
array<double> inputParametersMax={1};

string description="signal phase inverter";
double norm=1;

/** per-sample processing function: called for every sample with updated parameters values.
*
*/
void processSample(array<double>& ioSample)
{
    for(uint i=0;i<audioInputsCount;i++)
    {
        ioSample[i]*=norm;
    }
}

void updateInputParameters()
{
    norm=-2*inputParameters[0]+1;
}