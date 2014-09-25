string name="Pseudo White Noise";
string description="simple pseudo white noise generator";

array<string> inputParametersNames={"Amplitude"};
array<double> inputParameters(inputParametersNames.length);
array<double> inputParametersDefault={.5};

double amplitude=0;

void processSample(array<double>& ioSample)
{
   for(uint channel=0;channel<audioOutputsCount;channel++)
   {
      ioSample[channel]=rand(-amplitude,amplitude);
   }
}

void updateInputParameters()
{
   amplitude=inputParameters[0];
}

int getTailSize()
{
    return -1;
}