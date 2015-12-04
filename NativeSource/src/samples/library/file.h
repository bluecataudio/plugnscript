#ifndef _file_h_
#define _file_h_

#include <string>
#include <string.h>
#include <stdio.h>

/** emulation of the angelscript file API, for compatibility with angelscript.
 *  Adapted from the angelscript file extension source code (scriptfile.cpp).
 */
struct file
{
    bool    mostSignificantByteFirst=false;
    FILE    *f=NULL;
    
    int open(const std::string& filename, const std::string& iMode)
    {
        // Close the previously opened file handle
        if( f )
            close();
        
        std::string myFilename = filename;
        std::string mode=iMode;
        
        // Validate the mode
        std::string m;
        if( mode != "r" && mode != "w" && mode != "a" )
                return -1;
            else
                m = mode;
        
        // By default windows translates "\r\n" to "\n", but we want to read the file as-is.
        m += "b";
        
        // Open the file
#if _MSC_VER >= 1400 && !defined(__S3E__)
        // MSVC 8.0 / 2005 introduced new functions
        // Marmalade doesn't use these, even though it uses the MSVC compiler
        fopen_s(&f, myFilename.c_str(), m.c_str());
#else
        f = fopen(myFilename.c_str(), m.c_str());
#endif
        if( f == 0 )
            return -1;
        
        return 0;
    }
    
    int close()
    {
        if( f == 0 )
            return -1;
        
        fclose(f);
        f = 0;
        
        return 0;
    }
    
    int getSize() const
    {
        if( f == 0 )
            return -1;
        
        long pos = ftell(f);
        fseek(f, 0, SEEK_END);
        long size = ftell(f);
        fseek(f, pos, SEEK_SET);
        
        return (int)size;
    }
    
    int getPos() const
    {
        if( f == 0 )
            return -1;
        
        return (int)ftell(f);
    }
    
    int setPos(int pos)
    {
        if( f == 0 )
            return -1;
        
        int r = fseek(f, pos, SEEK_SET);
        
        // Return -1 on error
        return r ? -1 : 0;
    }
    
    int movePos(int delta)
    {
        if( f == 0 )
            return -1;
        
        int r = fseek(f, delta, SEEK_CUR);
        
        // Return -1 on error
        return r ? -1 : 0;
    }
    
    int readString(unsigned int length, std::string &str)
    {
        if( f == 0 )
            return 0;
        
        // Read the string
        str.resize(length);
        int size = (int)fread(&str[0], 1, length, f);
        str.resize(size);
        
        return size;
    }
    
    int readLine(std::string &str)
    {
        if( f == 0 )
            return 0;
        
        // Read until the first new-line character
        str = "";
        char buf[256];
        
        do
        {
            // Get the current position so we can determine how many characters were read
            int start = (int)ftell(f);
            
            // Set the last byte to something different that 0, so that we can check if the buffer was filled up
            buf[255] = 1;
            
            // Read the line (or first 255 characters, which ever comes first)
            char *r = fgets(buf, 256, f);
            if( r == 0 ) break;
            
            // Get the position after the read
            int end = (int)ftell(f);
            
            // Add the read characters to the output buffer
            str.append(buf, end-start);
        }
        while( !feof(f) && buf[255] == 0 && buf[254] != '\n' );
        
        return int(str.size());
    }
    
    int64 readInt(uint bytes)
    {
        if( f == 0 )
            return 0;
        
        if( bytes > 8 ) bytes = 8;
        if( bytes == 0 ) return 0;
        
        unsigned char buf[8];
        size_t r = fread(buf, bytes, 1, f);
        if( r == 0 ) return 0;
        
        int64 val = 0;
        if( mostSignificantByteFirst )
        {
            unsigned int n = 0;
            for( ; n < bytes; n++ )
                val |= int64(buf[n]) << ((bytes-n-1)*8);
            
            // Check the most significant byte to determine if the rest
            // of the qword must be filled to give a negative value
            if( buf[0] & 0x80 )
                for( ; n < 8; n++ )
                    val |= int64(0xFF) << (n*8);
        }
        else
        {
            unsigned int n = 0;
            for( ; n < bytes; n++ )
                val |= int64(buf[n]) << (n*8);
            
            // Check the most significant byte to determine if the rest
            // of the qword must be filled to give a negative value
            if( buf[bytes-1] & 0x80 )
                for( ; n < 8; n++ )
                    val |= int64(0xFF) << (n*8);
        }
        
        return val;
    }
    
