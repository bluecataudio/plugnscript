// C++ scripting support-----------------------------
#include "dspapi.h"
#include "cpphelpers.h"

#include <string>

/** \file persistent state.cpp
* A stupid simple example that shows how to make the DSP script state persistent.
* The DSP script counts the number of samples elapsed since it was initialized and stores it into
* the persistent state so that it gets restored upon preset or session reload.
*/

DSP_EXPORT string name = "Persistency Test";
DSP_EXPORT string description = "saves current samples count into persistent state";

DSP_EXPORT array<string> outputStringsNames = { "Elapsed Samples" };
DSP_EXPORT array<string> outputStrings(outputStringsNames.length,NULL);


bool changed=true;
int samplesCount=0;

// the internal state string
// since pointer is returned to engine during saveState,
// it has to be still valid until next function call
// in this example it has a fixed size and can be allocated once for all
char stateString[256];

// output string
char outputString[256];

DSP_EXPORT void processSample(array<double>& ioSample)
{
    samplesCount++;
    changed=true;
}

DSP_EXPORT int getTailSize()
{
    return -1;
}

DSP_EXPORT bool saveState(string& persistentState)
{
    // write only if changed
    bool hasChanged=changed;
    if(changed)
    {
        changed=false;
        // save a  uniqueID string first, so that we can check state upon restore
        snprintf(stateString,18,"%s","MyScriptUniqueID;");
        // save the samples count
        snprintf(stateString+17,256-17,"%d",samplesCount);
        // return pointer to string
        persistentState = stateString;
    }
    return hasChanged;
}

DSP_EXPORT void loadState(const string& persistentState)
{
    // check if this is a valid state with our ID to begin with
    if (persistentState != NULL && strstr(persistentState, "MyScriptUniqueID;")==persistentState)
    {
        int length = strlen(persistentState);
        if (length > 17)
        {
            // get the samples count
            char const * data = persistentState+17;
            samplesCount = atoi(data);
        }
    }
}

DSP_EXPORT uint64 getMaxStateLength()
{
    return 256;
}

DSP_EXPORT void computeOutputData()
{
    snprintf(outputString,256, "%d", samplesCount);
    outputStrings[0] = outputString;
}
