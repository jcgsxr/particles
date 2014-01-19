//////////////////////////////////////////////////////////////////////////////////////////////////
//																								//
// StringI.h																					//
//																								//
//////////////////////////////////////////////////////////////////////////////////////////////////
//
//	A simple string class.  The concept here is to make a very simple wrapper class for 
//	sprintf and other string manipulation functions in C.  The wrapper is StringI, which 
//	is a template interface.  This allows inheritance of an interface without having to rely on 
//	expensive virtualization.  Since strings can be numerous, having virtualization will incur 
//	overhead, so this avoids it. 
//
//	Our default strings are fixed length strings to avoid memory management problems with heap 
//	allocated vectorized strings. 
//
//	The main use of this strings is for small printouts that typically happen on the fly 
//	during game development.  For larger strings, use the standard techniques or generate a new 
//	inner class similar to StringFixed with the basic controller methods (copy, concat, clear, etc) 
//	defined. 
//
//	This interface attempts to make Java ports easier by mimicing its interface.  Operator+ 
//	and operator= overloads are defined to allow for Java style concatenations.  However beaware 
//	that this style: 
//
//	CString str; 
//	str = "SOMETHING " + intValue; 
//
//	will not work!  You must start the expression with a String object for operator overloads to work 
//	correctly.  Just a C++ design limitation.  Use valueOf() as you would in Java to directly assign 
//	numeric values into a string without concatenation. 
// 
////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _STRINGI_H_
#define _STRINGI_H_

#include <stdio.h>
#include "types.h"


static const int SMALL_STR_SIZE = 64;
// This file got a little sloppy when I was trying to debug a widescreen text problem.
// I have left the remnants of some debug printing in case someone else needs it later.

template <class T>
class StringI
{
public:
	StringI() { reset(); }
	//may want to auto clear the string? maybe too expensive
	StringI(const T& str) 
	{
		m_str = str;
	}
	StringI(const StringI& str) 
	{
		operator=(str); 
	}
	
#ifdef STRING_OVERFLOW_DEBUG
	StringI(const char* str) { m_str.copy(str, (unsigned short)strlen(str), FALSE); }
	StringI(const char* str, unsigned short len){ m_str.copy(str, len, FALSE); }
#else
	StringI(const char* str) { m_str.copy(str, (unsigned short)strlen(str)); }
	StringI(const char* str, unsigned short len){ m_str.copy(str, len); }
#endif
	StringI(const int i) {reset(); operator+(i);}
	StringI(const char* str, int index, unsigned short size) { if (str == NULL) return; m_str.copy(&str[index], size); }
	
	//
	// BEWARE: operator+ is really operator+=
	//
	StringI& operator+(int i) { char str[SMALL_STR_SIZE]; sprintf(str, "%d", i); m_str.concat(str, (unsigned short)strlen(str)); return *this; }
	StringI& operator+(short i) { char str[SMALL_STR_SIZE]; sprintf(str, "%d", i); m_str.concat(str, (unsigned short)strlen(str)); return *this; }
	StringI& operator+(char i) { char str[SMALL_STR_SIZE]; sprintf(str, "%c", i); m_str.concat(str, (unsigned short)strlen(str)); return *this; }
	StringI& operator+(bool i) { char str[SMALL_STR_SIZE]; sprintf(str, "%c", i); m_str.concat(str, (unsigned short)strlen(str)); return *this; }
	StringI& operator+(long i) { char str[SMALL_STR_SIZE]; sprintf(str, "%l", i); m_str.concat(str, (unsigned short)strlen(str)); return *this; }
	inline StringI& operator+(const StringI& str) { m_str.concat(str, str.length()); return *this; }
	inline StringI& operator+(const char* str){ m_str.concat(str, (unsigned short)strlen(str)); return *this; }
	inline StringI& operator += (const StringI& str) {m_str.concat(str, str.length()); return *this;}
	inline StringI& operator += (const char* str){ m_str.concat(str, (unsigned short)strlen(str)); return *this; }
	inline StringI& operator += (const char i){ char str[SMALL_STR_SIZE]; sprintf(str, "%c", i); m_str.concat(str, (unsigned short)strlen(str)); return *this; }
	inline StringI& operator += (const int i){ char str[SMALL_STR_SIZE]; sprintf(str, "%d", i); m_str.concat(str, (unsigned short)strlen(str)); return *this; }
	
	
#ifdef STRING_OVERFLOW_DEBUG
	StringI& operator=(const char* pStr){ assign(pStr, (unsigned short)strlen(pStr), TRUE); return *this; }
#else
	StringI& operator=(const char* pStr){ assign(pStr, (unsigned short)strlen(pStr)); return *this; }
#endif
	StringI& operator=(const int i)
	{
		char intval[16];
		sprintf(intval, "%d", i);
#ifdef STRING_OVERFLOW_DEBUG
		assign(intval, (unsigned short)strlen(intval), TRUE);
#else
		assign(intval, (unsigned short)strlen(intval));
#endif
		return *this;
	}
	
	
#ifdef STRING_OVERFLOW_DEBUG
	void assign(const char *pStr, unsigned short len, bool bDebugPrint)
#else
	void assign(const char *pStr, unsigned short len)
#endif
	{
		if (pStr != m_str.getData())
		{
#ifdef STRING_OVERFLOW_DEBUG
			if ( bDebugPrint )
				DPRINT_STRINGI("String m_str size   : %d", sizeof(m_str));
			m_str.copy(pStr, len, bDebugPrint);
#else
			m_str.copy(pStr, len);
#endif
		}
	}
	
