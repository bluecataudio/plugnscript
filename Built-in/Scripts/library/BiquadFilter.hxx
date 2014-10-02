/** \file BiquadFilter.hxx
* Simple biquad filter library for angelscript.
*
* Created by Blue Cat Audio <services@bluecataudio.com>
* \copyright 2011-2014 Blue Cat Audio. All rights reserved.
*
*/

namespace KittyDSP
{
    namespace Biquad
    {
        class FilterState
        {
            double  inputMem1=0; 			
            double  inputMem2=0; 			    												
            double  outputMem1=0; 			    												
            double  outputMem2=0;
        };

        const double sqr2_2=sqrt(2.0)/2.0;
        const double _sqrt2=1/sqrt(2.0);

        class Filter
        {
            double  inputCoeff0=0; 				
            double  inputCoeff1=0; 			
            double  inputCoeff2=0; 			    												
            double  outputCoeff1=0; 			    												
            double  outputCoeff2=0; 			    												

            private array<KittyDSP::Biquad::FilterState> state;

            Filter(int iChannelsCount)
            {
                state.resize(iChannelsCount);
                resetState();
            }

            void processSample(double& sample,int channel)
            {
                KittyDSP::Biquad::FilterState@ channelState=state[channel];

                //Compute Output
                double output=(inputCoeff0*sample+inputCoeff1*channelState.inputMem1+inputCoeff2*channelState.inputMem2
                    +outputCoeff1*channelState.outputMem1+outputCoeff2*channelState.outputMem2);

                // getting rid of denormal numbers
                const double antiDenormal=1e-30;
                output+=antiDenormal;
                output-=antiDenormal;

                // Update signal memories
                channelState.inputMem2=channelState.inputMem1;
                channelState.inputMem1=sample;
                channelState.outputMem2=channelState.outputMem1;
                channelState.outputMem1=output;

                sample=output;
            }

            void processSample(array<double>& ioSample)
            {
                for(int i=0, channelsCount=state.length;i<channelsCount;i++)
                {
                    KittyDSP::Biquad::FilterState@ channelState=state[i];
                    double input=ioSample[i];

                    //Compute Output
                    double output=(inputCoeff0*input+inputCoeff1*channelState.inputMem1+inputCoeff2*channelState.inputMem2
                        +outputCoeff1*channelState.outputMem1+outputCoeff2*channelState.outputMem2);

                    // getting rid of denormal numbers
                    const double antiDenormal=1e-30;
                    output+=antiDenormal;
                    output-=antiDenormal;

                    // Update signal memories
                    channelState.inputMem2=channelState.inputMem1;
                    channelState.inputMem1=input;
                    channelState.outputMem2=channelState.outputMem1;
                    channelState.outputMem1=output;

                    ioSample[i]=output;
                }
            }


            void	resetState()
            {
                for(int i=0, channelsCount=state.length;i<channelsCount;i++)
                {
                    KittyDSP::Biquad::FilterState@ channelState=state[i];
                    channelState.inputMem1=0; 			
                    channelState.inputMem2=0; 			    												
                    channelState.outputMem1=0; 			    												
                    channelState.outputMem2=0;
                }
            }

            void	setLowPass(double theta)
            {
                const double theta64=theta;
                const double alpha= sin(theta64)*sqr2_2;
                const double cs0=cos(theta64);
                const double b0 =  (1 - cs0)*.5;
                const double b1 =   1 - cs0;
                const double b2 =  (1 - cs0)*.5;
                const double _a0 =   1/(1 + alpha);
                const double a1 =  -2*cs0;
                const double a2 =   1 - alpha;

                inputCoeff0=b0*_a0;
                inputCoeff1=b1*_a0;
                inputCoeff2=b2*_a0;
                outputCoeff1=-a1*_a0;
                outputCoeff2=-a2*_a0;
            }


            void	setHighPass(double theta)
            {
                const double theta64=theta;
                const double alpha= sin(theta64)*sqr2_2;
                const double cs0=cos(theta64);
                const double b0 =  (1 + cs0)*.5;
                const double b1 = -(1 + cs0);
                const double b2 =  (1 + cs0)*.5;
                const double _a0 =   1/(1 + alpha);
                const double a1 =  -2*cs0;
                const double a2 =   1 - alpha;

                inputCoeff0=b0*_a0;
                inputCoeff1=b1*_a0;
                inputCoeff2=b2*_a0;
                outputCoeff1=-a1*_a0;
                outputCoeff2=-a2*_a0;
            }


            void	setResonantLowPass(double theta,double q /*peak value*/)
            {
                const double theta64=theta;
                const double alpha= sin(theta64)/(q*2);
                const double cs0=cos(theta64);
                const double b0 =  (1 - cs0)*.5;
                const double b1 =   1 - cs0;
                const double b2 =  (1 - cs0)*.5;
                const double _a0 =   1/(1 + alpha);
                const double a1 =  -2*cs0;
                const double a2 =   1 - alpha;

                inputCoeff0=b0*_a0;
                inputCoeff1=b1*_a0;
                inputCoeff2=b2*_a0;
                outputCoeff1=-a1*_a0;
                outputCoeff2=-a2*_a0;
            }


