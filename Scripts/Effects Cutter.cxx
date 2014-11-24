#include "library/Midi.hxx"
#include "library/Constants.hxx"

string name="Effects Cutter";
string description="By Sreejesh";

array<string> outputParametersNames={"Effects"};
array<string> outputParametersUnits={""};
array<double> outputParameters(outputParametersNames.length,0);

array<string> inputParametersNames={"BleedGain"};
array<double> inputParameters(inputParametersNames.length);

array<double> outputParametersDefault={1};
array<double> outputParametersMax={1};
array<string> outputParametersEnums={"Open;Closed"};

double gain =0;

void processBlock(BlockData & data)

{
	outputParameters[0] = 0;
	if (!data.transport.isPlaying) {
		outputParameters[0] = 1;
		for (uint i = 0; i < data.samplesToProcess; i++) {
			for (uint ch = 0; ch < audioOutputsCount; ch++) {
				data.samples[ch][i] *= gain;
			}
		}
	}
}


void updateInputParameters()

{
   	gain=inputParameters[0];
}