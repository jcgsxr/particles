#ifndef __LIST_H__
#define __LIST_H__


template <class T> class _iterator;

///////////////////////////////////////////////////////////
// Linked List Class
///////////////////////////////////////////////////////////
template<class T> class ListNode
{
public:
	T data;
	ListNode* next;
	
	ListNode() {}
	~ListNode() {}
};




template<class T> class _iterator
{
public:
	ListNode<T> *node;
	
	_iterator() 
	{
		
	}
	
	_iterator(ListNode<T> *theNode)
	{
		node = theNode;
	}
	
	_iterator(ListNode<T> theNode)
	{
		node = &theNode;
	}
	
	// Preincrement
	inline ListNode<T>& operator++ ()
	{
		//++(*(ListNode<T> *)this->node);
		node = node->next;
		return (*node);
	}
	
	// Postincrement
	inline ListNode<T> operator++ (int)
	{
		ListNode<T> temp = *node;
		++(*this);
		return (temp);
	}
	
	// Equality/inequality operators for the T value.
	inline bool operator== (T *val)
	{
		if (&node->data == val)
		{
			return TRUE;
		}
		return FALSE;
	}
	inline bool operator== (T &val)
	{
		if (node->data == val)
		{
			return TRUE;
		}
		return FALSE;
	}
	inline bool operator!= (T &val)
	{
		if (!operator==(val))
		{
			return FALSE;
		}
		return TRUE;
	}
	
	
	// Equality/inequality operators for the ListNode itself.
	inline bool operator== (ListNode<T> val)
	{
		if (&(node->data) == val.data)
		{
			return TRUE;
		}
		return FALSE;
	}
	inline bool operator!= (ListNode<T> val)
	{
		if (!operator==(val))
		{
			return TRUE;
		}
		return FALSE;
	}
	
	// Equality/inequality operators for the iterator itself.
	inline bool operator== (_iterator<T> val)
	{
		if (&(node->data) == &val.node->data)
		{
			return TRUE;
		}
		return FALSE;
	}
	inline bool operator!= (_iterator<T> val)
	{
		if (!operator==(val.node))
		{
			return TRUE;
		}
		return FALSE;
	}
	
	
	// Have this operator return a pointer to the data to make the syntax equivalent to std::list iterator.
	inline T& operator*()
	{
		return node->data;
	}
	
};




