/** \file persistentState.cxx
* A stupid simple example that shows how to make the DSP script state persistent.
* The DSP script counts the number of samples elapsed since it was initialized and stores it into
* the persistent state so that it gets restored upon preset or session reload.
*/

string name="PersistenT State Sample";
string description="saves current samples count into persistent state";

array<string> outputStringsNames={"Elapsed Samples"};
array<string> outputStrings(outputStringsNames.length);

// has the state changed?
bool 	changed=true;

// the number of samples to store in state
int samplesCount=0;

// temp string (pre-allocated)
string tempString;

void initialize()
{
  // preallocate the temporary string
   tempString.length=1024;
}

void processSample(array<double>& ioSample)
{
    samplesCount++;
    changed=true;
}

int getTailSize()
{
    return -1;
}

bool saveState(string& persistentState)
{
  // store only if changed
  bool hasChanged=changed;
  if(changed)
  {
     changed=false;
     // save a  uniqueID string first, so that we can check state upon restore
     persistentState="MyScriptUniqueID;";
     uIntToString(samplesCount,tempString);
     persistentState+=tempString;
  }
  return hasChanged;
}

void loadState(const string& persistentState)
{
   // check that this is the state we are expecting
   if(persistentState.findFirst("MyScriptUniqueID;")==0)
   {
      // use erase on temp copy to skip the ID to the integer data, as it does not allocate
      tempString=persistentState;
      tempString.erase(0,17);
      samplesCount=parseUInt(tempString);
   }
}

uint64 getMaxStateLength()
{
    // the max possible length for the state.
    // this allosw the engine to preallocate the state upon init
    return 256;
}

void computeOutputData()
{
    uIntToString(samplesCount,outputStrings[0]);
}
