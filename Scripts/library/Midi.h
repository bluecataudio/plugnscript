
#ifndef _Midi_h_
#define _Midi_h_

/**
 *  \file Midi.h
 *  Midi utilities library for c/c++ dsp scripting
 *
 *  Created by Blue Cat Audio <services@bluecataudio.com>
 *  Copyright 2011-2015 Blue Cat Audio. All rights reserved.
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
    kMidiPitchWheel,  ///< Picth Wheel Event
    kUnknown ///< Other Events
};


/** Utility functions to handle Midi events.
 *
 */
#ifdef __cplusplus
namespace MidiEventUtils
{
#endif
    /** Retrieves the type of the Midi event.
     *
     */
    static inline MidiEventType getType(const struct MidiEvent& evt)
    {
        MidiEventType type=kUnknown;
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
        }
        return type;
    }
    
    /** Set the type of the Midi event.
     *  @see MidiEventType enum for supported Midi events.
     */
    static inline void setType(struct MidiEvent& evt,MidiEventType type)
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
            default:
                break;
        }
    }
    
    /** Get the channel number of the event (1-16).
     *
     */
    static inline uint8 getChannel(const struct MidiEvent& evt)
    {
        return (evt.byte0 & 0x0F)+1;
    }
    
    /** Set the channel number for the event (1-16).
     *
     */
    static inline void setChannel(struct MidiEvent& evt, uint8 ch)
    {
        if(ch!=0)
            evt.byte0=(evt.byte0&0xF0)|((ch-1)&0x0F);
    }
    
    /** For a note event, retrieves the Midi note for the event (0-127).
     *
     */
    static inline uint8 getNote(const struct MidiEvent& evt)
    {
        return evt.byte1&0x7F;
    }
    
    /** For a note event, sets the Midi note for the event (0-127).
     *
     */
    static inline void setNote(struct MidiEvent& evt, uint8 note)
    {
        evt.byte1=(note&0x7F);
    }
    
    /** For a note event, retrieves the velocity for the event (0-127).
     *
     */
    static inline uint8 getNoteVelocity(const struct MidiEvent& evt)
    {
        return evt.byte2 & 0x7F;
    }
    
    /** For a note event, sets the velocity for the event (0-127).
     *
     */
    static inline void setNoteVelocity(struct MidiEvent& evt, uint8 velocity)
    {
        evt.byte2=velocity&0x7F;
    }
    
    /** For a CC (Control Change) event, gets the control number (0-127).
     *
     */
    static inline uint8 getCCNumber(const struct MidiEvent& evt)
    {
        return evt.byte1 & 0x7F;
    }
    
    /** For a CC (Control Change) event, sets the control number (0-127).
     *
     */
    static inline void setCCNumber(struct MidiEvent& evt,uint8 nb)
    {
        evt.byte1=nb&0x7F;
    }
    
    /** For a CC (Control Change) event, gets the control value (0-127).
     *
     */
    static inline uint8 getCCValue(const struct MidiEvent& evt)
    {
        return evt.byte2 & 0x7F;
    }
    
    /** For a CC (Control Change) event, sets the control value (0-127).
     *
     */
    static inline void setCCValue(struct MidiEvent& evt,uint8 value)
    {
        evt.byte2=value & 0x7F;
    }
    
    /** For a Program Change event, gets the program number (0-127).
     *
     */
    static inline uint8 getProgram(const struct MidiEvent& evt)
    {
        return evt.byte1&0x7F;
    }
    
    /** For a Program Change event, sets the program number (0-127).
     *
     */
    static inline void setProgram(struct MidiEvent& evt, uint8 prog)
    {
        evt.byte2=prog&0x7F;
    }
    
    /** For a pitch Wheel event, gets the pitch value (-8192 to +8192).
     *
     */
    static inline int getPitchWheelValue(const struct MidiEvent& evt)
    {
        return (evt.byte2 & 0x7F)*128+(evt.byte1 & 0x7F)-64*128;
    }
    
    /** For a pitch Wheel event, sets the pitch value (-8192 to +8192).
     *
     */
    static inline void setPitchWheelValue(struct MidiEvent& evt, int value)
    {
        int midiValue=value+64*128;
        evt.byte1=midiValue&0x7F;
        evt.byte2=(midiValue/128)&0x7F;
    }
#ifdef __cplusplus
}
#endif
#endif