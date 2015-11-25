/**
 *  C++ Helper classes and for angelscript compatibility.
 *  Copyright (c) 2015 Blue Cat Audio. All rights reserved.
 */

#ifndef _cpphelpers_h
#define _cpphelpers_h

#include "dspapi.h"

#ifdef __cplusplus

// C++11 initializer lists compiler check
#ifdef _MSC_VER
#if _MSC_VER>=1800 // VS2013 required
#define CPP11_INITIALIZERS
#endif
#elif defined(__clang__)
#if __has_feature(cxx_generalized_initializers)
#define CPP11_INITIALIZERS
#endif
#elif defined(__GNUG__) &&  (__GNUC__ >= 4) && (__GNUC_MINOR__ >=4) // supported starting with GCC 4.4
#define CPP11_INITIALIZERS
#endif

#ifdef CPP11_INITIALIZERS
#include <initializer_list>
#endif

/** Simple array template class that can be used to define
 *  parameters, names and other script definition arrays with the same syntax as
 *  angelscript.
 *  This is not a smart array or vector implementation, just a simple helper template to build 
 *  your parameters list easily (C++11 recommended for full compatibility with angelscript syntax).
 */
template <typename T>
struct array
{
    // expected C-style struct data----------
    T*      ptr;
    uint    length;
    
    // Helper C++ constructors------------------
    
    /// constructor with size
    array(uint size=0);
    
    /// constructor with size and default valur for elements
    array(uint size,const T& defaultValue);
    
    /// constructor that takes a fixed size array
    /// can be used to construct the array from a fixed size C array
    template <int N>
    array(const T(&iArray)[N]);
    
#ifdef CPP11_INITIALIZERS
    /// C++11 iitializer list support
    array(const std::initializer_list<T>& list);
#endif

	array(const array<T>& iArray);
	array<T>& operator =(const array<T>& iArray);

    ///destructor
    ~array();
    
    // Helper C++ functions------------------
    
    /// array style accessor
    T& operator       [](uint i);
    
    /// array style accessor (const)
    const T& operator [](uint i)const;
    
    /// resize array (re-allocation and copy using the = operator)
    void resize(uint newSize);
    
    /// find element in array and returns its index or -1 if not found
    int find(const T& elem)const;
    
};

template <typename T>
array<T>::array(uint size):
ptr(null),
length(size)
{
    if (length != 0)
        ptr = new T[length];
}

template <typename T>
array<T>::array(uint size, const T& defaultValue):
ptr(null),
length(size)
{
    if (length != 0)
        ptr = new T[length];
    for(uint i=0;i<length;i++)
    {
        ptr[i]=defaultValue;
    }
}

template <typename T>
template <int N>
array<T>::array(const T(&iArray)[N]):
ptr(null),
length(N)
{
    if (length != 0)
        ptr = new T[length];
    for(uint i=0;i<N;i++)
    {
        ptr[i]=iArray[i];
    }
}

#ifdef CPP11_INITIALIZERS
template <typename T>
array<T>::array(const std::initializer_list<T>& list):
length(static_cast<uint>(list.size()))
{
    if (length != 0)
    {
        ptr = new T[length];
        uint i = 0;
        for (typename std::initializer_list<T>::const_iterator iter = list.begin(); iter != list.end(); iter++)
        {
            ptr[i] = (*iter);
            i++;
        }
    }
}
#endif

template <typename T>
array<T>::array(const array<T>& iArray) :
	length(iArray.length)
{
	if (length != 0)
	{
		ptr = new T[length];
		for (uint i=0; i<length; i++)
		{
			ptr[i] = iArray[i];
			i++;
		}
	}
}

template <typename T>
array<T>& array<T>::operator =(const array<T>& iArray)
{
	// cleanup if different length
	if (iArray.length != length)
	{
		length = iArray.length;
		delete[] ptr;
		ptr = null;
	}
	if (length != 0)
	{
		if(ptr==null)
			ptr = new T[length];
		for (uint i = 0; i<length; i++)
		{
			ptr[i] = iArray[i];
			i++;
		}
	}
	return *this;
}

template <typename T>
array<T>::~array()
{
    delete[] ptr;
    ptr = 0;
}

template <typename T>
T& array<T>::operator [](uint i)
{
    return ptr[i];
}

template <typename T>
const T& array<T>::operator [](uint i)const
{
    return ptr[i];
}

template <typename T>
void array<T>::resize(uint newSize)
{
    if(newSize!=length)
    {
        T* oldPtr=ptr;
        ptr=0;
        if(newSize>0)
        {
            ptr=new T[newSize];
            if(oldPtr!=0)
            {
                for(uint i=0;i<length;i++)
                {
                    ptr[i]=oldPtr[i];
                }
            }
        }
        delete [] oldPtr;
        length=newSize;
    }
}

template <typename T>
int array<T>::find(const T& elem)const
{
    int index=-1;
    for(uint i=0;i<length;i++)
    {
        if(ptr[i]==elem)
        {
            index=i;
            break;
        }
    }
    return index;
}

/** Array Descriptor class - basic array descriptor as required by host,
 *  without all the bells and whistles.
 */
template <typename T>
struct ArrayDescriptor
{
    // C-struct layout
    T*      ptr;
    uint    length;
    
    // utility constructor
    ArrayDescriptor():ptr(null),length(0){};
};

#endif
#endif