	static StringI valueOf(int i) { char str[SMALL_STR_SIZE]; sprintf(str, "%d", i); return StringI(str, (unsigned short)strlen(str)); }
	static StringI valueOf(short i) { char str[SMALL_STR_SIZE]; sprintf(str, "%d", i); return StringI(str, (unsigned short)strlen(str)); }
	static StringI valueOf(long i){ char str[SMALL_STR_SIZE]; sprintf(str, "%d", i); return StringI(str, (unsigned short)strlen(str)); }
	
	static StringI valueOf(bool i) { char str[SMALL_STR_SIZE]; sprintf(str, "%d", i); return StringI(str, (unsigned short)strlen(str)); }
	static StringI valueOf(char i){ char str[SMALL_STR_SIZE]; sprintf(str, "%c", i); return StringI(str, (unsigned short)strlen(str)); }
	inline StringI& concat(const StringI& str) { m_str.concat(str.toCharArray(), length()); return *this; }
	inline StringI& concat(const char* pStr, unsigned short len){ m_str.concat(pStr, len); return *this; }
	
	inline int compareTo(const StringI& str) { return compareTo(str.toCharArray(), str.length()); } //i will not overload the == operator because of pointer comparisons
	inline int compareTo(const char* pStr, unsigned short len) { if (STRNCMP(pStr, toCharArray(), len) == 0) return 1; return 0; }
	
	inline char* toCharArray() { return m_str.getData(); }
	inline const char* toCharArray() const{ return m_str.getConstData(); }
	inline const char* toCharArray(int i) { return m_str.getData(i); }
	inline operator const char*() { return (const char*) (this->m_str.getConstData()); }
	
	inline bool operator == (StringI& str)
	{
		return equals(str);
	}
	
	inline bool operator == (const StringI& str)
	{
		return equals(str.toCharArray());
	}
	
	inline short length(){ return (short)strlen(m_str.getData()); }
	
	inline char charAt(int i) { return m_str.getData()[i]; }
	
	inline int indexOf(char c) { return m_str.indexOf(c); }
	
	inline void reset() { m_str.clear(); }
	
	inline void substring(StringI &out_data, int start, int end)
	{
#ifdef STRING_OVERFLOW_DEBUG
		out_data.m_str.copy(m_str.getConstData(start), (unsigned short)(end - start), FALSE);
#else
		out_data.m_str.copy(m_str.getConstData(start), (unsigned short)(end - start));
#endif
	}
	
	inline const char* substring(int start)
	{
		return toCharArray(start);
	}
	
	inline const char* substring(int start, int length)
	{
		StringI out_data;
#ifdef STRING_OVERFLOW_DEBUG
		out_data.m_str.copy(m_str.getConstData(start), (unsigned short)(start + length), FALSE);
#else
		out_data.m_str.copy(m_str.getConstData(start), (unsigned short)(start + length));
#endif		
		return out_data.toCharArray();
	}
	
	inline const char* trim()
	{
		char *start = toCharArray();
		char *end = toCharArray() + length() - 1;		// point to the last char
		while (*start == ' ' || *start == '\n' || *start == '\r' || *start == '\t')			// WARNING: check if this breaks anything else
			start++;			// first trim the start
		while (*end == ' ' || *end == '\n' || *end == '\r' || *end == '\t')				// any whitespace
			end--;				// then find the last non-whitespace
		end++;					// go past the end
		*end = '\0';			// and nullify
		return start;
	}
	
