// C++ scripting support-----------------------------
#include "dspapi.h"
#include "cpphelpers.h"

DSP_EXPORT uint    audioInputsCount=0;
DSP_EXPORT string  userDocumentsPath=null;
DSP_EXPORT double  sampleRate=0;

/** \file
*   Simple wave file recorder.
*   This simple recorder writes the file directly in the audio thread, and may be sensitive to
*   system load. It may produce drop outs if used with small buffer sizes.
*/

#include "../library/WaveFile.h"

DSP_EXPORT string name="Wave file recorder";
DSP_EXPORT string author="Blue Cat Audio";
DSP_EXPORT string description="Records audio files to your documents folder";

DSP_EXPORT array<string> inputStringsNames={"File Path"};
DSP_EXPORT array<string> inputStrings(inputStringsNames.length);

DSP_EXPORT array<string> inputParametersNames={"Recording", "Rotation","Bit Depth","Volume"};
DSP_EXPORT array<string> inputParametersUnits={"", "Files","bit"};
DSP_EXPORT array<double> inputParameters(inputParametersNames.length);
DSP_EXPORT array<double> inputParametersMin={0,1,1};
DSP_EXPORT array<double> inputParametersDefault={0,5,2,1};
DSP_EXPORT array<double> inputParametersMax={3,20,5};
DSP_EXPORT array<int>    inputParametersSteps={4,20,5};
DSP_EXPORT array<string>  inputParametersEnums={"Auto;Stop;Pause;Resume","","8;16;24;32;64"};

DSP_EXPORT array<string> outputParametersNames={"Status"};
DSP_EXPORT array<double> outputParameters(outputParametersNames.length);
DSP_EXPORT array<double> outputParametersMin={0};
DSP_EXPORT array<double> outputParametersMax={2};
DSP_EXPORT array<string>  outputParametersEnums={"Stopped;Paused;Recording"};

WaveFileWriter  wavWriter;
bool            recording=false;
bool            paused=false;
int             fileIndex=0;
std::string     fileName;
double          amplitude=0;

bool convertToUnix(std::string& path)
{
    bool isUNC=(path.find("\\\\")==0); //do not touch UNC file paths on windows, starting with \\ - not supported on unix anyway.
    if(!isUNC)
    {
        // find all \ separators and replace them
        size_t index=path.find("\\");
        while(index!=std::string::npos)
        {
            path[index]='/';
            index=path.find("\\",index+1);
        }
    }
    return !isUNC;
}

DSP_EXPORT bool initialize()
{
    // store file header data
    wavWriter.header.sampleRate=uint64(sampleRate);
    wavWriter.header.channelsCount=audioInputsCount;
    // reserve space for file name string (avoids memory allocation)
    fileName.resize(1024);
    return true;
}

DSP_EXPORT void processSample(double ioSample[])
{
    // apply gain
    for(uint i=0;i<audioInputsCount;i++)
    {
        ioSample[i]*=amplitude;
    }
    // record to file
    if(recording && !paused)
        wavWriter.writeSample(ioSample);
}

DSP_EXPORT void updateInputParameters()
{
    amplitude=inputParameters[3];
}

DSP_EXPORT void updateInputParametersForBlock(const TransportInfo* transportInfo)
{
    // store current state
    bool wasRecording=recording;

    // update state from transport and parameters
    int mode=int(inputParameters[0]+.5);
    bool syncToHost=(mode==0);
    recording=false;
    paused=false;
    if(syncToHost && transportInfo!=null)
        recording=transportInfo->isPlaying;
    if(!syncToHost)
    {
        paused=(mode==2);
		recording = (mode == 3) || (paused);
	}
	int filesCount = int(inputParameters[1] + .5);

	// if recording stopped -> close file
	if (wasRecording && !recording)
	{
		wavWriter.close();
	}

	// if recording started -> open new file with appropriate bit depth and name
    if (!wasRecording && recording)
    {
        wavWriter.header.bytesPerSample = int(inputParameters[2] + .5);
        if (wavWriter.header.bytesPerSample == 5)
            wavWriter.header.bytesPerSample = 8;
        // generate file name    
        if (inputStrings[0] != null && strlen(inputStrings[0]) != 0)
        {
            // generate file path (may be relative or absolute)
            fileName = inputStrings[0];
            bool converted = convertToUnix(fileName); // convert to unix-style path, supported on all platforms
            if((fileName.find("/")!=0) && (fileName.find(":")==std::string::npos) && converted) // check if relative path
                fileName = userDocumentsPath + fileName;
        }
        else
        {
            fileName = userDocumentsPath;
            fileName += "audio-capture";
        }
        int extensionIndex = int(fileName.length()) - 4;
        if (fileName.rfind(".wav") == extensionIndex)
        {
            fileName.erase(extensionIndex, std::string::npos);
        }
        if (filesCount > 1)
            fileName += std::to_string(fileIndex + 1);
        fileName += ".wav";

        // open file
        wavWriter.openFile(fileName);

        // update current file index       
        fileIndex++;
        fileIndex %= filesCount;
    }
}

DSP_EXPORT void computeOutputData()
{
    if(paused)
        outputParameters[0]=1;
    else if(recording)
        outputParameters[0]=2;
    else
        outputParameters[0]=0;
}

DSP_EXPORT int getTailSize()
{
    return -1;
}