/** \file
*   audio i/o router that includes aux inuts and outputs.
*   Select source channel for each audio output.
*/


// C++ scripting support-----------------------------
#include "dspapi.h"
#include "cpphelpers.h"

DSP_EXPORT double  sampleRate = 0;
DSP_EXPORT uint    audioInputsCount = 0;
DSP_EXPORT uint    audioOutputsCount = 0;
DSP_EXPORT uint    auxAudioInputsCount = 0; 
DSP_EXPORT uint    auxAudioOutputsCount = 0;

// extra headers
#include <string>

// metadata
DSP_EXPORT string name = "i/o router";
DSP_EXPORT string description = "Select source for each audio output";

// parameters definition: each parameter correponds to an audio output and lets you choose an input channel
DSP_EXPORT array<string>  inputParametersNames = {};
DSP_EXPORT array<double>  inputParameters = {};
DSP_EXPORT array<double>  inputParametersMin = {};
DSP_EXPORT array<double>  inputParametersMax = {};
DSP_EXPORT array<double>  inputParametersDefault = {};
DSP_EXPORT array<int>     inputParametersSteps = {};
DSP_EXPORT array<string>  inputParametersEnums = {};

// arrays that actually contain the strings
array<std::string> stdInputParametersNames;
array<std::string> stdInputParametersEnums;

array<int>      sourceChannel;
array<double>   temp;

DSP_EXPORT bool initialize()
{
	// initialize our arrays
	inputParametersNames.resize(audioOutputsCount + auxAudioOutputsCount);
	stdInputParametersNames.resize(audioOutputsCount + auxAudioOutputsCount);
	inputParameters.resize(audioOutputsCount + auxAudioOutputsCount);
	inputParametersMin.resize(audioOutputsCount + auxAudioOutputsCount);
	inputParametersMax.resize(audioOutputsCount + auxAudioOutputsCount);
	inputParametersDefault.resize(audioOutputsCount + auxAudioOutputsCount);
	inputParametersSteps.resize(audioOutputsCount + auxAudioOutputsCount);
	inputParametersEnums.resize(audioOutputsCount + auxAudioOutputsCount);
	stdInputParametersEnums.resize(audioOutputsCount + auxAudioOutputsCount);

	sourceChannel.resize(audioOutputsCount + auxAudioOutputsCount);
	temp.resize(audioInputsCount + auxAudioInputsCount + 1);
	temp[0] = 0;

	// initialize parameters properties (depend on the number of i/o channels)
	for (uint i = 0; i<inputParameters.length; i++)
	{
		inputParametersMin[i] = 0;
		inputParametersMax[i] = audioInputsCount + auxAudioInputsCount;
		inputParametersSteps[i] = audioInputsCount + auxAudioInputsCount + 1;

		if (i<audioOutputsCount)
			stdInputParametersNames[i] = "Out " + std::to_string(i + 1);
		else
			stdInputParametersNames[i] = "Aux Out " + std::to_string(i - audioOutputsCount + 1);
		inputParametersNames[i] = stdInputParametersNames[i].c_str(); // just points to std::string buffers

		if (i < audioInputsCount)
			inputParametersDefault[i] = i + 1;
		else
			inputParametersDefault[i] = 0;
		stdInputParametersEnums[i] = "None";
		for (uint ch = 0; ch<audioInputsCount+auxAudioInputsCount; ch++)
		{
			stdInputParametersEnums[i] += ";";
			if (ch<audioInputsCount)
				stdInputParametersEnums[i] += "Input " + std::to_string(ch + 1);
			else
				stdInputParametersEnums[i] += "Aux In " + std::to_string(ch - audioInputsCount + 1);

			inputParametersEnums[i] = stdInputParametersEnums[i].c_str();
		}
	}
	return true;
}

DSP_EXPORT void processSample(double ioSample[])
{
	// copy inputs to temp buffer
	for (uint ch = 0; ch<audioInputsCount+auxAudioInputsCount; ch++)
	{
		temp[ch + 1] = ioSample[ch];
	}

	// copy selected channels to outputs
	for (uint ch = 0; ch<audioOutputsCount+auxAudioOutputsCount; ch++)
	{
		ioSample[ch] = temp[sourceChannel[ch]];
	}
}

DSP_EXPORT void updateInputParametersForBlock(const TransportInfo* info)
{
	// copy source channels selection
	for (uint channel = 0; channel<audioOutputsCount+auxAudioOutputsCount; channel++)
	{
		sourceChannel[channel] = int(inputParameters[channel] + .5);
	}
}