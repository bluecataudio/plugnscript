
/** 
*  \file Midi.hxx
*  Midi utilities library for angelscript
* 
*  Created by Blue Cat Audio <services@bluecataudio.com>
*  Copyright 2011-2025 Blue Cat Audio. All rights reserved.
*
*/

/** Enumeration of Midi Events types currently supported by this library.
*
*/
enum MidiEventType
{
    kMidiNoteOff, ///< Note Off Event
    kMidiNoteOn, ///< Note On Event
    kMidiControlChange, ///< Control Change Event (CC)
    kMidiProgramChange, ///< Program Change Event
    kMidiPitchWheel,  ///< Pitch Wheel Event
    kMidiNoteAfterTouch,  ///< Note after touch
    kMidiChannelAfterTouch, ///< Channel after touch
    kSysexStart, ///< Sysex stream start event (may also contain beginning of data stream)
    kSysexEnd, ///< Sysex stream end event (may also contain end of data stream)
    kBeatClock, ///< MIDI Beat Clock Event
    kStart, ///< MIDI Start Event
    kSongPointer, ///< MIDI Song Pointer Position Event
    kContinue, ///< MIDI Continue Event
    kStop, ///< MIDI Stop Event
    kUnknown ///< Other Events
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
        MidiEventType type=kUnknown;
        // Channel Events
        switch(evt.byte0 & 0xF0)
        {
        case 0x80:
            type=kMidiNoteOff;
            break;
        case 0x90:
            type=kMidiNoteOn;
            break;
        case 0xB0:
            type=kMidiControlChange;
            break;
        case 0xC0:
            type=kMidiProgramChange;
            break;
        case 0xE0:
            type=kMidiPitchWheel;
            break;
        case 0xA0:
            type=kMidiNoteAfterTouch;
            break;
        case 0xD0:
            type=kMidiChannelAfterTouch;
            break;
        }

        // None of these? Check full byte
        if (type == kUnknown)
        {
            // channel-independent messages
            switch (evt.byte0)
            {
            case 0xF0:
                type = kSysexStart;
                break;
            case 0xF7:
                type = kSysexEnd;
                break;
            case 0xF8:
                type = kBeatClock;
                break;
            case 0xFA:
                type = kStart;
                break;
            case 0xFB:
                type = kContinue;
                break;
            case 0xFC:
                type = kStop;
                break;
            case 0xF2:
                type = kSongPointer;
                break;
            }

            if (type == kUnknown)
            {
                // sysex end can be anywhere else in the event
                for (int i = 1; i < 4; i++)
                {
                    if (getEventByte(evt,i) == 0xF7)
                    {
                        type = kSysexEnd;
                        break;
                    }
                }
            }
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
        case kMidiNoteOff:
            evt.byte0=0x80|(evt.byte0 & 0x0F);
            break;
        case kMidiNoteOn:
            evt.byte0=0x90|(evt.byte0 & 0x0F);
            break;
        case kMidiControlChange:
            evt.byte0=0xB0|(evt.byte0 & 0x0F);
            break;
        case kMidiProgramChange:
            evt.byte0=0xC0|(evt.byte0 & 0x0F);
            break;
        case kMidiPitchWheel:
            evt.byte0=0xE0|(evt.byte0 & 0x0F);
            break;
        case kMidiNoteAfterTouch:
            evt.byte0=0xA0|(evt.byte0 & 0x0F);
            break;
        case kMidiChannelAfterTouch:
            evt.byte0=0xD0|(evt.byte0 & 0x0F);
            break;
        case kSysexStart:
            evt.byte0 = 0xF0;
            break;
        case kSysexEnd:
            evt.byte0 = 0xF7;
            break;
        case kBeatClock:
            evt.byte0 = 0xF8;
            break;
        case kStart:
            evt.byte0 = 0xFA;
            break;
        case kContinue:
            evt.byte0 = 0xFB;
            break;
        case kStop:
            evt.byte0 = 0xFC;
            break;
        case kSongPointer:
            evt.byte0 = 0xF2;
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
    void setCCValue(MidiEvent& evt,uint8 value)
    {
        evt.byte2=value & 0x7F;
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
        evt.byte1=prog&0x7F;
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

    /** For a channel after touch event, gets the after touch value (0-127)
    *
    */
    int  getChannelAfterTouchValue(const MidiEvent&  evt)
    {
        return evt.byte1&0x7F;
    }

    /** For a channel after touch event, sets the after touch value (0-127)
    *
    */
    void  setChannelAfterTouchValue(MidiEvent& evt, int value)
    {
        evt.byte1=(value&0x7F);;
    }


    /** Utility function to read event bytes programmatically.
    *
    */
    uint8 getEventByte(const MidiEvent& event, uint index)
    {
        switch (index)
        {
        case 0:
            return event.byte0;
        case 1:
            return event.byte1;
        case 2:
            return event.byte2;
        case 3:
            return event.byte3;
        }
        return 0;
    }

    /** Utility function to set event bytes programmatically.
    *
    */
    void setEventByte(MidiEvent& event, uint index, uint8 value)
    {
        switch (index)
        {
        case 0:
            event.byte0 = value;
            break;
        case 1:
            event.byte1 = value;
            break;
        case 2:
            event.byte2 = value;
            break;
        case 3:
            event.byte3 = value;
            break;
        }
    }

    /** Utility function to finish a sysex stream in a midi event.
    *   Returns true is there was enough room left in the event to write Sysex stream end
    */
    bool finishSysex(MidiEvent& event, uint nextByteIndex)
    {
        bool done = false;
        // finish Sysex stream with 0xF7 status byte
        if (nextByteIndex < 4)
        {
            setEventByte(event, nextByteIndex, 0xF7);
            done = true;
            nextByteIndex++;

            // fill other bytes with zeros
            while (nextByteIndex < 4)
            {
                setEventByte(event, nextByteIndex, 0);
                nextByteIndex++;
            }
        }
        return done;
    }

    /** Returns true if the event contains SysEx data.
    *
    */
    bool isSysexData(const MidiEvent& evt)
    {
        bool isSysex = true;
        for (int i = 0; i < 4 && isSysex; i++)
        {
            const uint8 b = getEventByte(evt,i);
            isSysex = (b < 0x80 || b == 0xF0 || b == 0xF7);
        }
        return isSysex;
    }

    /** Returns true for system realtime events.
    *
    */
    bool isSystemRealtime(const MidiEvent& evt)
    {
        const uint8 b = evt.byte0;
        bool isRT = b >= 0xfa || b == 0xf8;
        return isRT;
    }

    /** For a Song Pointer Position Event, returns the position in the song (in sixteenth notes).
    *
    */
    int getSongPointerPosition(const MidiEvent& evt)
    {
        return ((evt.byte1 & 0x7F) | ((evt.byte2 & 0x7F) << 7));
    }

    /** For a Song Pointer Position Event, sets the position in the song (in sixteenth notes).
    *
    */
    void setSongPointerPosition(MidiEvent& evt, int posIn16thNotes)
    {
        evt.byte1 = posIn16thNotes & 0x7F;
        evt.byte2 = (posIn16thNotes >> 7) & 0x7F;
    }
}