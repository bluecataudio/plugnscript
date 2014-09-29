// =====================================================================================
// =====================================================================================
// Made by Ivan COHEN, for Blue Cat Audio Plug'n Script
//
// http://musicalentropy.wordpress.com/
// =====================================================================================
// =====================================================================================


/** include your dsp files.
*
*/
#include "./library/Constants.hxx"

/** Define our parameters.
*/
array <string> inputParametersNames={"Frequency","Resonance","Volume"};
array <string> inputParametersUnits={"%","%","dB"};
array <double> inputParameters(inputParametersNames.length);
array <double> inputParametersDefault={100, 50, 0};
array <double> inputParametersMin={0, 0, -40};
array <double> inputParametersMax={100, 100, 40};

string name = "Diode Ladder filter";
string author = "Ivan COHEN";
string description = "Modeling of the famous EMS VCS3 Diode ladder filter";

// Define our internal variables.
double f, r;
double volume;


// Classes definition
class sampleProcessor
{
    double s0, s1, s2, s3;
    double zi;
	double xx, y0, y1, y2, y3;

    void processSample(double& sample)
    {
        const double input=sample;
        
        // input with half delay, for non-linearities
		const double ih = 0.5f * (input + zi);

		// gains
		double g0 = 1.f;
		double g1 = 1.836f;
		double g2 = 3*1.836f;

		double g0inv = 1.f/g0;
		double g1inv = 1.f/g1;
		double g2inv = 1.f/g2;

        // optimization of function calls 
        const double in0 = (ih - r * s3)*g0inv;
        const double a0 = in0 * in0;
        const double t0 = g0inv * f * (((a0 + 105)*a0 + 945) / ((15*a0 + 420)*a0 + 945));

        const double in1 = (s1-s0)*g1inv;
        const double a1 = in1 * in1;
        const double t1 = g1inv * f * (((a1 + 105)*a1 + 945) / ((15*a1 + 420)*a1 + 945));

        const double in2 = (s2-s1)*g1inv;
        const double a2 = in2 * in2;
        const double t2 = g1inv * f * (((a2 + 105)*a2 + 945) / ((15*a2 + 420)*a2 + 945));

        const double in3 = (s3-s2)*g1inv;
        const double a3 = in3 * in3;
        const double t3 = g1inv * f * (((a3 + 105)*a3 + 945) / ((15*a3 + 420)*a3 + 945));

        const double in4 = (s3)*g2inv;
        const double a4 = in4 * in4;
        const double t4 = g2inv * f * (((a4 + 105)*a4 + 945) / ((15*a4 + 420)*a4 + 945));

        // This formula gives the result for y3 thanks to MATLAB
		double y3 = (s2 + s3 + t2*(s1 + s2 + s3 + t1*(s0 + s1 + s2 + s3 + t0*zi)) + t1*(2*s2 + 2*s3))*t3 + s3 + 2*s3*t1 + t2*(2*s3 + 3*s3*t1);
		y3 /= (t4 + t1*(2*t4 + 4) + t2*(t4 + t1*(t4 + r*t0 + 4) + 3) + 2)*t3 + t4 + t1*(2*t4 + 2) + t2*(2*t4 + t1*(3*t4 + 3) + 2) + 1;

		// Other outputs
		double y2 = (s3 - (1+t4+t3)*y3) / (-t3);
		double y1 = (s2 - (1+t3+t2)*y2 + t3*y3) / (-t2);
		double y0 = (s1 - (1+t2+t1)*y1 + t2*y2) / (-t1);
		double xx = (zi - r*y3);

        // update state
		s0 += 2 * (t0*xx + t1*(y1-y0));
		s1 += 2 * (t2*(y2-y1) - t1*(y1-y0));
		s2 += 2 * (t3*(y3-y2) - t2*(y2-y1));
		s3 += 2 * (-t4*(y3) - t3*(y3-y2));

        zi = input;

        sample = y3*r*volume;
        //sample = input;
        
        // denormalization
        if (! (s0 < -1.0e-8 || s0 > 1.0e-8)) s0 = 0;
        if (! (s1 < -1.0e-8 || s1 > 1.0e-8)) s1 = 0;
        if (! (s2 < -1.0e-8 || s2 > 1.0e-8)) s2 = 0;
        if (! (s3 < -1.0e-8 || s3 > 1.0e-8)) s3 = 0;
    }

    void reset()
    {  	zi = 0;
        s0 = s1 = s2 = s3 = 0;
    }
    
};


// Define our objects
array<sampleProcessor> processors(audioOutputsCount);


// Reset function
void reset()
{
    for(uint i=0;i<audioOutputsCount;i++)
    {
        processors[i].reset();
    }
}

/** update internal parameters from inputParameters array.
*   called every sample before processSample method or every buffer before process method
*/
void updateInputParameters()
{
    double cutoff = pow(10, inputParameters[0]/100*(log10(20000)-log10(40))+log10(40));
    f = tan(PI * cutoff/sampleRate);
    r = 7 * inputParameters[1]/100 + 0.5;
    volume = pow(10, (inputParameters[2])/20);
}

/** per-sample processing function: called for every sample with updated parameters values.
*
*/
void processSample(array<double>& ioSample)
{		
	for(uint i=0;i<audioOutputsCount;i++)
	{
        processors[i].processSample(ioSample[i]);
    }
}


