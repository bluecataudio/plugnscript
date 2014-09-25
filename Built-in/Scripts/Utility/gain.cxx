string description="simple volume control";

array<string> inputParametersNames={"Gain"};
array<double> inputParameters(inputParametersNames.length);

double gain=0;

void processSample(array<double>& ioSample)
{
   for(uint channel=0;channel<audioInputsCount;channel++)
   {
      ioSample[channel]*=gain;
   }
}

void updateInputParameters()
{
   gain=inputParameters[0];
}