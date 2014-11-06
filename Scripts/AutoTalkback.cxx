#include "library/Midi.hxx"
#include "library/Constants.hxx"

string name="Auto TalkBack";
string description="By Sreejesh";

array<string> outputParametersNames={"Talkback"};
array<string> outputParametersUnits={""};
array<double> outputParameters(outputParametersNames.length,0);

array<double> outputParametersDefault={0};
array<double> outputParametersMax={1};
array<string> outputParametersEnums={"Open;Closed"};

double gain =1;

void processBlock(BlockData & data)

{
	outputParameters[0] = 0;
	if (data.transport.isPlaying) {
		gain = 0;
		outputParameters[0] = 1;
		for (uint i = 0; i < data.samplesToProcess; i++) {
			for (uint ch = 0; ch < audioOutputsCount; ch++) {
				data.samples[ch][i] *= gain;
			}
		}
	}
}