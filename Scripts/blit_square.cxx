/*
 * blit_square.cxx
 *
 * Copyright (c) 2014, fukuroda (https://github.com/fukuroder)
 * Released under the MIT license
 */

#include "library/Midi.hxx"
#include "library/Constants.hxx"

string name="blit square";
string description="BLIT-Based square wave synthesis";
blit_square_oscillator _blit_square;

// note
class blit_square_oscillator_note
{
    // current time
    double t;

    // current value
    double value;

    // nyquist limit
    int n;

    // delta t
    double dt;

    // note number [0,127]
    int note_no;

    // velocity [0,1]
    double velocity;

    // constructor
    blit_square_oscillator_note()
    {
        t = 0.0;
        value = 0.0;
        n = 0;
        dt = 0.0;
        note_no = 0;
        velocity = 0.0;
    }
};

// oscillator class
class blit_square_oscillator
{
    array<blit_square_oscillator_note> _notes(8);
    uint _active_note_count;
    double _pitchbend;

    blit_square_oscillator()
    {
        _active_note_count = 0;
        _pitchbend = 0.0;
    }

    void trigger(const MidiEvent& evt)
    {
        if( _active_note_count < _notes.length )
        {
            blit_square_oscillator_note@ note = _notes[_active_note_count];

            note.note_no = MidiEventUtils::getNote(evt);
            note.velocity = MidiEventUtils::getNoteVelocity(evt)/127.0;
            note.value = 1.0;
            note.t = 0.25;

            //
            double freq = 440.0*(pow(2.0, (note.note_no + _pitchbend - 69.0) / 12.0));
            note.n = int(sampleRate / 4.0 / freq)*2;
            note.dt = freq / sampleRate;

            ++_active_note_count;
        }
    }

    void update_pitchbend(const MidiEvent& evt)
    {
        _pitchbend = MidiEventUtils::getPitchWheelValue(evt)/4096.0;

        for (uint i = 0; i < _active_note_count; ++i)
        {
            blit_square_oscillator_note@ note = _notes[i];

            double freq = 440.0*(pow(2.0, (note.note_no + _pitchbend - 69.0) / 12.0));
            note.n = int(sampleRate / 4.0 / freq)*2;
            note.dt = freq / sampleRate;
        }
    }

    void release(const MidiEvent& evt)
    {
        int note_no = MidiEventUtils::getNote(evt);

        //
        uint idx;
        for(idx = 0; idx < _active_note_count; ++idx)
        {
            if( _notes[idx].note_no == note_no )break;
        }

        if( idx < _active_note_count )
        {
            if( idx < _active_note_count - 1)
            {
                //    [1][ ][3][4][ ][ ]
                // -> [1][4][3][ ][ ][ ]
                _notes[idx] = _notes[_active_note_count - 1];
            }

            --_active_note_count;
        }
    }

    //
    bool is_silent()
    {
        return _active_note_count == 0;
    }

    //
    double bandlimited_bipolar_impulse(double t, int n)
    {
        if ( t < 1.0e-8 || 1.0-1.0e-8 < t )
        {
            return 4.0*n;
        }
        else if( 0.5 - 1.0e-8 < t && t < 0.5 + 1.0e-8 )
        {
            return -4.0*n;
        }
        else
        {
            double den = sin(2*PI*t);
            double num = sin(2*n*PI*t);
            return 4.0*num/den;
        }
    }

    //
    double process_sample()
    {
        double value = 0.0;
        for (uint i = 0; i < _active_note_count; ++i)
        {
            // render
            blit_square_oscillator_note@ note = _notes[i];
            value += note.value * note.velocity;

            // update
            note.t += note.dt;
            if (1.0 <= note.t)note.t -= 1.0;
            note.value = note.value*0.999 + bandlimited_bipolar_impulse(note.t, note.n)*note.dt;
        }
        return value;
    }
};

void processBlock(BlockData& data)
{
    const MidiEvent@ event;
    if( 0 < data.inputMidiEvents.length )
    {
        @event = data.inputMidiEvents[0];
    }

    uint event_idx = 0;
    for(uint i=0; i<data.samplesToProcess; ++i)
    {
        while( @event != null && event.timeStamp <= double(i) )
        {
            MidiEventType evt_type = MidiEventUtils::getType(event);
            if( evt_type == kMidiNoteOn )
            {
                _blit_square.trigger(event);
            }
            else if( evt_type == kMidiNoteOff )
            {
                _blit_square.release(event);
            }
            else if( evt_type == kMidiPitchWheel )
            {
                _blit_square.update_pitchbend(event);
            }

            ++event_idx;
            if( event_idx < data.inputMidiEvents.length )
            {
                @event = data.inputMidiEvents[event_idx];
            }
            else
            {
                @event = null;
            }
        }

        if( _blit_square.is_silent() )continue;

        double value = _blit_square.process_sample();
        for(uint ch = 0; ch < audioOutputsCount; ++ch)
        {
            data.samples[ch][i] = value;
        }
    }
}

int getTailSize()
{
    return -1;
}
