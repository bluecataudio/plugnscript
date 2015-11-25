/** \file
*   Simple wave file recorder.
*   This simple recorder writes the file directly in the audio thread, and may be sensitive to
*   system load. It may produce drop outs if used with small buffer sizes.
*/

#include "../library/WaveFile.hxx"

string name="Wave file recorder";
string description="Records audio files to your documents folder";

array<string> inputStringsNames={"File Name"};
array<string> inputStrings(inputStringsNames.length);

array<string> inputParametersNames={"Recording", "Rotation","Bit Depth","Volume"};
array<string> inputParametersUnits={"", "Files","bit"};
array<double> inputParameters(inputParametersNames.length);
array<double> inputParametersMin={0,1,1};
array<double> inputParametersDefault={0,5,2,1};
array<double> inputParametersMax={3,20,5};
array<int>    inputParametersSteps={4,20,5};
array<string>  inputParametersEnums={"Auto;Stop;Pause;Resume","","8;16;24;32;64"};

array<string> outputParametersNames={"Status"};
array<double> outputParameters(outputParametersNames.length);
array<double> outputParametersMin={0};
array<double> outputParametersMax={2};
array<string>  outputParametersEnums={"Stopped;Paused;Recording"};

WaveFileWriter  wavWriter;
bool            recording=false;
bool            paused=false;
int             fileIndex=0;
string          fileName;
double          amplitude=0;

bool convertToUnix(string& path)
{
    bool isUNC=(path.findFirst("\\\\")==0); //do not touch UNC file paths on windows, starting with \\ - not supported on unix anyway.
    if(!isUNC)
    {
        // find all \ separators and replace them
        int index=path.findFirst("\\");
        while(index>=0)
        {
            path[index]='/';
            index=path.findFirst("\\",index+1);
        }
    }
    return !isUNC;
}

void initialize()
{
    // store file header data
    wavWriter.header.sampleRate=uint64(sampleRate);
    wavWriter.header.channelsCount=audioInputsCount;
    // reserve space for file name string (avoids memory allocation)
    fileName.resize(1024);
}

void processSample(array<double>& ioSample)
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

void updateInputParameters()
{
    amplitude=inputParameters[3];
}

void updateInputParametersForBlock(const TransportInfo@ transportInfo)
{
    // store current state
    bool wasRecording=recording;

    // update state from transport and parameters
    int mode=int(inputParameters[0]+.5);
    bool syncToHost=(mode==0);
    recording=false;
    paused=false;
    if(syncToHost && @transportInfo!=null)
        recording=transportInfo.isPlaying;
    if(!syncToHost)
    {
        paused=(mode==2);
        recording=(mode==3)||(paused);
    }
    int filesCount=int(inputParameters[1]+.5);

    // if recording stopped -> close file
    if(wasRecording && !recording)
    {
        wavWriter.close();
    }

    // if recording started -> open new file with appropriate bit depth and name
    if(!wasRecording && recording)
    {
        wavWriter.header.bytesPerSample=int(inputParameters[2]+.5);
        if(wavWriter.header.bytesPerSample==5)
            wavWriter.header.bytesPerSample=8;
        // generate file name    
        if(inputStrings[0].length!=0)
        {
            // generate file path (may be relative or absolute)
            fileName=inputStrings[0];
            bool converted=convertToUnix(fileName); // convert to unix-style path, supported on all platforms
            if((fileName.findFirst("/")!=0) && (fileName.findFirst(":")<0) && converted) // check if relative path
                fileName=userDocumentsPath+fileName;
        }
        else
            fileName=userDocumentsPath+"audio-capture";
        int extensionIndex=int(fileName.length)-4;
        if(fileName.findLast(".wav")==extensionIndex)
        {
            fileName.length=extensionIndex;
        }
        if(filesCount>1)
            fileName+=(fileIndex+1);
        fileName+=".wav";
        
        // open file
        wavWriter.openFile(fileName); 
        
        // update current file index       
        fileIndex++;
        fileIndex%=filesCount;
    }
}

void computeOutputData()
{
    if(paused)
        outputParameters[0]=1;
    else if(recording)
        outputParameters[0]=2;
    else
        outputParameters[0]=0;
}

int getTailSize()
{
    return -1;
}