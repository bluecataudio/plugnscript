/** \file
*   Cheap pink noise generator.
*   Generates pseudo pink noise from white noise, using a simplified fixed 3dB/Octave filter.
*/
string name="Pseudo Pink Noise";
string description="simple pseudo pink noise generator";

array<string> inputParametersNames={"Amplitude"};
array<double> inputParameters(inputParametersNames.length);
array<double> inputParametersDefault={.5};

double amplitude=0;
double b0=0;
double b1=0;
double b2=0;
double b3=0;
double b4=0;
double b5=0;
double b6=0;

void processSample(array<double>& ioSample)
{
    // using filter described here: http://www.musicdsp.org/showone.php?id=76
    double white=rand(-amplitude,amplitude);
    b0 = 0.99765 * b0 + white * 0.0990460;
    b1 = 0.96300 * b1 + white * 0.2965164;
    b2 = 0.57000 * b2 + white * 1.0526913;
    double pink = b0 + b1 + b2 + white * 0.1848;
    for(uint channel=0;channel<audioOutputsCount;channel++)
    {
        ioSample[channel]=pink;
    }
}

void updateInputParameters()
{
    // amplitude should be kept below .13 because of filtering
    amplitude=inputParameters[0]*.12;
}

int getTailSize()
{
    return -1;
}