#ifndef __FLOATBUFFER_H__
#define __FLOATBUFFER_H__

#include "MathUtil.h"
#include "ArrayList.h"


class FloatBuffer
{
private:
	ArrayList<float> buffer;
	ArrayList<int>	fp_buffer;	// The fixed point buffer.
	int		length;

public:
	FloatBuffer(){buffer = NULL;  length = 0;}
	FloatBuffer(int size)
	{
		buffer = NULL;
		buffer = ArrayList<float>::alloc(size); 

		fp_buffer = ArrayList<int>::alloc(size); 

		length = size;
	}
	//~FloatBuffer(){release();}
	
	////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Allocates our buffer as an array of the indicated size.
	void allocate(int capacity)
	{			
		release();
		REF_ASSERT(capacity > 0);
		if (capacity <= 0)
		{
			DPRINT_FLOATBUFFER("===+++ ERROR TRYING TO ALLOCATE() - FLOATBUFFER +++===");
			return;
		}
			
		buffer = ArrayList<float>::alloc(capacity); 

		length = capacity;
		fp_buffer = NULL;
		fp_buffer = ArrayList<int>::alloc(capacity); 
	}
	
	////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Copies value f into into the indicated index in our buffer.
	void put(int index, float f)
	{
		if (index >= 0 && index < length)
		{
			buffer[index] = f;
			fp_buffer[index] = FPTOXP(f);
		}
		else
		{
			DPRINT_FLOATBUFFER("===+++ ERROR TRYING TO PUT() - FLOATBUFFER +++===");
		}

	}
	
	////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Copies the contents of an array into our buffer.  The offset is where in our buffer we want the
	// copy to begin. This is used as a wrapper for the java version of the FloatBuffer put method.
	void put(ArrayList<float>& f, int offset, int f_length)
	{
		if (offset >= 0 && offset < length && offset + f_length <= length)
		{
			for (int i = 0; i < f_length ; i++)
			{
				if (i == length)
				{
					return;
				}
				
				buffer[offset + i] = f[i];
				fp_buffer[offset + i] = FPTOXP(f[i]);
			}
		}
	}


	////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Returns the current length of our buffer.
	int getLength(void){return length;}
	
	////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Returns our buffer.
	int* getIntBuf(void)
	{
		// Generate the fixed-point buffer values and return that.
		return fp_buffer.getRawPtr();
	}

	float* getFloatBuf(void)
	{
		return buffer.getRawPtr();
	}

	
	////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Releases our buffer.
	void release(void)
	{
		buffer = NULL;
		fp_buffer = NULL;
		length = 0;
	}
};


#endif