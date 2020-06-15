#include "../library/WaveFile.hxx"

string name="Sample Player";
string description="Plays an audio sample loaded from the script data folder (sample script).";

array<string> inputParametersNames={"Play"};
array<double> inputParameters(inputParametersNames.length);
array<double> inputParametersMin={0};
array<double> inputParametersDefault={0};
array<double> inputParametersMax={1};
array<int>    inputParametersSteps={2};
array<string>  inputParametersEnums={"Pause;Play"};

WaveFileReader  wavReader;
uint            channelsToPlay=0;
bool            playing=false;
bool initialize()
{
    // load sample file
    string filePath=scriptDataPath + "/audio.wav";
    bool ok=wavReader.openFile(filePath,audioOutputsCount);
    if(ok)
    {
        channelsToPlay=wavReader.channelsCount;
        if(channelsToPlay>audioOutputsCount)
            channelsToPlay=audioOutputsCount;
    }
    return ok;
}

void processSample(array<double>& ioSample)
{
    if(playing)
    {
        // read samples
        wavReader.readSample(ioSample);

        // silence unused channels
        for(uint ch=channelsToPlay;ch<audioOutputsCount;ch++)
        {
            ioSample[ch]=0;;
        }   
    }

    // loop back to beginning if end of file was reached
    if(wavReader.isEndOfFile())
        wavReader.setPos(0);
}

void updateInputParameters()
{
    playing=(inputParameters[0]!=0);
}

int getTailSize()
{
    // infinite tail, as we generate audio data whatever the input
    return -1;
}