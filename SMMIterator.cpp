#include "SMMIterator.h"

SMMIterator::SMMIterator(SortedMultiMap& d) : map(d){
    // create a new table and copy in it the nodes from the hashtable (just the first one)
    // and find the position of the minimum
    //  Divide and Conquer, O(n Log m). Copy: O(n+m)
    auto copyTable = new Node*[this->map.hashTable.m];
    for(int i = 0; i < this->map.hashTable.m; i++) {
        copyTable[i] = this->deepCopyList(this->map.hashTable.table[i]);
    }

    this->head = mergeKLists(copyTable, this->map.hashTable.m-1);
    this->currentNode = head;

} // O(n+m+n*Log m)

void SMMIterator::first(){
    this->currentNode = head;
} // Theta(1)

void SMMIterator::next(){
	// remove the minimum node (replace it with its next) and find the position of the next minimum
    if (!this->valid())
        throw std::exception();

    this->currentNode = this->currentNode->next; // go to next pair
} // Theta(1)

bool SMMIterator::valid() const{
	// next should set the position of the minimum to a special value when there are no more elements.
	// Valid should just check for this special value
	if (this->currentNode == nullptr)
	    return false;
	return true;
} // Theta(1)

TElem SMMIterator::getCurrent() const{
	// return the information from the node from the minimum position
	if (!this->valid())
        throw std::exception();
	return this->currentNode->element;
} // Theta(1)

Node* SMMIterator::SortedMerge(Node* a, Node* b)
{
    Node* result;

    /* Base cases */
    if (a == nullptr)
        return (b);
    else if (b == nullptr)
        return (a);

    /* Pick either a or b, and recur */
    if (this->map.hashTable.relation(a->element.first, b->element.first)) {
        result = a;
        result->next = SortedMerge(a->next, b);
    }
    else {
        result = b;
        result->next = SortedMerge(a, b->next);
    }

    return result;
} // Theta(1)

// The main function that takes an array of lists
// arr[0..last] and generates the sorted output
Node* SMMIterator::mergeKLists(Node** arr, int last)
{
    // repeat until only one list is left
    while (last != 0) {
        int i = 0, j = last;

        // (i, j) forms a pair
        while (i < j) {
            // merge List i with List j and store merged list in List i
            arr[i] = SortedMerge(arr[i], arr[j]);

            // consider next pair
            i++, j--;

            // If all pairs are merged, update last
            if (i >= j)
                last = j;
        }
    }

    return arr[0];
} // O(n Log m)

Node* SMMIterator::deepCopyList(Node* headOfList) {
    if (headOfList == nullptr) {
        return nullptr;
    }
    else {
        // Allocate the memory for new Node
        // in the heap and set its data
        auto newNode = new Node;

        newNode->element = headOfList->element;

        // Recursively set the next pointer of
        // the new Node by recurring for the
        // remaining nodes
        newNode->next = deepCopyList(headOfList->next);

        return newNode;
    }
}// O(m+n)

TElem SMMIterator::remove() {
    if (!valid())
        throw exception();

    Node* elemNode = this->currentNode;
    TElem element = elemNode->element;
    this->map.remove(element.first, element.second);
    this->currentNode = elemNode->next;

    return element;
} // //O(size of linked list) < O(n) because of remove function from smm