/** Blue Cat PlugNScript dsp api header file.
 *  Contains all definitions required for DSP scripting in C or C++.
 *  Copyright (c) 2015-2017 Blue Cat Audio. All rights reserved.
 */

#ifndef _dspapi_h
#define _dspapi_h

// handling "bool" definition for C compilers
#ifndef __cplusplus
#if(!defined(_MSC_VER) || _MSC_VER >= 1900)
#include <stdbool.h> // using the C99 definition of bool
#else
#define bool char // for binary compatibility with C++
#define true 1
#define false 0
#endif
#endif

// Visual Studio does not support the inline keyword in C before VS2015
#if(!defined(_MSC_VER) || _MSCVER>=1900 || defined(__cplusplus))
#define INLINE_API inline
#else
#define INLINE_API
#endif

// basic types definitions for angelscript compatibility-----------------
typedef char const*     string;
typedef unsigned int    uint;
typedef signed char     int8;
typedef unsigned char   uint8;
#ifdef _MSC_VER // Visual Studio
typedef _int64          int64;
typedef unsigned _int64 uint64;
#else // GCC compatible compiler assumed
#include <stdint.h>
typedef int64_t         int64;
typedef uint64_t        uint64;
#endif

#define null 0

/** MIDI Event (packet) abstraction.
 *	Contains 4 bytes of data and a timestamp.
 */
struct MidiEvent
{
    /// MIDI data (4 bytes packet)
    uint8 byte0;
    uint8 byte1;
    uint8 byte2;
    uint8 byte3;
    /// time stamp of the event, as an offset in samples
    /// from the beginning of the current block.
    double timeStamp;
    
    
#ifdef __cplusplus // a default costructor is provided for C++
    MidiEvent():byte0(0),byte1(0),byte2(0),byte3(0),timeStamp(0){}
#endif
};

struct MidiQueue;

/// push function type definition for C implementation
typedef void (MidiQueuePushEventFunc)(struct MidiQueue* queue,const struct MidiEvent* evt);

/** List of MIDI Events.
 *
 */
struct MidiQueue
{
    /// Midi Events array
    struct MidiEvent* events;
    
    /// The number of events available in the queue. Never change this value
    /// Adding events should only be done using the fPush function (C) or push method (C++)
    uint             length;
    
    /// pushes an event at the end of the events queue.
    MidiQueuePushEventFunc*   pushEvent;
    
    
    // extra C++ inline methods for angelscript compatibility
#ifdef __cplusplus
    /// pushes "evt" at the end of the events queue.
    inline void push(const MidiEvent & evt)
    {
        if(pushEvent!=null)
            pushEvent(this,&evt);
    }
    
    /// random access operator to access events directly (angelscript compatibility)
    inline const MidiEvent& operator [](uint i)const
    {
        return events[i];
    }
    
    /// random access operator to access events directly (angelscript compatibility)
    inline MidiEvent& operator [](uint i)
    {
        return events[i];
    }
#endif
};

/** Host Transport information.
 *
 */
struct TransportInfo
{
    /// Returns the current tempo (beats per minute)
    /// Can be accessed directly as "bpm" attribute.
    double  bpm;
    /// Returns the upper value of the time signature.
    /// Can be accessed directly as "timeSigTop" attribute.
    uint    timeSigTop;
    /// Returns the lower value of the time signature.
    /// Can be accessed directly as "timeSigBottom" attribute.
    uint    timeSigBottom;
    /// Returns true when the host application is playing.
    /// Can be accessed directly as "isPlaying" attribute.
    bool    isPlaying;
    /// Returns true when the transport of the host application is in a loop.
    /// Can be accessed directly as "isLooping" attribute.
    bool    isLooping;
    /// Returns true when the host application is recording.
    /// Can be accessed directly as "isRecording" attribute.
    bool    isRecording;
    /// Returns the position in samples of the first sample of the current
    /// buffer since the beginning of the song.
    /// Can be accessed directly as "positionInSamples" attribute.
    int64   positionInSamples;
    /// returns the position in quarter notes of the first sample of the
    /// current buffer since the beginning of the song.
    /// Can be accessed directly as "positionInQuarterNotes" attribute.
    double  positionInQuarterNotes;
    /// Returns the position in seconds of the first sample of the current
    /// buffer since the beginning of the song.
    /// Can be accessed directly as "positionInSeconds" attribute.
    double  positionInSeconds;
    /// Returns the position in quarter notes of the first bar of the current measure.
    /// Can be accessed directly as "currentMeasureDownBeat" attribute.
    double  currentMeasureDownBeat;
    ///  When looping, returns the position in quarter notes of the beginning of the loop.
    /// Can be accessed directly as "loopStart" attribute.
    double  loopStart;
    /// When looping, returns the position in quarter notes of the end of the loop.
    /// Can be accessed directly as "loopEnd" attribute.
    double  loopEnd;
};

#ifdef __cplusplus
#define MidiQueueRef MidiQueue&
#else
#define MidiQueueRef MidiQueue*
#endif

/** Structure passed to the script for block processing.
 *
 */
struct BlockData
{
    /// An array containing audio buffers of each audio channel for this block.
    /// You can access sample i of channel ch using samples[ch][i].
    double**                    samples;
    /// The number of audio samples to process for this block.
    uint                        samplesToProcess;
    /// The incoming MIDI events queue.
    const struct MidiQueueRef   inputMidiEvents;
    /// The MIDI events output queue to send MIDI events.
    struct MidiQueueRef         outputMidiEvents;
    /// The input parameters values at the beginning of the block.
    const double*               beginParamValues;
    /// The input parameters values at the ends of the block.
    const double*               endParamValues;
    /// Transport information - may be null if not supported or not provided
    /// by the host application.
    const struct TransportInfo* transport;
};

// C API definition
#ifdef __cplusplus
#define EXTERN_C extern "C"
#else
#define EXTERN_C
#endif

#ifdef _MSC_VER
#define DSP_EXPORT EXTERN_C __declspec(dllexport)
#else
#define DSP_EXPORT EXTERN_C __attribute__ ((visibility("default")))
#endif

typedef void (HostPrintFunc)(void* hostImpl,const char* message);

DSP_EXPORT void*          host;
DSP_EXPORT HostPrintFunc* hostPrint;

// for angelscript compatibility
static INLINE_API void print(const char* message)
{
    // be paranoid
    if(hostPrint!=null && host!=null)
        hostPrint(host,message);
}

#endif
