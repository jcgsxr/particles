//////////////////////////////////////////////////////////////////////////////
//                                                                          //
// HashTable.h																//
//                                                                          //
//////////////////////////////////////////////////////////////////////////////
//                                                                          //
// This class mimics the Java HashTable class. Gets and Puts should be used	//
// in conjunction with CRC32::Make to get unique key values.				//
//                                                                          //
//                                                                          //
// Caveats:                                                                 //
// 1. The size must be allocated after the HashTable is declared.			//
// 2. The table must be cleared by calling clear() to prevent leaks.		//
//                                                                          //
//    Example:																//
//    int hash_tbl_size = 256;												//
//    // Initialize a hash table of CFoos of size hash_tbl_size				//
//    HastTable<CFoo> FooA = HashTable<CFoo>::alloc(hash_tbl_size);			//
//    // Put some element into the hash table with an associated key.		//
//    FooA.put(CRC32::Make("idstring"), blah_elem);							//
//    // Get the element by handing it the same key.						//
//    CFoo *blah_elem2 = FooA.get(CRC32::Make("idstring"));					//
//    // To prevent memory leaks, clear must be called when the table is no	//
//    // longer used.														//
//    FooA.clear();															//
//                                                                          //
//////////////////////////////////////////////////////////////////////////////

#ifndef __HASHTABLE_H__
#define __HASHTABLE_H__


static const int HASHTABLE_SIZE_LIMIT = 512;

template <class T> struct HashTableListNode
{
	T *data;
	int keyValue;
	HashTableListNode<T> *next;
};

template <class T> struct HashTableList
{
	HashTableListNode<T> *first;
	HashTableListNode<T> *last;
	HashTableListNode<T> *curr;
};


