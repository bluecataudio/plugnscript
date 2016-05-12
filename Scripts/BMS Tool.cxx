/** \file
*   Mid/Side Enhancer.
*   For stereo audio streams only (2 channels).
*/

string name="BMS Tool";
string description="Balance & Mid/Side Tool";
string author="James Cadwallader";

bool initialize()
{
    if(audioInputsCount!=2)
    {
        print("Error: this script requires 2 audio channels, not "+audioInputsCount);
        return false;
    }
    return true;
}

array<string> inputParametersNames={"Left","Right","Mid","Side"};
array<string> inputParametersUnits={"dB","dB","dB","dB"};
array<double> inputParameters(inputParametersNames.length);
array<double> inputParametersMin={-24,-24,-24,-24};
array<double> inputParametersMax={12,12,12,12};
array<double> inputParametersDefault={0,0,0,0};

double leftgain=0;
double rightgain=0;
double midgain=0;
double sidegain=0;

void processSample(array<double>& ioSample)
{
   ioSample[0]*=leftgain;
   ioSample[1]*=rightgain;
   double mid=ioSample[0];
   double side=ioSample[1];
   ioSample[0]=.5*(mid+side);
   ioSample[1]=mid-side;
   ioSample[0]*=midgain;
   ioSample[1]*=sidegain;
   double unmid=ioSample[0];
   double unside=ioSample[1];
   ioSample[0]=unmid+(.5*unside);
   ioSample[1]=unmid-(.5*unside);
}

void updateInputParameters()
{
   leftgain=pow(10,inputParameters[0]/20);
   rightgain=pow(10,inputParameters[1]/20);
   midgain=pow(10,inputParameters[2]/20);
   sidegain=pow(10,inputParameters[3]/20);
}
