/** 
*  \file WaveFile.hxx
*  Simple wave file utilities for angelscript.
* 
*  Created by Blue Cat Audio <services@bluecataudio.com>
*  \copyright 2014-2016 Blue Cat Audio. All rights reserved.
*
*  Supports most 8/16/24/32/64-bit wave files but probably not all.
*  
*  Note: This file requires the file and array angelscript add-ons.
*
*  - WaveFileData: load an entire wave file into memory or write
*   a wave file to disk from memory with a single call.
*  - WaveFileReader: read a wave file from disk sample after sample.
*   Use this class to stream audio data from disk.
*   - WaveFileWriter: write a wave file to disk sample after sample.
*   Use this class to stream audio data to disk.
*   - WaveFileHeader: utility class to load ans save wave file header.
*/


/** Simple wave file loader/saver (limited support for simple wav files).
*
*/
class WaveFileData
{
    array<double> interleavedSamples;   ///< interleaved audio samples
    uint64        channelsCount;        ///< number of audio channels
    double        sampleRate;           ///< the sample rate of the audio data

    /// length property (number of samples)
    int length
    {
        get const
        {
            if(channelsCount!=0)
                return interleavedSamples.length/channelsCount;
            else
                return 0;
        }
    }

    /** load entire audio file from disk into the interleavedSamples array.
    *   This method allocates memory and may take a while,
    *   so this should not be called from the real time audio thread.
    */
    bool loadFile(string filePath)
    {
        bool ok=false;
        // open the file
        file f;
        if(f.open(filePath,"r")>=0)
        {
            // read the header
            WaveFileHeader header;
            if(header.read(f))
            {
                ok=true;
                // store file data
                channelsCount=header.channelsCount;
                sampleRate=header.sampleRate;
                interleavedSamples.length=header.samplesCount*channelsCount;

                switch(header.bytesPerSample)
                {
                    // 8-bit wav files use only positive values (0 to 255)
                case 1:
                    {
                        for (uint i = 0; i < header.samplesCount; i++)
                        {
                            for(uint ch=0;ch<channelsCount;ch++)
                            {
                                interleavedSamples[i+ch]=(double(f.readUInt(header.bytesPerSample))-128.0)/128.0;
                            }
                        }
                        break;
                    }
                    // single precision floating point wav file
                case 4:
                    {
                        for (uint i = 0; i < header.samplesCount; i++)
                        {
                            for(uint ch=0;ch<channelsCount;ch++)
                            {
                                interleavedSamples[i+ch]=f.readFloat();
                            }
                        }
                        break;
                    }
                case 8:
                    // double precision floating point wav file
                    {
                        for (uint i = 0; i < header.samplesCount; i++)
                        {
                            for(uint ch=0;ch<channelsCount;ch++)
                            {
                                interleavedSamples[i+ch]=f.readDouble();
                            }
                        }
                        break;
                    }
                    // 16 or 24-bit integer files
                default:
                    {
                        uint64 maxValue=(1<<(header.bytesPerSample*8-1))-1;
                        double factor=1.0/double(maxValue);
                        for (uint i = 0; i < header.samplesCount; i++)
                        {
                            for(uint ch=0;ch<channelsCount;ch++)
                            {
                                int64 value=f.readInt(header.bytesPerSample);
                                interleavedSamples[i+ch]=double(value)*factor;
                            }
                        }
                        break;
                    }
                }
            }
            f.close();
        }
        return ok;
    }

    /** Write the audio data in interleavedSamples to disk as a wav file.
    *   This method allocates memory and may take a while,
    *   so this should not be called from the real time audio thread.
    *   bitDepth is the number of bits for each sample in the wave file.
    */
    bool writeFile(string filePath, uint bitDepth=16)
    {
        bool ok=false;
        // open file to write
        file f;
        if(f.open(filePath,"w")>=0)
        {
            // write header
            WaveFileHeader header;
            header.bytesPerSample=bitDepth/8;
            header.sampleRate=uint64(sampleRate);
            header.samplesCount=interleavedSamples.length/channelsCount;
            header.channelsCount=channelsCount;
            header.write(f);
            // write samples
            switch(header.bytesPerSample)
            {
            case 1:
                {
                    // 8-bit wav files use only positive values (0 to 255)
                    for (uint i = 0; i < header.samplesCount; i++)
                    {
                        for(uint ch=0;ch<header.channelsCount;ch++)
                        {
                            f.writeUInt(uint64(interleavedSamples[i+ch]*128.0+128.0),1);
                        }
                    }
                    break;
                }
                // single precision floating point wav file
            case 4:
                {
                    for (uint i = 0; i < header.samplesCount; i++)
                    {
                        for(uint ch=0;ch<header.channelsCount;ch++)
                        {
                            f.writeFloat(float(interleavedSamples[i+ch]));
                        }
                    }
                    break;
                }
            case 8:
                // double precision floating point wav file
                {
                    for (uint i = 0; i < header.samplesCount; i++)
                    {
                        for(uint ch=0;ch<header.channelsCount;ch++)
                        {
                            f.writeDouble(interleavedSamples[i+ch]);
                        }
                    }
                    break;
                }
            default:
                {
                    double maxValue=(1<<(header.bytesPerSample*8-1))-1;
                    double factor=double(maxValue);
                    for (uint i = 0; i < header.samplesCount; i++)
                    {
                        for(uint ch=0;ch<header.channelsCount;ch++)
                        {
                            int64 value=int64(interleavedSamples[i+ch]*maxValue);
                            f.writeInt(value,header.bytesPerSample);
                        }
                    }
                    break;
                }
            }
            ok=true;
            f.close();
        }
        return ok;
    }
};