template<class T> class List
{
private:
	ListNode<T> *first, *last;
	int numNodes;
	
public:
	
	typedef _iterator<T> iterator;
	
	List()
	{
		first = NULL;
		last = NULL;
		numNodes = 0;
	}
	
	~List()
	{
		clear();
	}
	
	// Deletes all nodes in the list.
	void clear()
	{
		ListNode<T>* iter = first;
		ListNode<T>* next;
		
		while(iter != NULL)
		{
			next = iter->next;
			delete iter;
			iter = next;
		}
		first = NULL;
		last = NULL;
		numNodes = 0;
	}
	
	// Returns an iterator pointing to the first node. This is set up to be mostly equivalent to stl::list iterator.
	_iterator<T> begin()
	{
		_iterator<T> it;
		it.node = first;
		return it;
	}
	
	// Returns an iterator pointing to the last node. This is set up to be mostly equivalent to stl::list iterator.
	_iterator<T> end()
	{
		_iterator<T> it;
		
		// Check whether there exists a last node at all.
		if (last)
		{
			it.node = last->next;
		}
		else 
		{
			it.node = last;
		}

		return it;
	}
	
	// Inserts a node at the front of the list.
	void push_front(const T& val)
	{		
		ListNode<T>* newEntry = new ListNode<T>;
		
		newEntry->data = val;
		newEntry->next = first;
		
		if (first == NULL)
			last = newEntry;
		
		first = newEntry;
		
		numNodes++;
	}
	
	// Inserts a node at the end of the list.
	void push_back(const T& val)
	{
		ListNode<T>* newEntry = new ListNode<T>;
		newEntry->data = val;
		newEntry->next = NULL;
		
		// If there are no nodes in the list, we will have it point to the new one.
		if (first == NULL)
		{
			first = newEntry;
			last = newEntry;
		}
		else
		{
			last->next = newEntry;
			last = newEntry;
		}
		
		numNodes++;
	}
	
	// Deletes the first node of the list.
	void pop_front()
	{
		if (first != NULL)
		{
			ListNode<T>* front = first;
			
			first = first->next;
			
			delete front;
			
			numNodes--;
		}
	}
	
	// Returns a pointer to the first node's data.
	T* peek_front()
	{
		if (first)
			return &first->data;
		else
			return NULL;
	}
	
	// Returns a pointer to the last node's data.
	T* peek_back()
	{
		if(last)
			return &last->data;
		else
			return NULL;
	}
	
	// Inserts a node after the given node.
	void insert_after(ListNode<T>* previous, const T& val)
	{
		ListNode<T>* newEntry = new ListNode<T>;
		
		newEntry->data = val;
		newEntry->next = previous->next;
		previous->next = newEntry;
		
		numNodes++;
	}
	
	// Removes the node after the given node.
	void remove_after(ListNode<T>* previous)
	{
		ListNode<T>* removed = previous->next;
		previous->next = removed->next;
		delete removed;
		numNodes--;
	}
	
	// Checks whether the list contains the given data.
	bool contains(const T& val)
	{
		ListNode<T>* iter = first;
		
		while(iter != NULL)
		{
			if (iter->data == val)
				return true;
			else
				iter = iter->next;
		}
		return false;
	}
	
	// Inserts a node before a given node.
	void insert_before(ListNode<T>* node, const T& val)
	{
		ListNode<T> *temp = first;
		while (temp)
		{
			if (temp->next != NULL)
			{
				// If we see that the next node matches, we insert after the current node, which is before the next node.
				if (temp->next == node)
				{
					// Create new node.
					ListNode<T>* newEntry = new ListNode<T>;
					newEntry->data = val;
					// Point the new node's next to the current node's next.
					newEntry->next = temp->next;
					// Point the current node's next to the new node.
					temp->next = newEntry;
					numNodes++;
					
					// Done, break out.
					break;
				}
			}
			else
			{
				// No such node exists, just push the element onto the front of the list.
				push_front(val);
				break;
			}
			temp = temp->next;
		}
	}

	
	// Removes the node containing the given data.
	void remove(const T& val)
	{
		ListNode<T>* iter = first;
		ListNode<T>* prev = NULL;
		
		while(iter != NULL)
		{			
			// Check for whether there is matching data.
			if (iter->data == val)
			{
				// If we are on the first node, then point the first node to the next node, so we can delete it.
				if (prev == NULL)
				{
					first = iter->next;
				}
				else
				{
					// Otherwise, set the previous node's next to the current node's next, 
					// effectively skipping the node to be removed so that it can be deleted safely.
					prev->next = iter->next;
				}
				
				// If the current node is the last node, then point it back to the previous node
				// so that it can be deleted safely.
				if (iter == last)
					last = prev;
				
				// Delete the matching node.
				delete iter;
				
				// Update node count.
				numNodes--;
				
				return;
			}
			else
			{
				// Save off previous node location.
				prev = iter;
				// Move to the next node.
				iter = iter->next;
			}
		}
	}
	
	// Length and size are the same, use whichever one is more (syntactically) convenient.
	int length()
	{
		return numNodes;
	}
	
	int size()
	{
		return numNodes;
	}


	void reverse()
	{
		ListNode<T> *p_reverse = NULL;
		ListNode<T> *p = first;
		
		last = first;
		
		while (p != NULL)
		{
			// Save off the next node location, so that when we are done, we move to the next node in the list.
			ListNode<T> *tmp = p->next;
			
			// Now that the next node location is saved, we will point it to front of the reverse list.
			p->next = p_reverse;
			
			// Now that the next node is pointing to the new location, we will point the reverse list to that node.
			p_reverse = p;
			
			// Set the list node back to the temp location, which is the next node in the list.
			p = tmp;
		}
		
		first = p_reverse;
	}
	
	
	// Sorting for this list. The ListNode<T> must have the < operator defined in order for this to work.
	// This method uses insertion sort.
	void sort()
	{
		List<T> unsorted_list = *this;
		List<T> sorted_list;
		
		ListNode<T> *cur_unsorted_node = unsorted_list.first;
		
		// Place the first element into the sorted list so it's not empty.
		sorted_list.push_front(cur_unsorted_node->data);
		
		// Point the node to the first element of the list. We will use these to compare and insert elements.
		ListNode<T> *cur_sorted_node = sorted_list.first;
		
		// Move to the next unsorted node since we already added the first sorted one.
		cur_unsorted_node = cur_unsorted_node->next;
		
		while (cur_unsorted_node)
		{
			while (cur_sorted_node)
			{
				// Compare values to see whether we should push the value onto the stored list.
				if (cur_unsorted_node->data < cur_sorted_node->data)
				{
					//sorted_list.push_front(cur_unsorted_node->data);
					sorted_list.insert_before(cur_sorted_node, cur_unsorted_node->data);
					// Once we insert the element into the sorted list, we can break out and look at the next unsorted element.
					break;
				}
				else if (cur_sorted_node->next == NULL)
				{
					// When the next node is null, we know that the value is greater than any of the exisiting nodes in the sorted list so we append it to the end.
					sorted_list.push_back(cur_unsorted_node->data);
					// Once we insert the element into the sorted list, we can break out and look at the next unsorted element.
					break;
				}
				cur_sorted_node = cur_sorted_node->next;
			}
			// Make the sorted node point back to the beginning of the sorted list.
			cur_sorted_node = sorted_list.first;
			cur_unsorted_node = cur_unsorted_node->next;
		}
		
		// Delete existing list.
		clear();
		
		// Copy contents of sorted list to this list.
		ListNode<T> *temp_p = sorted_list.first;
		while (temp_p)
		{
			push_back(temp_p->data);
			temp_p = temp_p->next;
		}
	}
};

#endif