string name="Latency Reporter";
string description="Reports chosen latency to host";

array<string> inputParametersNames={"Latency"};
array<string> inputParametersUnits={"Samp"};
array<double> inputParametersMin={0};
array<double> inputParametersMax={1024};
array<double> inputParametersDefault={0};
array<int>    inputParametersSteps={1025};
array<double> inputParameters(inputParametersNames.length);

int getLatency()
{
    return int(inputParameters[0]+.5);
}