/** Simple (sample per sample) wave file reader.
*   Note: does not handle resampling.
*/
class WaveFileReader
{
    /** open a file for streaming audio data from it.
    *   maxChannelsCount is the maximum number of channels that will be read from the
    *   file. Set to -1 if you intend to use the number of channels of the file.
    */
    bool openFile(string filePath, int maxChannelsCount=-1)
    {
        bool ok=false;
        if(f.open(filePath,"r")>=0)
        {
            if(header.read(f))
            {
                ok=true;
                blockSize=header.channelsCount*header.bytesPerSample;
                if(header.bytesPerSample>1 && header.bytesPerSample<4)
                {
                    uint64 maxValue=(1<<(header.bytesPerSample*8-1))-1;
                    gainFactor=1.0/double(maxValue);
                }
                if(maxChannelsCount>0 && uint(maxChannelsCount)<header.channelsCount)
                {
                    channelsToRead=maxChannelsCount;
                    channelsToSkip=(header.channelsCount-maxChannelsCount);
                }
                else
                {
                    channelsToRead=header.channelsCount;
                    channelsToSkip=0;
                }
            }
        }
        return ok;
    }

    /** Read one sample from the current position in the file.
    *   Assumes that the file was succesfuly opened.
    *   oSample is an array with at least maxChannelsCount (@see openFile),
    *   or the number of channels containedin the file (@see get_channelsCount).
    */
    void readSample(array<double>& oSample)
    {
        switch(header.bytesPerSample)
        {
            // 8-bit wav file contain only positive values.
        case 1:
            {
                for(uint ch=0;ch<channelsToRead;ch++)
                {
                    oSample[ch]=(double(f.readUInt(header.bytesPerSample))-128.0)/128.0;
                }
                for(uint ch=0;ch<channelsToSkip;ch++)
                {
                    f.readUInt(header.bytesPerSample);
                }

                break;
            }
            // single precision floating point wav file
        case 4:
            {
                for(uint ch=0;ch<channelsToRead;ch++)
                {
                    oSample[ch]=f.readFloat();
                }
                for(uint ch=0;ch<channelsToSkip;ch++)
                {
                    f.readFloat();
                }
                break;
            }
        case 8:
            // double precision floating point wav file
            {
                for(uint ch=0;ch<channelsToRead;ch++)
                {
                    oSample[ch]=f.readDouble();
                }
                for(uint ch=0;ch<channelsToSkip;ch++)
                {
                    f.readDouble();
                }
                break;
            }
            // 16 or 24-bit integer wav file
        default:
            {
                for(uint ch=0;ch<channelsToRead;ch++)
                {
                    oSample[ch]=double(f.readInt(header.bytesPerSample))*gainFactor;
                }
                for(uint ch=0;ch<channelsToSkip;ch++)
                {
                    f.readInt(header.bytesPerSample);
                }
                break;
            }
        }
    }
    /** Move file cursor to sample number "samplePosition".
    *   Warning: does not check if end of data chunk is reached.
    */
    bool setPos(uint samplePosition)
    {
        return f.setPos(header.headerSize+samplePosition*blockSize)>=0;
    }

    /** Move file cursor by "sampleOffset" samples.
    *   returns false if failed.
    *   Warning: does not check if end of data chunk is reached.
    */
    bool movePos(int sampleOffset)
    {
        int newPos=f.getPos()+sampleOffset*blockSize;
        if(newPos>=int(header.headerSize))
            return f.setPos(sampleOffset*blockSize)>=0;
        else
            return false;
    }

    /** Check if end of file reached.
    *
    */
    bool isEndOfFile()
    {
        return f.isEndOfFile();
    }

    /** close the audio file.
    *
    */
    bool close()
    {
        return f.close()>=0;
    }

    /** channelsCount property (read only).
    *
    */
    int get_channelsCount()const
    {
        return header.channelsCount;
    }

    // private data
    private file    f; // the file
    private uint    blockSize=0; // sample block size (all channels)
    private uint    channelsToRead=0; // number of channels to read from file
    private uint    channelsToSkip=0; // number of channels to skip (channels not used)
    private double gainFactor=1; // gain factor for integer files
    private WaveFileHeader header; // the wave file header data
};

/** Simple (sample per sample) wave file writer.
*   
*/
class WaveFileWriter
{
    /// header data to be used to write the file.
    WaveFileHeader header;

