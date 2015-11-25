/** \file
 *   Transport info monitor.
 *   Display transport info (tempo, beat, playhead status etc.).
 */
#include "../library/Midi.hxx"

// metadata
string name="Transport Monitor";
string description="Displays transport information";

// output parameters definition
array<string> outputParametersNames={"Tempo", "Sig. Top", "Sig. Bottom", "Beat", "Playing", "Recording", "Looping","Position","Beat Pos."};
array<string> outputParametersUnits={"bpm", "", "", "", "", "", "","sec.","beats"};
array<double> outputParameters(outputParametersNames.length,0);

array<double> outputParametersMin={0,0,0,0,0,0,0,0};
array<double> outputParametersMax={400,24,24,24,1,1,1,100000000,100000000};
array<double> outputParametersDefault={0,0,0,0,0,0,0,0};
array<string> outputParametersEnums={"","","","","No;Yes","No;Yes","No;Yes",""};

void processBlock(BlockData& data)
{
    // storing transport info if available
    if(@data.transport!=null)
    {
        outputParameters[0]=data.transport.bpm;
        outputParameters[1]=double(data.transport.timeSigTop);
        outputParameters[2]=double(data.transport.timeSigBottom);
        outputParameters[3]=data.transport.positionInQuarterNotes;
        if(data.transport.currentMeasureDownBeat>0)
            outputParameters[3]-=data.transport.currentMeasureDownBeat;
        if(data.transport.timeSigBottom!=0)
            outputParameters[3]*=data.transport.timeSigBottom/4.0;
        outputParameters[3]+=1;
        outputParameters[4]=0;
        if(data.transport.isPlaying)
            outputParameters[4]=1;
        outputParameters[5]=0;
        if(data.transport.isRecording)
            outputParameters[5]=1;
        outputParameters[6]=0;
        if(data.transport.isLooping)
            outputParameters[6]=1;
        outputParameters[7]=data.transport.positionInSeconds;
        // sometimes first beat may be off (there is no convention regarding the position of the first beat)
        int measuresCount=0;
        if(data.transport.timeSigTop!=0)
            measuresCount=int(data.transport.currentMeasureDownBeat*data.transport.timeSigBottom/4.0+.001)/int(data.transport.timeSigTop+.001);
        outputParameters[8]=outputParameters[3]+double(measuresCount)*data.transport.timeSigTop;
    }
}