	inline int equals(T &str) { return compareTo(str); }
	inline int equals(const char* str) 
	{ 
		if (!str)
		{
			str = "";
		}
		
		return compareTo(str, (unsigned short)strlen(str)); 
	}
	
	inline bool startsWith(const char* str) { return (bool)STRBEGINS(str, toCharArray()); }
	inline bool endsWith(const char* str) { return (bool)STRENDS(str, toCharArray()); }
	
	inline int size() {return m_str.size();}
	
private:
	
	T m_str;
};




// default template parameters not supported by RVCT 1.2
template <int TSize>
class StringFixed
{
	friend class StringI< StringFixed<TSize> >;
public:
	StringFixed() { clear(); }
	StringFixed(const char* str, unsigned short len){ copy(str, len); }
	
	StringFixed& concat(const char* pStr, unsigned short len)
	{
		int mylen = strlen(m_arrStr);
		if (mylen >= (TSize-1))
			return *this;			// TWW string is full, maybe we should assert
		if (mylen + len > (TSize - 1))
		{
			// DPRINT_STRINGI("%s: cropped string from %d to %d", __FUNCTION__, len, len - (TSize - mylen - 1));
			if ( 0 > (s16)(len - TSize - mylen - 1) )
				len = (u16)(len - TSize - mylen - 1);
			else
				len = 0;
		}
		if (len > 0)		// so we don't overflow our string
		{
			memcpy(m_arrStr + mylen, pStr, len);
			m_arrStr[mylen + len] = 0;
		}
		
		return *this;
	}
	
	inline char* getData(int offset = 0) { return &m_arrStr[offset]; }
	inline const char* getConstData(int offset = 0) const {return &m_arrStr[offset]; }
	
	inline void clear() { MEMSET(m_arrStr, 0, sizeof(m_arrStr)); }
#ifdef STRING_OVERFLOW_DEBUG
	void copy(const char* pStr, unsigned short size, bool bDebugPrint)
#else
	void copy(const char* pStr, unsigned short size)
#endif
	{
		if (size + 1 > TSize)
		{
			// DPRINT_STRINGI("%s: cropped string from %d to %d", __FUNCTION__, size, TSize - 1);
#ifdef STRING_OVERFLOW_DEBUG
			const char overflow[] = {"LINE OVERFLOW"};
			memcpy(m_arrStr, overflow, strlen(overflow));		// debug only
			return;
#endif
			size = TSize - 1;
		}
#ifdef STRING_OVERFLOW_DEBUG
		if ( bDebugPrint )
		{
			DPRINT_STRINGI("FixedString size   : %d", sizeof(m_arrStr));
			DPRINT_STRINGI("copy size   : %d", size);
		}
#endif
		memcpy(m_arrStr, pStr, size);
#ifdef STRING_OVERFLOW_DEBUG
		if ( bDebugPrint )
		{
			DPRINT_STRINGI("%s", m_arrStr);
			if ( strlen(m_arrStr) > 40 )
				DPRINT_STRINGI("%s", &m_arrStr[39]);
			DPRINT_STRINGI("m_arrStr size   : %d", strlen(m_arrStr));
			DPRINT_STRINGI("%s", pStr);
			if ( strlen(pStr) > 40 )
				DPRINT_STRINGI("%s", &pStr[39]);
			DPRINT_STRINGI("pStr size   : %d", strlen(pStr));
		}
#endif
		m_arrStr[size] = 0;
	}
	
	void copy(const char* pStr)
	{
		int size = strlen(pStr);
		// make sure we don't overflow our buffer
		if (size + 1 > TSize)
		{
#ifdef STRING_OVERFLOW_DEBUG
			const char overflow[] = {"LINE OVERFLOW"};
			memcpy(m_arrStr, overflow, strlen(overflow));		// debug only
			return;
#endif
			size = TSize - 1;
		}
		memcpy(m_arrStr, pStr, size);
	}
	
	int indexOf(const char c)
	{
		char *ptr_value = STRCHR(m_arrStr, c);
		if (ptr_value == NULL)
			return 0;
		return (ptr_value - m_arrStr);		// return pointer offset
	}
	
	int size() {return TSize;}
	
private:
	char m_arrStr[TSize];
};



//typedef StringI< StringFixed<256> > String;


#endif

