#include "SMMIterator.h"
#include "SortedMultiMap.h"
#include <iostream>
#include <vector>
#include <exception>
#include <cmath>
using namespace std;

SortedMultiMap::SortedMultiMap(Relation r) {
	this->hashTable.m = 10;
	this->hashTable.relation = r;
	this->hashTable.table = new Node*[this->hashTable.m];
	for(int i = 0; i < this->hashTable.m; i++)
	    this->hashTable.table[i] = nullptr;
	this->sizeOfMap = 0;
}

void SortedMultiMap::add(TKey c, TValue v) {
    // resize
    if ((float)this->size() / (float)this->hashTable.m >= 0.75) {
        resize(2 * this->hashTable.m);
    }

    int newIndex = this->hashTable.hashFunction(c);
    Node* newNode = new Node;
    newNode->element.first = c;
    newNode->element.second = v;

    // CASE1: if list is empty just add it
    if (this->hashTable.table[newIndex] == nullptr) {
        newNode->next = nullptr;
        this->hashTable.table[newIndex] = newNode;
        this->sizeOfMap++;
        return;
    }

    // CASE2: we add at the beginning of the list
    if(!this->hashTable.relation(this->hashTable.table[newIndex]->element.first, newNode->element.first)){
        newNode->next = this->hashTable.table[newIndex];
        this->hashTable.table[newIndex] = newNode;
        this->sizeOfMap++;
        return;
    }

    auto currentNode = this->hashTable.table[newIndex]; // first node from new table
    Node* prevNode = currentNode;  // save the previous node from while

    // while the node from table is not null and the relation between this node and the new node is respected
    while(currentNode != nullptr && this->hashTable.relation(currentNode->element.first, newNode->element.first)){
        prevNode = currentNode;
        currentNode = currentNode->next;
    }

    // CASE3: add at the end of list
    if(currentNode == nullptr) {// the end of list
        newNode->next = nullptr;
        prevNode->next = newNode;  // new tail
        this->sizeOfMap++;
    }
    // CASE4: add in the middle of list
    else{ // it's not at the end
        newNode->next = prevNode->next;
        prevNode->next = newNode;
        this->sizeOfMap++;
    }

} // O(size)

vector<TValue> SortedMultiMap::search(TKey c) const {
	int index = this->hashTable.hashFunction(c);
	std::vector<TValue> valuesOfKey;

	Node* currentNode = this->hashTable.table[index];

    // we stop either at the end or at the key
	while(currentNode!= nullptr && currentNode->element.first!=c){
	    currentNode = currentNode->next;
	}
	// if we didn't stop at the end, parse the linked list until the key changes
	while(currentNode!= nullptr && currentNode->element.first==c){
	    valuesOfKey.push_back(currentNode->element.second);
	    currentNode = currentNode->next;
	}
	return valuesOfKey;
} //O(size of linked list) < O(n)

bool SortedMultiMap::remove(TKey c, TValue v) {
    int newIndex = this->hashTable.hashFunction(c);

    // case 1: list is empty
    if(hashTable.table[newIndex] == nullptr)
        return false;

    // case 2: remove from beginning
    auto headOfList = hashTable.table[newIndex];
    if(headOfList->element.first == c and headOfList->element.second == v){
        hashTable.table[newIndex] = headOfList->next;
        this->sizeOfMap--;
        return true;
    }

    auto currentNode = headOfList->next;
    auto prevNode = headOfList;
    if (currentNode != nullptr){
        while(currentNode->next!= nullptr){
            if(currentNode->element.first == c and currentNode->element.second == v){ // case 3: remove from middle
                prevNode->next = currentNode->next;
                this->sizeOfMap--;
                return true;
            }
            currentNode = currentNode->next;
        }
        // case 4: remove from end
        if(currentNode->element.first == c and currentNode->element.second == v){
            prevNode->next = nullptr;
            this->sizeOfMap--;
            return true;
        }
    }
    else{ // special case for second element as the last element
        if(prevNode->element.first == c and prevNode->element.second == v){
            prevNode->next = nullptr;
            this->sizeOfMap--;
            return true;
        }
    }

    return false;
} //O(size of linked list) < O(n)