            void	setResonantHighPass(double theta,double q /*peak value*/)
            {
                const double theta64=theta;
                const double alpha= sin(theta64)/(q*2);
                const double cs0=cos(theta64);
                const double b0 =  (1 + cs0)*.5;
                const double b1 = -(1 + cs0);
                const double b2 =  (1 + cs0)*.5;
                const double _a0 =   1/(1 + alpha);
                const double a1 =  -2*cs0;
                const double a2 =   1 - alpha;

                inputCoeff0=b0*_a0;
                inputCoeff1=b1*_a0;
                inputCoeff2=b2*_a0;
                outputCoeff1=-a1*_a0;
                outputCoeff2=-a2*_a0;
            }


            void	setLowShelf(double theta,double doubleSquareRootGain)
            {
                const double A=doubleSquareRootGain*doubleSquareRootGain;
                const double theta64=theta;
                const double alpha= sin(theta64)*sqr2_2;
                const double sqrtAx2xAlpha=2*alpha*doubleSquareRootGain;

                const double cs0=cos(theta64);
                const double b0 =    A*( (A+1) - (A-1)*cs0 + sqrtAx2xAlpha );
                const double b1 = 2*A*( (A-1) - (A+1)*cs0                   );
                const double b2 =    A*( (A+1) - (A-1)*cs0 - sqrtAx2xAlpha );
                const double _a0 =    1/((A+1) + (A-1)*cs0 + sqrtAx2xAlpha);
                const double a1 =   -2*( (A-1) + (A+1)*cs0                  );
                const double a2 =        (A+1) + (A-1)*cs0 - sqrtAx2xAlpha;

                inputCoeff0=b0*_a0;
                inputCoeff1=b1*_a0;
                inputCoeff2=b2*_a0;
                outputCoeff1=-a1*_a0;
                outputCoeff2=-a2*_a0;
            }


            void	setHighShelf(double theta,double doubleSquareRootGain)
            {
                const double A=doubleSquareRootGain*doubleSquareRootGain;
                const double theta64=theta;
                const double alpha= sin(theta64)*sqr2_2;
                const double sqrtAx2xAlpha=2*alpha*doubleSquareRootGain;

                // Gain
                const double cs0=cos(theta64);
                const double b0 =    A*( (A+1) + (A-1)*cs0 + sqrtAx2xAlpha );
                const double b1 = -2*A*( (A-1) + (A+1)*cs0                   );
                const double b2 =    A*( (A+1) + (A-1)*cs0 - sqrtAx2xAlpha );
                const double _a0 =    1/(    (A+1) - (A-1)*cs0 + sqrtAx2xAlpha);
                const double a1 =    2*( (A-1) - (A+1)*cs0                  );
                const double a2 =        (A+1) - (A-1)*cs0 - sqrtAx2xAlpha;

                inputCoeff0=b0*_a0;
                inputCoeff1=b1*_a0;
                inputCoeff2=b2*_a0;
                outputCoeff1=-a1*_a0;
                outputCoeff2=-a2*_a0;
            }


            void	setPeak(double theta,double halfBwInOctava,double sqrtGain)
            {
                const double theta64=theta;
                const double A=sqrtGain;
                const double alpha= sin(theta64)*halfBwInOctava;
                const double alphaA=alpha*A;
                const double alpha_A=alpha/A;
                const double b0 =   1 + alphaA;
                const double b1 =  -2*cos(theta64);
                const double b2 =   1 - alphaA;
                const double _a0 =   1.0/(1.0 + alpha_A);
                const double a2 =   1 - alpha_A;

                inputCoeff0=b0*_a0;
                inputCoeff1=b1*_a0;
                inputCoeff2=b2*_a0;
                outputCoeff1=-inputCoeff1;
                outputCoeff2=-a2*_a0;
            }

            void	setBandPass(double theta,double halfBwInOctava)
            {
                const double theta64=theta;
                const double alpha= sin(theta64)*halfBwInOctava;
                const double b0 =alpha;
                const double b2 =  -alpha;
                const double _a0 =   1.0/(1.0 + alpha);
                const double a1 =  -2*cos(theta64);
                const double a2 =   1 - alpha;

                inputCoeff0=b0*_a0;
                inputCoeff1=0;
                inputCoeff2=b2*_a0;
                outputCoeff1=-a1*_a0;
                outputCoeff2=-a2*_a0;
            }

            void	setNotch(double theta, double halfBwInOctava)
            {
                double alpha= sin(theta)*halfBwInOctava;
                double cs0=cos(theta);
                double b0 =1;
                double b1 =  -2*cs0;
                double b2 =  1;
                double _a0 =   1.0/(1.0 + alpha);
                double a1 =  -2*cs0;
                double a2 =   1 - alpha;

                inputCoeff0=b0*_a0;
                inputCoeff1=b1*_a0;
                inputCoeff2=b2*_a0;
                outputCoeff1=-a1*_a0;
                outputCoeff2=-a2*_a0;
            }

            void	setAllPass(double theta, double Q)
            {
                const double theta64=theta;
                const double alpha= sin(theta64)*.5/Q;
                const double cs0=cos(theta64);
                const double b0 =   1 - alpha;
                const double b1 =  -2*cs0;
                const double b2 =   1 + alpha;
                const double _a0 =  1/b2;

                inputCoeff0=b0*_a0;
                inputCoeff1=b1*_a0;
                inputCoeff2=b2*_a0;
                outputCoeff1=-inputCoeff1;
                outputCoeff2=-inputCoeff0;
            }
        }
    }
}