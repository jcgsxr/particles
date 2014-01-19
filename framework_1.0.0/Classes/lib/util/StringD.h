// String class for creation and use of strings of dynamic size.
 

#ifndef _STRINGD_H_
#define _STRINGD_H_


#include <string.h>
#include "StringI.h"
#include "types.h"
#include "ArrayList.h"

class StringD
{
public:

	StringD() 
	{ 
		m_arrStr = NULL;
		m_length = 0;
	}

	StringD(const char* str)
	{
		m_arrStr = (char*)str;
		m_arrStr.disown();
		m_length = strlen(str);
		m_arrStr.setLength(m_length + 1);		
	}

	StringD(char* str)
	{
		m_arrStr = NULL;
		m_length = 0;
		copy(str);
	}

	StringD(const StringD& str)
	{
		m_arrStr = str.m_arrStr;
		m_length = str.length();		
	}

	~StringD()
	{
		clear();
	}

	inline void clear() 
	{ 	
		m_arrStr = NULL;
		m_length = 0;
	}

	inline int indexOf(const char c)
	{
		char *ptr_value = strchr(m_arrStr.getRawPtr(), c);

		// Not found.
		if (ptr_value == NULL)
			return -1;

		return (ptr_value - m_arrStr.getRawPtr());		// return pointer offset
	}

	inline char charAt(int i)
	{
		REF_ASSERT(i < m_length);
		return m_arrStr[i];
	}

	inline char* toCharArray() const
	{
		return m_arrStr.getRawPtr();
	}

	/*inline operator char*()
	{
		return m_arrStr;
	}*/

	template <class T> inline void operator=(StringI<T>& str)
	{
		copy(str.toCharArray());
	}

	// Assigns a const char null-terminated string to this object
	// This will simply copy the address of the string and will not attempt to free it
	// If you want StringD to make its own copy of the string then assign a non-const char pointer
	inline StringD& operator=(const char* str)
	{
		m_arrStr = (char*)str;
		m_arrStr.disown();
		m_length = strlen(str);
		m_arrStr.setLength(m_length + 1);
		return *this;
	}

	// Makes a copy of string
	inline StringD& operator=(char* str)
	{
		copy(str);
		return *this;
	}

	inline StringD& operator=(const StringD& str)
	{
		copy(str.toCharArray());
		return *this;
	}

	inline bool operator ==(const StringD& str)
	{
		return (str.m_arrStr.getRawPtr() == m_arrStr.getRawPtr() || strcmp(str.m_arrStr.getRawPtr(), m_arrStr.getRawPtr()) == 0);
	}

	//inline StringD operator+(const char* str)
	//{
	//	return concat(str);
	//}

	//inline StringD operator+(StringD& str)
	//{
	//	return concat(str.toCharArray());
	//}

	inline StringD& operator+=(char c)
	{
		*this = concat(&c);
		return *this;
	}

	inline StringD& operator+=(const char* str)
	{
		*this = concat(str);
		return *this;
	}

	inline StringD& operator+=(const StringD& str)
	{
		*this = concat(str.toCharArray());
		return *this;
	}

	inline int equals(const char* str)
	{ 
		if (strncmp(str, toCharArray(), m_length) == 0)
			return 1; 
		return 0; 
	}

	inline bool startsWith(const char* str) 
	{
		const char* tmpstr1 = str;
		const char* tmpstr2 = toCharArray();
		while (tmpstr1 && tmpstr2)
		{
			if (tmpstr1 != tmpstr2)
			{
				return FALSE;
			}
			tmpstr1++;
			tmpstr2++;
		}
		return TRUE;
		//return (bool)strbegins(str, toCharArray()); 
	}

	inline bool endsWith(const char* str) 
	{ 
		const char* tmpstr1 = str;
		const char* tmpstr2 = toCharArray();
		int strlen1 = strlen(str);
		int strlen2 = length();
		
		while (strlen2)
		{
			if (tmpstr1[strlen1] != tmpstr2[strlen2])
			{
				return FALSE;
			}
			strlen1--;
			strlen2--;
		}
		return FALSE;
		//return (bool)strends(str, toCharArray()); 
	}


	inline StringD concat(const char* str)
	{
		StringD result;

		if (str)
		{
			result.m_length = m_length + strlen(str);
			result.m_arrStr = ArrayList<char>::alloc(result.m_length + 1);

 			strcpy(result.m_arrStr.getRawPtr(), m_arrStr.getRawPtr());
			strcat(result.m_arrStr.getRawPtr(), str);
		}
		else
		{
			result.copy(m_arrStr.getRawPtr());
		}

		return result;
	}

	inline StringD substring(int beginIndex)
	{
		return substring(beginIndex, m_length);
	}

	inline StringD substring(int beginIndex, int endIndex)
	{
		StringD result;
		result.m_length = endIndex - beginIndex;
		if (result.m_length > 0)
		{
			result.m_arrStr = ArrayList<char>::alloc(result.m_length + 1);
			memcpy(result.m_arrStr.getRawPtr(), m_arrStr.getRawPtr() + beginIndex, result.m_length);
			result.m_arrStr[result.m_length] = '\0';
		}
		else
		{
			result = "";
		}
		return result;
	}

	int length() const{ return m_length; }

private:

	inline void copy(const char* str)
	{
		clear();
		if (str)
		{
			m_length = strlen(str);
			if (m_length > 0)
			{
				m_arrStr = ArrayList<char>::alloc(m_length+1); //MMAP(m_arrStr);
				strcpy(m_arrStr.getRawPtr(), str);
			}
			else
			{
				*this = "";
			}
		}
	}

	ArrayList<char> m_arrStr;
	uint16 m_length;
};


typedef StringD String; // Dynamic string type

#endif

