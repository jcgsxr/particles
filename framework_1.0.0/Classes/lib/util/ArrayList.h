#ifndef __ARRAYLIST_H__
#define __ARRAYLIST_H__

#include "systemdefines.h"
#include "types.h"


// Convenient declaration macros
// Usage: ARRAYLIST2D(char) my2DArray; is equivalent to ArrayList<ArrayList<char> > my2DArray;

#define ARRAYLIST(t)	ArrayList<t> 
#define ARRAYLIST2D(t)	ARRAYLIST(ArrayList<t> )
#define ARRAYLIST3D(t)	ARRAYLIST(ARRAYLIST2D(t) )

#ifdef DEBUG
#define DEBUG_BLOCK(code)			{code}
#else
#define DEBUG_BLOCK(code)
#endif

#define DEBUG_BLOCK_TAG(tag, code)	DEBUG_BLOCK(if (tag == 1){code})
#define DEBUG_BREAK()				//DEBUG_BLOCK({__asm{ int 3}})
#define REF_ASSERT(condition)		DEBUG_BLOCK(if(!(condition)){DBGPRINTF("REF_ASSERT FAILED: %s", TOSTRING(condition)); DEBUG_BREAK();})


///////////////////////////////////////////////////////////
// Array Reference class
///////////////////////////////////////////////////////////
template <class T> class ArrayList
{
protected:

	T* pRef;
	int* pCount;
	int m_length;

	///////////////////////////////////////////////////////////
	// Instantiates reference metadata when raw pointer is
	// assigned to the array reference
	///////////////////////////////////////////////////////////
	inline void _initRefData(T* ref)
	{
		if(ref)
		{			
			pRef = ref;
			pCount = new int;
			*pCount = 1;
			m_length = 0;
		}
	}
	
	///////////////////////////////////////////////////////////
	// Increments reference count on assignment 
	///////////////////////////////////////////////////////////
	inline void _addRef()
	{
		if (pCount)
			++*pCount; 
	}

	///////////////////////////////////////////////////////////
	// Decrements reference count when an ArrayList object
	// releases ownership
	///////////////////////////////////////////////////////////
	inline void _release()
	{
		if (pCount)
		{
			if (--*pCount == 0)
			//if (pCount && --*pCount == 0)
			{
				if (pRef)
				{
					delete[] pRef;
					pRef = NULL;
				}
				delete pCount;
				pCount = NULL;
			}		
		}
	}

public:

	///////////////////////////////////////////////////////////
	// Default constructor
	///////////////////////////////////////////////////////////
	ArrayList() : pRef(NULL), pCount(NULL), m_length(0)	
	{ 
		DPRINT_REFERENCE("ArrayList() \n");
	}

	///////////////////////////////////////////////////////////
	// Raw pointer initialization constructor
	///////////////////////////////////////////////////////////
	ArrayList(T* ref) : pRef(NULL), pCount(NULL), m_length(0)
	{ 
		DPRINT_REFERENCE("ArrayList(T*) \n"); 
		_initRefData(ref);
	}

	///////////////////////////////////////////////////////////
	// Copy constructor
	///////////////////////////////////////////////////////////
	ArrayList(const ArrayList &ref) : pRef(ref.pRef), pCount(ref.pCount), m_length(ref.m_length) 
	{ 
		DPRINT_REFERENCE("ArrayList(ArrayList&) \n"); 
		_addRef();
	}

	///////////////////////////////////////////////////////////
	// Destructor
	// - Calls release when this object is destroyed
	///////////////////////////////////////////////////////////
	~ArrayList()
	{ 
		DPRINT_REFERENCE("~ArrayList() \n");
		_release();
	}	

	///////////////////////////////////////////////////////////
	// Raw pointer assignment operator
	// - Assumes reference was freshly allocated and initializes
	//   reference count to one
	///////////////////////////////////////////////////////////
	void operator=(T* ref)
	{
		DPRINT_REFERENCE("operator=(T*) \n");

		_release();		
		pRef = NULL;
		pCount = NULL;
		m_length = 0;
		_initRefData(ref);		
	}
	
	///////////////////////////////////////////////////////////
	// ArrayList assignment operator
	// - Copies parameters reference data structure and
	//   increments reference count
	///////////////////////////////////////////////////////////
	void operator=(const ArrayList& ref)
	{
		DPRINT_REFERENCE("operator=(ArrayList&) \n");

		_release();
		pRef = ref.pRef;
		pCount = ref.pCount;
		m_length = ref.m_length;
		_addRef();
	}

	///////////////////////////////////////////////////////////
	// Allocates an array of type T with specified size
	// You must use this method if you want to be able to use length()
	// Usage:	ArrayList<MyType> ref = ArrayList<MyType>::alloc(size);
	///////////////////////////////////////////////////////////
	static ArrayList alloc(int size)
	{
		REF_ASSERT(size > 0);
		T* p_array = new T[size];
		memset(p_array, 0, sizeof(T) * size);
		
		REF_ASSERT(p_array != NULL);
		ArrayList ret(p_array);
		ret.m_length = size;

		return ret;
	}
	
	///////////////////////////////////////////////////////////
	// Equality operator
	// Useful when doing NULL pointer/reference checks
	///////////////////////////////////////////////////////////
	inline bool operator ==(T* ptr) const
	{
		return pRef == ptr;
	}

	///////////////////////////////////////////////////////////
	// Inequality operator
	///////////////////////////////////////////////////////////
	inline bool operator !=(T* ptr) const
	{
		return !operator==(ptr);
	}

	///////////////////////////////////////////////////////////
	// Array access operator 
	///////////////////////////////////////////////////////////
	inline T& operator[](int i) const
	{
		REF_ASSERT(pRef != NULL);
		REF_ASSERT(i < m_length && i >= 0);
		return pRef[i]; 
	}

	///////////////////////////////////////////////////////////
	// Disowns reference
	///////////////////////////////////////////////////////////
	inline void disown() 
	{ 	
		if (pCount)
		{
			delete pCount;
			pCount = NULL;
		}
	}

	///////////////////////////////////////////////////////////
	// Sets the length of the array
	///////////////////////////////////////////////////////////
	inline void setLength(int length)
	{
		m_length = length;
	}
	
	///////////////////////////////////////////////////////////
	// Raw Pointer access
	// TODO: convert this method to a cast operator
	///////////////////////////////////////////////////////////
	inline T* getRawPtr() const 
	{ 
		return pRef; 
	}

#if defined (_DEBUG_) && defined (DEBUG_MMAP)
	inline operator void*()
	{
		return pRef;
	}
#endif

	///////////////////////////////////////////////////////////
	// Returns current objects reference count 
	///////////////////////////////////////////////////////////
	inline int getRefCount() const 
	{ 
		REF_ASSERT(pCount != NULL);
		return *pCount; 
	}

	///////////////////////////////////////////////////////////
	// Returns length of array 
	///////////////////////////////////////////////////////////
	inline int length() const 
	{
		return m_length; 
	}
	
	void clear()
	{
		if (pRef)
		{
			delete[] pRef;
			pRef = NULL;
		}
	}
};


#endif