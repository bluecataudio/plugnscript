string description="gain (dB)";

/** Define our input parameters.
*/
array<string> inputParametersNames={"Gain"};
array<string> inputParametersUnits={"dB"};
array<double> inputParameters(inputParametersNames.length);
array<double> inputParametersMin={-20};
array<double> inputParametersMax={20};
array<double> inputParametersDefault={0};

/** Define our internal variables.
*
*/
double gain=0;

/** per-sample processing function: called for every sample with updated parameters values.
*
*/
void processSample(array<double>& ioSample)
{
   for(uint channel=0;channel<audioInputsCount;channel++)
   {
      ioSample[channel]*=gain;
   }
}

/** update internal parameters from inputParameters array.
*   called every sample before processSample method
*/
void updateInputParameters()
{
   /*using reverse dB formula: gain=10^(gaindB/20)*/
   gain=pow(10,inputParameters[0]/20);
}