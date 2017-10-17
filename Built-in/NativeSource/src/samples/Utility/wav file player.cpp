// C++ scripting support-----------------------------
#include "dspapi.h"
#include "cpphelpers.h"

DSP_EXPORT uint    audioOutputsCount=0;
DSP_EXPORT string  userDocumentsPath=null;

/** \file
*   Simple wave file player.
*   This simple player reads the file directly in the audio thread, and may be sensitive to
*   system load. It may produce drop outs if used with small buffer sizes.
*/

#include "../library/WaveFile.h"

DSP_EXPORT string name="Wave File Player";
DSP_EXPORT string author="Blue Cat Audio";
DSP_EXPORT string description="Plays a wave file from your Documents";

DSP_EXPORT array<string> inputStringsNames={"File Path"};
DSP_EXPORT array<string> inputStrings(inputStringsNames.length);

DSP_EXPORT array<string> inputParametersNames={"Play","Volume"};
DSP_EXPORT array<string> inputParametersUnits={"","%"};
DSP_EXPORT array<double> inputParameters(inputParametersNames.length);
DSP_EXPORT array<double> inputParametersMin={0};
DSP_EXPORT array<double> inputParametersDefault={0};
DSP_EXPORT array<double> inputParametersMax={3};
DSP_EXPORT array<int>    inputParametersSteps={4};
DSP_EXPORT array<string>  inputParametersEnums={"Auto;Stop;Pause;Resume"};

DSP_EXPORT array<string> outputParametersNames={"Status"};
DSP_EXPORT array<double> outputParameters(outputParametersNames.length);
DSP_EXPORT array<double> outputParametersMin={0};
DSP_EXPORT array<double> outputParametersMax={2};
DSP_EXPORT array<string>  outputParametersEnums={"Stopped;Paused;Playing"};

WaveFileReader  wavReader;
uint            channelsToPlay=0;
bool            playing=false;
bool            paused=false;
double          amplitude=0;
std::string     fileName; // file name as entered
std::string     filePath; // full file path (user documents folder + file name)

bool convertToUnix(std::string& path)
{
	bool isUNC = (path.find("\\\\") == 0); //do not touch UNC file paths on windows, starting with \\ - not supported on unix anyway.
	if (!isUNC)
	{
		// find all \ separators and replace them
		size_t index = path.find("\\");
		while (index != std::string::npos)
		{
			path[index] = '/';
			index = path.find("\\", index + 1);
		}
	}
	return !isUNC;
}

DSP_EXPORT bool initialize()
{
    // reserve space for file name string (avoids memory allocation)
    fileName.resize(1024);
    filePath.resize(1024);
    return true;
}

DSP_EXPORT void processSample(double ioSample[])
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

DSP_EXPORT void updateInputParameters()
{
    // amplitude is updated for each sample, to avoid steps
   amplitude=inputParameters[1];
}

DSP_EXPORT void updateInputParametersForBlock(const TransportInfo* transportInfo)
{
    // store current state
    bool wasPlaying=playing;

    // update state from transport and parameters
    int mode=int(inputParameters[0]+.5);
    bool syncToHost=(mode==0);
    playing=false;
    paused=false;
    if(syncToHost && transportInfo!=null)
        playing=transportInfo->isPlaying;
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
        // generate file path (may be relative or absolute)
        filePath=fileName;
        bool converted=convertToUnix(filePath); // convert to unix-style path, supported on all platforms
        if((filePath.find("/")!=0) && (filePath.find(":")==std::string::npos) && converted) // check if relative path
            filePath=userDocumentsPath+filePath;
        
        // open file and store the number of channels to play
        channelsToPlay=0;
        bool ok=wavReader.openFile(filePath,audioOutputsCount);
        if(ok)
        {
            channelsToPlay=wavReader.get_channelsCount();
            if(channelsToPlay>audioOutputsCount)
                channelsToPlay=audioOutputsCount;
        }
    }
}

DSP_EXPORT void computeOutputData()
{
    if(paused)
        outputParameters[0]=1;
    else if(playing && channelsToPlay!=0)
        outputParameters[0]=2;
    else
        outputParameters[0]=0;
}

DSP_EXPORT int getTailSize()
{
    return -1;
}