int SortedMultiMap::size() const {
	return this->sizeOfMap;
} //Theta(1)

bool SortedMultiMap::isEmpty() const {
	if (this->sizeOfMap!=0)
	    return false;
    return true;
} //Theta(1)

SMMIterator SortedMultiMap::iterator() {
	return SMMIterator(*this);
}

SortedMultiMap::~SortedMultiMap() {
    for (int i = 0; i < this->hashTable.m; ++i) {
        auto currentLinkedList = this->hashTable.table[i];
        while (currentLinkedList != nullptr) {
            auto prev = currentLinkedList;
            currentLinkedList = currentLinkedList->next;
            delete prev;
        }
    }
    delete []this->hashTable.table;
} // O(m+n)

int SortedMultiMap::HashTableSortedMap::hashFunction(TKey c) const{
    return abs(c%(this->m));
} // Theta(1)

void SortedMultiMap::resize(int newCapacity) {
    Node** newTable = new Node*[newCapacity];
    for(int i = 0; i < newCapacity; i++)
        newTable[i] = nullptr;

    // modify m
    int oldM = this->hashTable.m;
    this->hashTable.m = newCapacity;

    // copy the old table into the new table
    for(int i = 0; i < oldM; i++){
        auto currentLinkedList = this->hashTable.table[i];
        while(currentLinkedList!= nullptr){
            auto currentNode = currentLinkedList;

            // find index of newNode
            int newIndex = this->hashTable.hashFunction(currentNode->element.first);
            Node* newNode = new Node;
            newNode->element = currentNode->element;

            // CASE1: if list is empty just add it
            if (newTable[newIndex] == nullptr) {
                newNode->next = nullptr;
                newTable[newIndex] = newNode;
                currentLinkedList = currentLinkedList->next;
                delete currentNode;
                continue;
            }


            // CASE2: we add at the beginning of the list
            if(!this->hashTable.relation(newTable[newIndex]->element.first, newNode->element.first)){
                newNode->next = newTable[newIndex];
                newTable[newIndex] = newNode;
                currentLinkedList = currentLinkedList->next;
                delete currentNode;
                continue;
            }


            auto currentTableNode = newTable[newIndex]; // first node from new table
            Node* prevNode = currentTableNode;  // save the previous node from while

            // while the node from table is not null and the relation between this node and the new node is respected
            while(currentTableNode != nullptr && this->hashTable.relation(currentNode->element.first, newNode->element.first)){
                prevNode = currentTableNode;
                currentTableNode = currentTableNode->next;
            }

            // CASE3: add at the end of list
            if(currentTableNode == nullptr) {// the end of list
                newNode->next = nullptr;
                prevNode->next = newNode;  // new tail
            }
            // CASE4: add in the middle of list
            else{ // it's not at the end
                    newNode->next = prevNode->next;
                    prevNode->next = newNode;
            }


            currentLinkedList = currentLinkedList->next;
            delete currentNode;
        }
    }
    delete []this->hashTable.table;
    this->hashTable.table = newTable;
} // O(m+alpha*n)

void SortedMultiMap::printMap() const {
    // for debugging purposes
    std::cout << "Size = " << this->size() << "\n";
    for(int i=0; i<this->hashTable.m; i++){
        auto currentList = this->hashTable.table[i];
        if(currentList!= nullptr)
            std::cout << "i= " << i << ":\n";
        else
            continue;
        while (currentList!= nullptr){
            std::cout << "(" << currentList->element.first << ", " << currentList->element.second << ")  ";
            currentList = currentList->next;
        }
        std::cout << "\n";
    }
}
