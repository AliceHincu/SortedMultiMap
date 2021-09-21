#pragma once

#include <iostream>
#include "SortedMultiMap.h"


class SMMIterator{
	friend class SortedMultiMap;
private:
	//DO NOT CHANGE THIS PART
	SortedMultiMap& map;
	Node* head;
	Node* currentNode;
	// prevNode;
	explicit SMMIterator(SortedMultiMap& map);

    Node *SortedMerge(Node *a, Node *b);

    Node *mergeKLists(Node **arr, int last);

    Node *deepCopyList(Node *head);

public:
	void first();
	void next();
	bool valid() const;
   	TElem getCurrent() const;

    //removes and returns the current pair from the iterator
    //after the operation the current pair from the Iterator is the next pair from the SortedMultiMap, or,
    // if the removed pair was the last one, the iterator is invalid
    //throws exception if the iterator is invalid
    TElem remove();
};
