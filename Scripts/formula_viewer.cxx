//////////////////////////////////////
// KUIML Formulas viewer
// written by Ilya Orlov (LetiMix) 
// http://letivyshe.ru/i/misc/formula_viewer_v1b.gif
// https://www.youtube.com/channel/UCbS0EpAZytfizVvysUluEHQ
// St. Petersburg, Russia, 2019
//////////////////////////////////////

// THE MAIN SCRIPT IS IN KUIML FILE

////////////////////////////
// script name

string name="Formula viewer";

////////////////////////////
// script functions

// per-sample processing function
void processSample(array<double>& ioSample){
    // mute output
    for(uint channel=0;channel<audioInputsCount;channel++) { 
        ioSample[channel]=0; 
    }
}
