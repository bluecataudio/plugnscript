#include "dspapi.h"
#include "cpphelpers.h"

// expected from host (Warning: appropriate values are filled before calling
// the initialize() function - do not use before)
DSP_EXPORT uint    audioInputsCount=0;
DSP_EXPORT uint    audioOutputsCount=0;
DSP_EXPORT uint    auxAudioInputsCount=0;
DSP_EXPORT uint    auxAudioOutputsCount=0;
DSP_EXPORT int     maxBlockSize=0;
DSP_EXPORT double  sampleRate=0;
DSP_EXPORT string  userDocumentsPath=null;
DSP_EXPORT string  scriptFilePath=null;
DSP_EXPORT void*   host=null;
DSP_EXPORT HostPrintFunc* hostPrint=null;

// Script metadata----------------------------------------
/// The name of the script to be displayed in the plug-in.
DSP_EXPORT string const name="Script Name";
/// The short description of the script to be displayed in the plug-in.
DSP_EXPORT string description="Script Description";

// Script I/O parameters and strings and associated metadata----------
/// An array of parameters to be used as input for the script.
/// Will be displayed in the user interface of the plug-in and accessible for 
/// automation and MIDI control.
DSP_EXPORT array<double> inputParameters(2); 
/// Names to be displayed in the plug-in for the input parameters.
string names[]={"P1","P2"};
DSP_EXPORT array<string> inputParametersNames(names);
/// Units for the corresponding input parameters.
DSP_EXPORT array<string> inputParametersUnits{"dB","%"};
/// Enumeration values for the corresponding input parameters.
/// Array of strings containing semicolon separated values. Use empty strings
/// for non-enum parameters. Require that min and max values are defined.
DSP_EXPORT array<string> inputParametersEnums={"value1;value2",""};
/// Minimum values for the corresponding input parameters. Default value is 0.
DSP_EXPORT array<double> inputParametersMin={0, 0};
/// Maximum values for the corresponding input parameters. Default value is 1.
DSP_EXPORT array<double> inputParametersMax={10,20};
/// Default values for the corresponding input parameters. Default value is 0.
DSP_EXPORT array<double> inputParametersDefault={5,0};
/// Number of steps for the corresponding input parameters.
/// Default is -1 (continuous control - no steps).
DSP_EXPORT array<int>    inputParametersSteps={10,20};

/// An array of strings to be used as input for the script.
/// Will be displayed in the user interface of the plug-in
DSP_EXPORT array<string> inputStrings(2);
/// Names to be displayed in the plug-in for the input strings.
DSP_EXPORT array<string> inputStringsNames={"S1","S2"};

/// An array of parameters to be used as output of the script. 
/// Will be displayed in the user interface of the plug-in as meters and accessible 
/// to generate automation and MIDI controllers.
DSP_EXPORT array<double> outputParameters(2);
/// Names to be displayed in the plug-in for the output parameters.
DSP_EXPORT array<string> outputParametersNames={"OUT 1"," OUT 2"};
/// Units for the corresponding input parameters.
DSP_EXPORT array<string> outputParametersUnits={"dB","dB"};
/// Enumeration values for the corresponding input parameters.
/// Array of strings containing semicolon separated values. Use empty strings
/// for non-enum parameters. Require that min and max values are defined.
DSP_EXPORT array<string> outputParametersEnums={"value1;value2",""};
/// Minimum values for the corresponding input parameters. Default value is 0.
DSP_EXPORT array<double> outputParametersMin={0, 0};
/// Maximum values for the corresponding input parameters. Default value is 1.
DSP_EXPORT array<double> outputParametersMax={10,20};
/// Default values for the corresponding input parameters. Default value is 0.
DSP_EXPORT array<double> outputParametersDefault={5,0};

/** Initialization: called right after the script has been compiled
*   and before any other processing occurs.
*   return false if initialization fails (for example if the number of 
*   audio channels or the sample rate are not compatible).
*   When returning false, it is strongly advised to print a message with
*   the "print" function for the end user using the print function.
*/
DSP_EXPORT bool initialize()
{
    /// write script initialization here
    /// and return false if something is not supported (number of channels, sample rate...)
    return true;
}

/** Reset the state of the filter.
*
*/
DSP_EXPORT void reset()
{
}

/** Returns the tail size in samples. 
*   use -1 for infinite (typically for audio or MIDI generators or synths).
*   use 0 if the processor does not produce any sound when fed with silence (default).
*/
DSP_EXPORT int getTailSize()
{
    return 0;
}

/** Returns the latency added by the script if any, in samples.
*   Returns 0 by default.
*/
DSP_EXPORT int getLatency()
{
    return 0;
}

/** Per-sample processing function: called for every sample with updated parameters values.
*   If defined, the processBlock function is ignored.
*   ioSample: an array of current audio samples (one element for each audio channel).
* You can access the current sample of channel 'ch' using ioSample[ch].
*/
/*DSP_EXPORT void processSample(double ioSample[])
{
    
}*/

/** Update internal parameters from inputParameters array when required.
*   Called every sample, right before the processSample() method, or every block, 
*   before the processBlock() method.
*   This function will not be called if input parameters have not been modified since last call.
*/
DSP_EXPORT void updateInputParameters()
{
}

/** Per-block processing function: called for every block with updated parameters values.
*   Ignored if the processSample() function is defined.
*/
DSP_EXPORT void processBlock(BlockData& data)
{
 
}

/** Called for every block to update internal parameters from the inputParameters and
*   inputStrings arrays that have been updated by the host.
*   This function will not be called if input parameters have not been modified since last call,
*   and if transport info (tempo and time signature only) are unchanged.
*/
DSP_EXPORT void updateInputParametersForBlock(const TransportInfo* info)
{
}

/** Update output parameters values array from internal variables.
*   
*/
DSP_EXPORT void computeOutputData()
{
}

/** Cleanup on shutdown: release all resources (allocated memory, files, system handles...).
 *
 */
DSP_EXPORT void shutdown()
{
    
}