    int64 readUInt(uint bytes)
    {
        if( f == 0 )
            return 0;
        
        if( bytes > 8 ) bytes = 8;
        if( bytes == 0 ) return 0;
        
        unsigned char buf[8];
        size_t r = fread(buf, bytes, 1, f);
        if( r == 0 ) return 0;
        
        int64 val = 0;
        if( mostSignificantByteFirst )
        {
            unsigned int n = 0;
            for( ; n < bytes; n++ )
                val |= int64(buf[n]) << ((bytes-n-1)*8);
        }
        else
        {
            unsigned int n = 0;
            for( ; n < bytes; n++ )
                val |= int64(buf[n]) << (n*8);
        }
        
        return val;
    }
    
    float readFloat()
    {
        if( f == 0 )
            return 0;
        
        unsigned char buf[4];
        size_t r = fread(buf, 4, 1, f);
        if( r == 0 ) return 0;
        
        uint val = 0;
        if( mostSignificantByteFirst )
        {
            unsigned int n = 0;
            for( ; n < 4; n++ )
                val |= uint(buf[n]) << ((3-n)*8);
        }
        else
        {
            unsigned int n = 0;
            for( ; n < 4; n++ )
                val |= uint(buf[n]) << (n*8);
        }
        
        return *reinterpret_cast<float*>(&val);
    }
    
    double readDouble()
    {
        if( f == 0 )
            return 0;
        
        unsigned char buf[8];
        size_t r = fread(buf, 8, 1, f);
        if( r == 0 ) return 0;
        
        int64 val = 0;
        if( mostSignificantByteFirst )
        {
            unsigned int n = 0;
            for( ; n < 8; n++ )
                val |= int64(buf[n]) << ((7-n)*8);
        }
        else
        {
            unsigned int n = 0;
            for( ; n < 8; n++ )
                val |= int64(buf[n]) << (n*8);
        }
        
        return *reinterpret_cast<double*>(&val);
    }
    
    bool isEndOfFile() const
    {
        if( f == 0 )
            return true;
        
        return feof(f) ? true : false;
    }
    
    int writeString(const std::string& str)
    {
        if( f == 0 )
            return -1;
        
        // Write the entire string
        size_t r=fwrite(&str[0], 1, str.length(), f);
        return int(r);
    }
    
    int writeInt(int64 val, uint bytes)
    {
        if( f == 0 )
            return 0;
        
        unsigned char buf[8];
        if( mostSignificantByteFirst )
        {
            for( unsigned int n = 0; n < bytes; n++ )
                buf[n] = (val >> ((bytes-n-1)*8)) & 0xFF;
        }
        else
        {
            for( unsigned int n = 0; n < bytes; n++ )
                buf[n] = (val >> (n*8)) & 0xFF;
        }
        
        size_t r = fwrite(&buf, bytes, 1, f);
        return int(r);
    }
    
    int writeUInt(int64 val, uint bytes)
    {
        if( f == 0 )
            return 0;
        
        unsigned char buf[8];
        if( mostSignificantByteFirst )
        {
            for( unsigned int n = 0; n < bytes; n++ )
                buf[n] = (val >> ((bytes-n-1)*8)) & 0xFF;
        }
        else
        {
            for( unsigned int n = 0; n < bytes; n++ )
                buf[n] = (val >> (n*8)) & 0xFF;
        }
        
        size_t r = fwrite(&buf, bytes, 1, f);
        return int(r);
    }
    
    int writeFloat(float fl)
    {
        if( f == 0 )
            return 0;
        
        unsigned char buf[4];
        uint val = *reinterpret_cast<uint*>(&fl);
        if( mostSignificantByteFirst )
        {
            for( unsigned int n = 0; n < 4; n++ )
                buf[n] = (val >> ((3-n)*4)) & 0xFF;
        }
        else
        {
            for( unsigned int n = 0; n < 4; n++ )
                buf[n] = (val >> (n*8)) & 0xFF;
        }
        
        size_t r = fwrite(&buf, 4, 1, f);
        return int(r);
    }
    
    int writeDouble(double d)
    {
        if( f == 0 )
            return 0;
        
        unsigned char buf[8];
        int64 val = *reinterpret_cast<int64*>(&d);
        if( mostSignificantByteFirst )
        {
            for( unsigned int n = 0; n < 8; n++ )
                buf[n] = (val >> ((7-n)*8)) & 0xFF;
        }
        else
        {
            for( unsigned int n = 0; n < 8; n++ )
                buf[n] = (val >> (n*8)) & 0xFF;
        }
        
        size_t r = fwrite(&buf, 8, 1, f);
        return int(r);
    }
};
#endif