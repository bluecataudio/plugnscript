/** \file
*   Simple wave file player.
*   This simple player reads the file directly in the audio thread, and may be sensitive to
*   system load. It may produce drop outs if used with small buffer sizes.
*/

#include "../library/WaveFile.hxx"

string name="Wave File Player";
string description="plays a wave file from your Documents";

array<string> inputStringsNames={"File Name"};
array<string> inputStrings(inputStringsNames.length);

array<string> inputParametersNames={"Play","Volume"};
array<string> inputParametersUnits={"","%"};
array<double> inputParameters(inputParametersNames.length);
array<double> inputParametersMin={0};
array<double> inputParametersDefault={0};
array<double> inputParametersMax={3};
array<int>    inputParametersSteps={4};
array<string>  inputParametersEnums={"Auto;Stop;Pause;Resume"};

array<string> outputParametersNames={"Status"};
array<double> outputParameters(outputParametersNames.length);
array<double> outputParametersMin={0};
array<double> outputParametersMax={2};
array<string>  outputParametersEnums={"Stopped;Paused;Playing"};

WaveFileReader  wavReader;
uint            channelsToPlay=0;
bool            playing=false;
bool            paused=false;
double          amplitude=0;
string          fileName; // file name as entered
string          filePath; // full file path (user documents folder + file name)

void initialize()
{
    // reserve space for file name string (avoids memory allocation)
    fileName.resize(1024);
    filePath.resize(1024);
}

void processSample(array<double>& ioSample)
{
    if(playing)
    {
        if(paused)
        {
            // silence all channels
            for(uint ch=0;ch<audioOutputsCount;ch++)
            {
                ioSample[ch]=0;;
            }   
        }
        else
        {
            // read samples
            wavReader.readSample(ioSample);

            // apply gain
            for(uint ch=0;ch<channelsToPlay;ch++)
            {
                ioSample[ch]*=amplitude;
            }
            // silence unused channels
            for(uint ch=channelsToPlay;ch<audioOutputsCount;ch++)
            {
                ioSample[ch]=0;;
            }   
        }
    }

    // loop back to beginning if end of file was reached
    if(wavReader.isEndOfFile())
        wavReader.setPos(0);
}

void updateInputParameters()
{
    // amplitude is updated for each sample, to avoid steps
   amplitude=inputParameters[1];
}

void updateInputParametersForBlock(const TransportInfo@ transportInfo)
{
    // store current state
    bool wasPlaying=playing;

    // update state from transport and parameters
    int mode=int(inputParameters[0]+.5);
    bool syncToHost=(mode==0);
    playing=false;
    paused=false;
    if(syncToHost && @transportInfo!=null)
        playing=transportInfo.isPlaying;
    if(!syncToHost)
    {
        paused=(mode==2);
        playing=(mode==3)||(paused);
    }

    // if playing stopped -> close file
    if(wasPlaying && !playing)
    {
        wavReader.close();
    }

    // store file name if different
    if(fileName!=inputStrings[0])
    {
        fileName=inputStrings[0];
        wasPlaying=false; // force file re-open
    }
    
    // if playing started -> open new file
    if(!wasPlaying && playing)
    {
        // generate file name    
        filePath=userDocumentsPath;
        filePath+=fileName;
        
        // open file and store the number of channels to play
        channelsToPlay=0;
        bool ok=wavReader.openFile(filePath,audioOutputsCount);
        if(ok)
        {
            channelsToPlay=wavReader.channelsCount;
            if(channelsToPlay>audioOutputsCount)
                channelsToPlay=audioOutputsCount;
        }
    }
}

void computeOutputData()
{
    if(paused)
        outputParameters[0]=1;
    else if(playing)
        outputParameters[0]=2;
    else
        outputParameters[0]=0;
}

int getTailSize()
{
    return -1;
}