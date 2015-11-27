/*
*   Random Walk multi-CC LFO for Blue Cat's Plug'n'Script
*   by Chris Dewhurst, Flying Objects Sofware Inc., copyright 2014
*   chris@flyingobjects.com
*   License is granted to anyone to use for whatever you like
*
*   This script generates N-channels of constantly varying CC values
*   which wander around in varying size circles at a given rate.
*
*   The controls are Rate, which can vary from 1/100 Hz to 10 Hz
*   and "Wander", which determines how far the values move.
*   Wander has a special property that if it is < 0.5, the values
*   move around their current center point, if it is >= 0.5 they
*   can move anywhere in the CC range.  Thus, if you get to a sweet
*   spot in the random variations, you can move the control to < 0.5
*   and it will stay in that area.
*
*   I created this script to help generate effects that change continuously
*   and organically.  For example, it works really well with Absynth 5 Fx
*   hooked up to a bunch of the Perform parameters
*/

// Output parameters definition

const int numOutputs = 6;

array<string> outputParametersNames(numOutputs);
array<double> outputParameters(numOutputs);
array<double> outputParametersMin(numOutputs);
array<double> outputParametersMax(numOutputs);
array<double> a(numOutputs);
array<double> b(numOutputs);
array<double> c(numOutputs);

array<string> inputParametersNames={"Rate","Wander"};
array<double> inputParameters(inputParametersNames.length);
array<double> inputParametersMin={0, 0};
array<double> inputParametersMax={3,1};
array<double> inputParametersDefault={2,0.5};


// curStep and lastStep mark out time, measured in radians as we move around the cycle
// We use the sample clock as our time base, so the step size is based
double lastStep = 0;
double curStep = 0;
double pi = 3.1415926;
double twoPi = pi * 2;
double oneHz = twoPi / sampleRate;
double rate = oneHz / 5;		// Default to 1/5 Hz cycle rate



void initialize()
{
    // initialize output parameters properties
    for(uint i=0;i<numOutputs;i++)
    {
        outputParametersNames[i]="Ch"+(i+1);
        outputParametersMin[i]= -1;
        outputParametersMax[i]= 1;
        a[i]= -1;	// These will be the outside limits of the wanderings on each axis
        b[i]= 1;
        c[i]= 0;	// This will be the center of the wanderings on each axis
    } 
}



/* per-sample processing.
*
*/
void processSample(array<double>& ioSample)
{   // Only used to keep track of current time
    curStep += rate;
}


int getTailSize()
{
    // We wan to run all the time
    return -1;
}

void reset()
{
}


void computeOutputData()
{
    double wander = inputParameters[1];		// An input parameter to determine how far and fast
						// the outputs will change

    bool halfWay = (lastStep <= pi) && (curStep >= pi);		// True if we just past 180deg
    bool allTheWay = (lastStep <= twoPi) && (curStep >= twoPi);	// True if we just past 360deg

    for (uint i=0;i<numOutputs;i++)
    {	// Each output parameter follows a sine (actually, for calculating convenience, a cosine)
	// between two points a and b which are adjusted each time through the cycle

	outputParameters[i] = ((a[i]-b[i]) * cos(curStep) + (a[i] + b[i])) / 2.0;

	if (halfWay)
	{   // We're at 180deg so compute a new point a
	    if (wander < 0.5)
	    {	// If "wander" is less than 0.5, move around the current center
		a[i] = c[i] - wander + rand(0, 2*wander);
		if (a[i] < -1) a[i] = -1;
		else if (a[i] > 1) a[i] = 1;
	    }
	    else
	    {	// If wander >= 0.5 move around the whole space, making sure we
		// don't run up against the outside wall

    		if ((rand(0, 1) > 0.5) && (b[i] < 0.99) || (b[i] < -0.99))
		    a[i] = c[i] + pow(rand(0, 1), 1/wander);
    		else 
		    a[i] = c[i] - pow(rand(0, 1), 1/wander);
		if (a[i] < -1) a[i] = -1;
		else if (a[i] > 1) a[i] = 1;
     		c[i] = (a[i] + b[i]) / 2.0;
	    }
	}
	else if (allTheWay)
	{   // We're at 360deg so do the same for point b
	    if (wander < 0.5)
	    {
		b[i] = c[i] - wander + rand(0, 2*wander);
		if (b[i] < -1) b[i] = -1;
		else if (b[i] > 1) b[i] = 1;
	    }
	    else
	    {
     		if ((rand(0, 1) > 0.5) && (a[i] < 0.99) || (a[i] < -0.99))
		    b[i] = c[i] + pow(rand(0, 1), 1/wander);
    		else 
		    b[i] = c[i] - pow(rand(0, 1), 1/wander);
		if (b[i] < -1) b[i] = -1;
		else if (b[i] > 1) b[i] = 1;
    		c[i] = (a[i] + b[i]) / 2.0;
	    }
	}
//print("sampleCount=" + sampleCount + "  outputParameters[i]=" + outputParameters[i]);
    }

    if (allTheWay)
    {	// If we've completed a cycle, take the step count back 360deg
	curStep -= twoPi;
    }
    lastStep = curStep;

    // Compute new rate based on Rate control, the range is 1/100 Hz to 10 Hz as input goes from 0 to 3
    rate = oneHz * (pow(10.0, inputParameters[0]) / 100.0);
}
