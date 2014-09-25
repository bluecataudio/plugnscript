
/* 
*  Midi.hxx - Midi utilities library for angelscript
* 
*  Created by Blue Cat Audio <services@bluecataudio.com>
*  Copyright 2011-2014 Blue Cat Audio. All rights reserved.
*
*/

/** Enumeration of Midi Events types supported by this library.
*
*/
enum MidiEventType
{
    kMidiNoteOff,
    kMidiNoteOn,
    kMidiControlChange,
    kMidiProgramChange,
    kMidiPitchWheel,
    kUnknown
};


/** Utility functions to handle Midi events.
*
*/
namespace MidiEventUtils
{
    /** Retrieves the type of the Midi event.
    *
    */
    MidiEventType getType(const MidiEvent& evt)
    {
        MidiEventType type=MidiEvent::kUnknown;
        switch(evt.byte0 & 0xF0)
        {
        case 0x80:
            type=MidiEvent::kMidiNoteOff;
            break;
        case 0x90:
            type=MidiEvent::kMidiNoteOn;
            break;
        case 0xB0:
            type=MidiEvent::kMidiControlChange;
            break;
        case 0xC0:
            type=MidiEvent::kMidiProgramChange;
            break;
        case 0xE0:
            type=MidiEvent::kMidiPitchWheel;
            break; 
        }
        return type;
    }

    /** Set the type of the Midi event.
    *  @see MidiEventType enum for supported Midi events.
    */
    void setType(MidiEvent& evt,MidiEventType type)
    {
        switch(type)
        {
        case MidiEvent::kMidiNoteOff:
            evt.byte0=0x80|(evt.byte0 & 0x0F);
            break;
        case MidiEvent::kMidiNoteOn:
            evt.byte0=0x90|(evt.byte0 & 0x0F);
            break;
        case MidiEvent::kMidiControlChange:
            evt.byte0=0xB0|(evt.byte0 & 0x0F);
            break;
        case MidiEvent::kMidiProgramChange:
            evt.byte0=0xC0|(evt.byte0 & 0x0F);
            break;
        case MidiEvent::kMidiPitchWheel:
            evt.byte0=0xE0|(evt.byte0 & 0x0F);
            break;
        }
    }

    /** Get the channel number of the event (1-16).
    *
    */
    uint8 getChannel(const MidiEvent& evt)
    {
        return (evt.byte0 & 0x0F)+1;
    }

    /** Set the channel number for the event (1-16).
    *
    */
    void setChannel(MidiEvent& evt, uint8 ch)
    {
        if(ch!=0)
            evt.byte0=(evt.byte0&0xF0)|((ch-1)&0x0F);
    }

    /** For a note event, retrieves the Midi note for the event (0-127).
    *
    */
    uint8 getNote(const MidiEvent& evt)
    {
        return evt.byte1&0x7F;
    }

    /** For a note event, sets the Midi note for the event (0-127).
    *
    */
    void setNote(MidiEvent& evt, uint8 note)
    {
        evt.byte1=(note&0x7F);
    }

    /** For a note event, retrieves the velocity for the event (0-127).
    *
    */
    uint8 getNoteVelocity(const MidiEvent& evt)
    {
        return evt.byte2 & 0x7F;
    }

    /** For a note event, sets the velocity for the event (0-127).
    *
    */
    void setNoteVelocity(MidiEvent& evt, uint8 velocity)
    {
        evt.byte2=velocity&0x7F;
    }

    /** For a CC (Control Change) event, gets the control number (0-127).
    *
    */
    uint8 getCCNumber(const MidiEvent& evt)
    {
        return evt.byte1 & 0x7F;
    }

    /** For a CC (Control Change) event, sets the control number (0-127).
    *
    */
    void setCCNumber(MidiEvent& evt,uint8 nb)
    {
        evt.byte1=nb&0x7F;
    }

    /** For a CC (Control Change) event, gets the control value (0-127).
    *
    */
    uint8 getCCValue(const MidiEvent& evt)
    {
        return evt.byte2 & 0x7F;	
    }

    /** For a CC (Control Change) event, sets the control value (0-127).
    *
    */
    uint8 setCCValue(const MidiEvent& evt)
    {
        return evt.byte1 & 0x7F;
    }

    /** For a Program Change event, gets the program number (0-127).
    *
    */
    uint8 getProgram(const MidiEvent& evt)
    {
        return evt.byte1&0x7F;
    }

    /** For a Program Change event, sets the program number (0-127).
    *
    */
    void setProgram(MidiEvent& evt, uint8 prog)
    {
        evt.byte2=prog&0x7F;
    }    

    /** For a pitch Wheel event, gets the pitch value (-8192 to +8192).
    *
    */
    int getPitchWheelValue(const MidiEvent& evt)
    {
        return (evt.byte2 & 0x7F)*128+(evt.byte1 & 0x7F)-64*128;	
    }

    /** For a pitch Wheel event, sets the pitch value (-8192 to +8192).
    *
    */
    void setPitchWheelValue(MidiEvent& evt, int value)
    {
        int midiValue=value+64*128;
        evt.byte1=midiValue&0x7F;
        evt.byte2=(midiValue/128)&0x7F;
    }
}