/////////////////////////
template <class T> class HashTable
{
protected:

	HashTableList<T> **mapListArray;
	int _table_size;
	
	// A simple hash function.
	int getBucketValue(unsigned long hashKey)
	{
		// We'll use a simple Knuth variant on the division method.
		return ((hashKey * (hashKey + 3)) % _table_size);
	}

public:

	HashTable() : _table_size(HASHTABLE_SIZE_LIMIT)
	{
		mapListArray = new HashTableList<T>*[HASHTABLE_SIZE_LIMIT];
		MEMSET(mapListArray, 0, sizeof(HashTableList<T>*) * HASHTABLE_SIZE_LIMIT);
		for (int i = 0; i < HASHTABLE_SIZE_LIMIT; ++i)
		{
			mapListArray[i] = new HashTableList<T>;
			MEMSET(mapListArray[i], 0, sizeof(HashTableList<T>));
		}
	}

	HashTable(int size) : _table_size(size)
	{
		mapListArray = new HashTableList<T>*[size];
		MEMSET(mapListArray, 0, sizeof(HashTableList<T>*) * size);
		for (int i = 0; i < size; ++i)
		{
			mapListArray[i] = new HashTableList<T>;
			MEMSET(mapListArray[i], 0, sizeof(HashTableList<T>));
		}
	}

	int length()
	{
		return _table_size;
	}

	static HashTable alloc(int size)
	{
		HashTable ret(size);
		return ret;
	}


	// Returns the value to which the specified keyValue is mapped in this hashtable. 
	T* get(unsigned long keyValue)
	{
		int bucketValue = 0;
		T* data = NULL;

		bucketValue = getBucketValue(keyValue);

		// Check for an empty bucket. If so, return null.
		if (!mapListArray[bucketValue]->first)
		{
			return NULL;
		}

		// Point the current node back to the first node in case we need to traverse through the linked list when a collision is encountered.
		mapListArray[bucketValue]->curr = mapListArray[bucketValue]->first;

		// Point to the data in the current node first, then we can check whether this is the actual data that we want after.
		data = mapListArray[bucketValue]->curr->data;

		// Check for a match in the current bucket, in case there was a collision during set up.
		while (mapListArray[bucketValue]->curr->keyValue != keyValue)
		{
			// Move to the next node.
			mapListArray[bucketValue]->curr = mapListArray[bucketValue]->curr->next;

			// Check whether we have reached the end of the list, indicating that we were not able to find the matching bucket with the desired data.
			if (mapListArray[bucketValue]->curr == NULL)
			{
				DPRINT_HASHTABLE("HashTable::get() data not found with key:%d", keyValue);
				return NULL;
			}

			// This is where we have found the matching node in the bucket (since there was a collision), and return the data.
			if (mapListArray[bucketValue]->curr->keyValue == keyValue)
			{
				data = mapListArray[bucketValue]->curr->data;
				// Reset the current node pointer back to the first node.
				mapListArray[bucketValue]->curr = mapListArray[bucketValue]->first;
			}
		}

		return data;
	}

	// Maps the specified keyValue to the specified data in this hashtable. Neither the keyValue nor the data can be null. 
	void put(unsigned long keyValue, T *data)
	{
		int bucketValue = 0;

		bucketValue = getBucketValue(keyValue);

		// If there is anything in the first node of the bucket, it means we must handle a collision.
		if (mapListArray[bucketValue]->first)
		{
			DPRINT_HASHTABLE("HashTable::put() Hash table collision at bucket:%d with key:%d", bucketValue, keyValue);

			// Point the current node back to the first one so we can find the first available spot to store the data in the bucket.
			mapListArray[bucketValue]->curr = mapListArray[bucketValue]->first;

			// Loop until an open spot is found.
			while (mapListArray[bucketValue]->curr->next != NULL)
			{
				// Traverse the list but pointing to the next node here.
				mapListArray[bucketValue]->curr = mapListArray[bucketValue]->curr->next;
			}

			DPRINT_HASHTABLE("HashTable::put() creating new map list node after collision");

			// Create the new node.
			mapListArray[bucketValue]->curr->next = new HashTableListNode<T>;

			// Store the data immediately after the node has been created.
			mapListArray[bucketValue]->curr->next->data = data;
			//memcpy(&mapListArray[bucketValue]->curr->next->data, data, sizeof(T));

			// Store the key value, used to help resolve collisions on gets.
			mapListArray[bucketValue]->curr->next->keyValue = keyValue;
		}
		else
		{
			// This case is reached when the bucket is empty and no collisions have occured. This is the easy case.
			mapListArray[bucketValue]->first = new HashTableListNode<T>;

			// Store the data immediately after the node has been created.
			mapListArray[bucketValue]->first->data = data;
			//memcpy(&mapListArray[bucketValue]->first->data, data, sizeof(T));

			// Store the key value, used to help resolve collisions on gets.
			mapListArray[bucketValue]->first->keyValue = keyValue;

			// We can safely set the pointer to the next node to be null since this bucket is empty.
			mapListArray[bucketValue]->first->next = NULL;
			
			// Set the current node to point back to the first as a safety measure.
			mapListArray[bucketValue]->curr = mapListArray[bucketValue]->first;
		}
	}


	// Clears this hashtable so that it contains no keys. 
	void clear()
	{
		int bucketValue = 0;
		HashTableListNode<T> *temp_next;

		for (int i = 0; i < _table_size; ++i)
		{
			mapListArray[i]->curr = mapListArray[i]->first;

			if(mapListArray[i]->curr == NULL)
			{
				// Nothing to clear, move on to the next list.
				continue;
			}

			do
			{
				// Save the poitision of the next node so we don't lose it when we delete the current node.
				temp_next = mapListArray[i]->curr->next;

				// Destroy the current node, we already checked that it isn't null above.
				delete mapListArray[i]->curr->data;
				mapListArray[i]->curr->data = NULL;
				delete mapListArray[i]->curr;
				
				if (temp_next == NULL)
				{
					// Once a null node has been reached, we know that there is no more data in the linked list. 
					// We can safely traverse the next list so we will just break.
					mapListArray[i]->first = NULL;
					mapListArray[i]->last = NULL;
					mapListArray[i]->curr = NULL;
					break;
				}

				// Go to the next node.
				mapListArray[i]->curr = temp_next;
			} 
			while (mapListArray[i]->curr != NULL);
		}
		

		// Delete the array itself.
		for (int i = 0; i < _table_size; ++i)
		{
			if (mapListArray[i])
			{
				delete mapListArray[i];
				mapListArray[i] = NULL;
			}
		}
		if (mapListArray)
		{
			delete[] mapListArray;
			mapListArray = NULL;
		}

		_table_size = 0;
	}
};