    /** Open the file for writing and pushes the current header data.
    *   Note: header data will be updated on close, but file format cannot be changed
    *   after the file has been opened for writing.
    */
    bool openFile(string filePath)
    {
        // reinit header
        header.samplesCount=0;

        // open the file
        bool ok=false;
        if(f.open(filePath,"w")>=0)
        {
            if(header.write(f))
            {
                ok=true;
                if(header.bytesPerSample>1 && header.bytesPerSample<4)
                {
                    uint64 maxValue=(1<<(header.bytesPerSample*8-1))-1;
                    gainFactor=double(maxValue);
                }
            }
        }
        return ok;
    }

    /** Write one sample at the current position in the file.
    *   Assumes that the file was succesfuly opened.
    *   iSample is an array with at least the number of channels set in the header.
    */
    void writeSample(const array<double>& iSample)
    {
        // increment samples count
        header.samplesCount++;

        // write data
        switch(header.bytesPerSample)
        {
            // 8-bit wav file
        case 1:
            {
                for(uint ch=0;ch<header.channelsCount;ch++)
                {
                    f.writeUInt(uint64(iSample[ch]*128.0+128.0),1);
                }
                break;
            }
            // single precision floating point wav file
        case 4:
            {
                for(uint ch=0;ch<header.channelsCount;ch++)
                {
                    f.writeFloat(iSample[ch]);
                }
                break;
            }
        case 8:
            // double precision floating point wav file
            {
                for(uint ch=0;ch<header.channelsCount;ch++)
                {
                    f.writeDouble(iSample[ch]);
                }
                break;
            }
        default:
            {
                for(uint ch=0;ch<header.channelsCount;ch++)
                {
                    f.writeInt(int64(iSample[ch]*gainFactor),header.bytesPerSample);
                }
                break;
            }
        }
    }

    bool close()
    {
        // flush header to update file size
        f.setPos(0);
        header.write(f);

        // close file
        return f.close()>=0;
    }

    // destructor ensures that the file is properly closed,
    // with updated header, before the object is destroyed.
    ~WaveFileWriter()
    {
        close();
    }

    // private data
    private file    f; ///< the audio file.
    private double  gainFactor=1; ///< factor used for integer wav files.
};

/** Utility class to handle Wave file header data.
*   Used by the classes above.
*   Ignores LIST and INFO chunks.
*/
class WaveFileHeader
{
    uint64  channelsCount=0;        // number of audio channels
    uint64  samplesCount=0;         // number of audio samples
    uint64  sampleRate=0;           // the sample rate of the audio data
    uint    bytesPerSample=0;       // the number of bytes per individual channel sample      
    uint    headerSize=0;           // the size of the header (offset to audio data)

    /** Read header data from file.
    *   Assumes that the file is at position 0.
    */
    bool read(file& f)
    {
        bool ok=false;
        string tempString;
        tempString=f.readString(4);
        if(tempString=="RIFF")
        {
            int64 size=f.readUInt(4);
            tempString=f.readString(4);
            if(tempString=="WAVE")
            {
                tempString=f.readString(4);
                if(tempString=="fmt ")
                {
                    uint64 fmtSize = f.readUInt(4);
                    if(fmtSize>=16) // PCM format size is 16 or 18
                    {
                        uint64 dataOffset=fmtSize-16;
                        uint64 format=f.readInt(2);
                        if(format==1 || format==3) //uncompressed PCM format or float are supported
                        {
                            channelsCount=f.readUInt(2);
                            sampleRate=f.readUInt(4);
                            uint64 byteRate=f.readUInt(4);
                            uint64 blockSize=f.readUInt(2);
                            bytesPerSample = f.readUInt(2)/8;
                            // end of subchunk
                            f.movePos(dataOffset);

                            // looking for data subchunk => skipping all others 
                            tempString=f.readString(4); // data id
                            while(tempString!="data" && !f.isEndOfFile())
                            {
                                uint64 dataSize = f.readUInt(4);
                                f.movePos(dataSize);        // skip junk data
                                tempString=f.readString(4); // read next chunk id
                            }
                            if(tempString=="data")
                            {
                                uint64 dataSize = f.readUInt(4);
                                samplesCount=dataSize/blockSize;
                            }
                            headerSize=f.getPos();
                            ok=true;
                        }
                    }
                }
            }
        }
        return ok;
    }

    /** Write header data to file.
    *   Assumes that the file is at position 0.
    */
    bool write(file& f)
    {
        f.writeString("RIFF");
        int64 dataSize=(bytesPerSample)*samplesCount*channelsCount;
        f.writeUInt(36+dataSize,4);
        f.writeString("WAVE");
        f.writeString("fmt ");
        f.writeUInt(16,4);
        //uncompressed PCM format or float
        if(bytesPerSample>=4)
            f.writeInt(3,2);
        else
            f.writeInt(1,2);
        f.writeUInt(channelsCount,2);
        f.writeUInt(sampleRate,4);
        f.writeUInt(sampleRate*channelsCount * bytesPerSample,4); // byte rate
        f.writeUInt(channelsCount * bytesPerSample,2);
        f.writeUInt(bytesPerSample*8,2);

        // data subchunk  
        f.writeString("data"); // data id
        f.writeUInt(dataSize,4);
        return true;
    }
};
