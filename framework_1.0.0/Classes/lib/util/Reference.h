///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// Reference.h                                                               //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// Mimics Java array and object references as well as providing automatic    //
// memory management. An object is returned to the heap when its reference   //
// count goes to zero.                                                       //
//                                                                           //
// Caveat:                                                                   //
// 1. Does not handle cyclical references.                                   //
// 2. Avoid mixing references and raw pointer types.                         //
// 3. When allocating an object, assign it directly to a Reference or        //
//    ArrayList. If you do the following, your object will be reclaimed early //
//                                                                           //
//    Wrong way:                                                             //
//    CFoo* foo = new CFoo();                                                //
//    Reference<CFoo> rFooA = foo;  // OK, foo's reference count is 1        //
//    Reference<CFoo> rFooB = foo;	// WRONG, foo's ref count is still 1     //
//    rFooA = NULL;  // BAD, ref count is 0, object reclaimed                //
//    rFooB->fooFunction()   // VERY BAD, null pointer access                //
//                                                                           //
//    Right way:                                                             //
//    Reference<CFoo> rFooA = new CFoo();  // OK, ref count is 1             //
//    Reference<CFoo> rFooB = rFooA;       // OK, ref count is 2             //
//    rFooA = NULL; // OK, ref count is 1, object is still alive             //
//    rFooB->fooFunction()  // OK                                            //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

#ifndef __REFERENCE_H__
#define __REFERENCE_H__



///////////////////////////////////////////////////////////
// Reference class
///////////////////////////////////////////////////////////
template <class T> class Reference
{
protected:

	T* pRef;
	int* pCount;

	///////////////////////////////////////////////////////////
	// Instantiates reference metadata when raw pointer is
	// assigned to the array reference
	///////////////////////////////////////////////////////////
	inline void _initRefData(T* ref)
	{
		if(ref)
		{			
			pRef = ref;
			pCount = new int; //MMAP(pCount);
			*pCount = 1;
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
		if (pCount && --*pCount == 0)
		{
			if (pRef)
			{
				delete pRef;
				pRef = NULL;
			}
			delete pCount;
			pCount = NULL;
		}		
	}

public:
	
	///////////////////////////////////////////////////////////
	// Default constructor
	///////////////////////////////////////////////////////////
	Reference() : pRef(NULL), pCount(NULL) 
	{ 
		DPRINT_REFERENCE("Reference()"); 
	}

	///////////////////////////////////////////////////////////
	// Raw pointer initialization constructor
	///////////////////////////////////////////////////////////
	Reference(T* ref) : pRef(NULL), pCount(NULL) 
	{ 
		DPRINT_REFERENCE("Reference(T*)"); 
		_initRefData(ref); 
	}
	
	///////////////////////////////////////////////////////////
	// Copy constructor
	///////////////////////////////////////////////////////////
	Reference(const Reference &ref) : pRef(ref.pRef), pCount(ref.pCount)
	{ 
		DPRINT_REFERENCE("Reference(Reference&)"); 
		_addRef(); 
	}

	///////////////////////////////////////////////////////////
	// Destructor
	// - Calls release when this object is destroyed
	///////////////////////////////////////////////////////////
	~Reference() 
	{ 
		DPRINT_REFERENCE("~Reference()");
		_release();
	}

	///////////////////////////////////////////////////////////
	// Raw pointer assignment operator
	// - Assumes object was freshly allocated and initializes
	//   reference count to one
	///////////////////////////////////////////////////////////
	void operator=(T* ref)
	{
		DPRINT_REFERENCE("operator=(T*)");

		_release();		
		pRef = NULL;
		pCount = NULL;
		_initRefData(ref);
	}

	///////////////////////////////////////////////////////////
	// Reference assignment operator
	// - Copies parameters reference data structure and
	//   increments reference count
	///////////////////////////////////////////////////////////
	void operator=(const Reference& ref)
	{
		DPRINT_REFERENCE("operator=(Reference<T>&)");

		_release();
		pRef = ref.pRef;
		pCount = ref.pCount;
		_addRef();
	}

	///////////////////////////////////////////////////////////
	// Equality operator
	// Useful when doing NULL pointer/reference checks
	///////////////////////////////////////////////////////////
	inline bool operator ==(T* ptr)
	{
		return pRef == ptr;
	}

	///////////////////////////////////////////////////////////
	// Inequality operator
	// Useful when doing NULL pointer/reference checks
	///////////////////////////////////////////////////////////
	inline bool operator !=(T* ptr) 
	{ 
		return !operator==(ptr);
	}

	///////////////////////////////////////////////////////////
	// Equality operator
	// Useful when doing NULL pointer/reference checks
	///////////////////////////////////////////////////////////
	inline bool operator ==(const Reference<T>& ref)
	{
		return pRef == ref.pRef;
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
	// Cast to raw pointer operator
	///////////////////////////////////////////////////////////
	inline operator T*()
	{
		return pRef;	
	}

	///////////////////////////////////////////////////////////
	// Pointer access operator
	///////////////////////////////////////////////////////////
	inline T* operator->() const
	{ 
		return pRef; 
	}

	///////////////////////////////////////////////////////////
	// Dereferencing operator
	///////////////////////////////////////////////////////////
	inline T& operator*() const 
	{
		REF_ASSERT(pRef != NULL);
		return *pRef;
	}

	///////////////////////////////////////////////////////////
	// Returns raw pointer
	///////////////////////////////////////////////////////////
	inline T* getRawPtr() const 
	{ 
		return pRef; 
	}

	///////////////////////////////////////////////////////////
	// Returns current objects reference count 
	///////////////////////////////////////////////////////////
	inline int getRefCount() const
	{
		REF_ASSERT(pCount != NULL);
		return *pCount; 
	}
};



#endif		// REFERENCE_H