static unsigned long crc32_tab[] = 
{
	0x00000000L, 0x77073096L, 0xee0e612cL, 0x990951baL, 0x076dc419L,
	0x706af48fL, 0xe963a535L, 0x9e6495a3L, 0x0edb8832L, 0x79dcb8a4L,
	0xe0d5e91eL, 0x97d2d988L, 0x09b64c2bL, 0x7eb17cbdL, 0xe7b82d07L,
	0x90bf1d91L, 0x1db71064L, 0x6ab020f2L, 0xf3b97148L, 0x84be41deL,
	0x1adad47dL, 0x6ddde4ebL, 0xf4d4b551L, 0x83d385c7L, 0x136c9856L,
	0x646ba8c0L, 0xfd62f97aL, 0x8a65c9ecL, 0x14015c4fL, 0x63066cd9L,
	0xfa0f3d63L, 0x8d080df5L, 0x3b6e20c8L, 0x4c69105eL, 0xd56041e4L,
	0xa2677172L, 0x3c03e4d1L, 0x4b04d447L, 0xd20d85fdL, 0xa50ab56bL,
	0x35b5a8faL, 0x42b2986cL, 0xdbbbc9d6L, 0xacbcf940L, 0x32d86ce3L,
	0x45df5c75L, 0xdcd60dcfL, 0xabd13d59L, 0x26d930acL, 0x51de003aL,
	0xc8d75180L, 0xbfd06116L, 0x21b4f4b5L, 0x56b3c423L, 0xcfba9599L,
	0xb8bda50fL, 0x2802b89eL, 0x5f058808L, 0xc60cd9b2L, 0xb10be924L,
	0x2f6f7c87L, 0x58684c11L, 0xc1611dabL, 0xb6662d3dL, 0x76dc4190L,
	0x01db7106L, 0x98d220bcL, 0xefd5102aL, 0x71b18589L, 0x06b6b51fL,
	0x9fbfe4a5L, 0xe8b8d433L, 0x7807c9a2L, 0x0f00f934L, 0x9609a88eL,
	0xe10e9818L, 0x7f6a0dbbL, 0x086d3d2dL, 0x91646c97L, 0xe6635c01L,
	0x6b6b51f4L, 0x1c6c6162L, 0x856530d8L, 0xf262004eL, 0x6c0695edL,
	0x1b01a57bL, 0x8208f4c1L, 0xf50fc457L, 0x65b0d9c6L, 0x12b7e950L,
	0x8bbeb8eaL, 0xfcb9887cL, 0x62dd1ddfL, 0x15da2d49L, 0x8cd37cf3L,
	0xfbd44c65L, 0x4db26158L, 0x3ab551ceL, 0xa3bc0074L, 0xd4bb30e2L,
	0x4adfa541L, 0x3dd895d7L, 0xa4d1c46dL, 0xd3d6f4fbL, 0x4369e96aL,
	0x346ed9fcL, 0xad678846L, 0xda60b8d0L, 0x44042d73L, 0x33031de5L,
	0xaa0a4c5fL, 0xdd0d7cc9L, 0x5005713cL, 0x270241aaL, 0xbe0b1010L,
	0xc90c2086L, 0x5768b525L, 0x206f85b3L, 0xb966d409L, 0xce61e49fL,
	0x5edef90eL, 0x29d9c998L, 0xb0d09822L, 0xc7d7a8b4L, 0x59b33d17L,
	0x2eb40d81L, 0xb7bd5c3bL, 0xc0ba6cadL, 0xedb88320L, 0x9abfb3b6L,
	0x03b6e20cL, 0x74b1d29aL, 0xead54739L, 0x9dd277afL, 0x04db2615L,
	0x73dc1683L, 0xe3630b12L, 0x94643b84L, 0x0d6d6a3eL, 0x7a6a5aa8L,
	0xe40ecf0bL, 0x9309ff9dL, 0x0a00ae27L, 0x7d079eb1L, 0xf00f9344L,
	0x8708a3d2L, 0x1e01f268L, 0x6906c2feL, 0xf762575dL, 0x806567cbL,
	0x196c3671L, 0x6e6b06e7L, 0xfed41b76L, 0x89d32be0L, 0x10da7a5aL,
	0x67dd4accL, 0xf9b9df6fL, 0x8ebeeff9L, 0x17b7be43L, 0x60b08ed5L,
	0xd6d6a3e8L, 0xa1d1937eL, 0x38d8c2c4L, 0x4fdff252L, 0xd1bb67f1L,
	0xa6bc5767L, 0x3fb506ddL, 0x48b2364bL, 0xd80d2bdaL, 0xaf0a1b4cL,
	0x36034af6L, 0x41047a60L, 0xdf60efc3L, 0xa867df55L, 0x316e8eefL,
	0x4669be79L, 0xcb61b38cL, 0xbc66831aL, 0x256fd2a0L, 0x5268e236L,
	0xcc0c7795L, 0xbb0b4703L, 0x220216b9L, 0x5505262fL, 0xc5ba3bbeL,
	0xb2bd0b28L, 0x2bb45a92L, 0x5cb36a04L, 0xc2d7ffa7L, 0xb5d0cf31L,
	0x2cd99e8bL, 0x5bdeae1dL, 0x9b64c2b0L, 0xec63f226L, 0x756aa39cL,
	0x026d930aL, 0x9c0906a9L, 0xeb0e363fL, 0x72076785L, 0x05005713L,
	0x95bf4a82L, 0xe2b87a14L, 0x7bb12baeL, 0x0cb61b38L, 0x92d28e9bL,
	0xe5d5be0dL, 0x7cdcefb7L, 0x0bdbdf21L, 0x86d3d2d4L, 0xf1d4e242L,
	0x68ddb3f8L, 0x1fda836eL, 0x81be16cdL, 0xf6b9265bL, 0x6fb077e1L,
	0x18b74777L, 0x88085ae6L, 0xff0f6a70L, 0x66063bcaL, 0x11010b5cL,
	0x8f659effL, 0xf862ae69L, 0x616bffd3L, 0x166ccf45L, 0xa00ae278L,
	0xd70dd2eeL, 0x4e048354L, 0x3903b3c2L, 0xa7672661L, 0xd06016f7L,
	0x4969474dL, 0x3e6e77dbL, 0xaed16a4aL, 0xd9d65adcL, 0x40df0b66L,
	0x37d83bf0L, 0xa9bcae53L, 0xdebb9ec5L, 0x47b2cf7fL, 0x30b5ffe9L,
	0xbdbdf21cL, 0xcabac28aL, 0x53b39330L, 0x24b4a3a6L, 0xbad03605L,
	0xcdd70693L, 0x54de5729L, 0x23d967bfL, 0xb3667a2eL, 0xc4614ab8L,
	0x5d681b02L, 0x2a6f2b94L, 0xb40bbe37L, 0xc30c8ea1L, 0x5a05df1bL,
	0x2d02ef8dL
};


class CRC32
{
public:
	// Returns a 32-bit CRC of the contents of the buffer.
	static unsigned long Make(const unsigned char *s)
	{
		unsigned int i;
		unsigned long crc32val;

		crc32val = 0;

		for (i = 0; s[i] != 0; i++)
		{
			crc32val = crc32_tab[(crc32val ^ s[i]) & 0xff] ^ (crc32val >> 8);
		}
		return crc32val;
	}

	static unsigned long Make(const unsigned char *s, unsigned int len)
	{
		unsigned int i;
		unsigned long crc32val;

		crc32val = 0;
		for (i = 0;  i < len;  i++)
		{
			crc32val =
				crc32_tab[(crc32val ^ s[i]) & 0xff] ^
				(crc32val >> 8);
		}
		return crc32val;
	}

};



#endif