#ifndef __DLLIST_H
#define __DLLIST_H
#include "synch.h"
#define INS 0
#define DEL 1
#define PRI 2

class DLLElement {
    public:
        DLLElement(void *itemPtr, int sortKey); // initialize a list element
        DLLElement *next; // next element on list
        // NULL if this is the last
        DLLElement *prev; // previous element on list
        // NULL if this is the first
        int key; // priority, for a sorted list
        void *item; // pointer to item on the list
};

class DLList { 
    public: 
        DLList(); // initialize the list
        ~DLList(); // de-allocate the list
        void Prepend(void *item); // add to head of list (set key = min_key-1)
        void Append(void *item); // add to tail of list (set key = max_key+1)
        void *Remove(int *keyPtr); // remove from head of list
        // set *keyPtr to key of the removed item
        // return item (or NULL if list is empty)
        bool IsEmpty(); // return true if list has no elements
        // routines to put/get items on/off list in order (sorted by key)
        void SortedInsert(void *item, int sortKey); 
        void SortedInsert2(void *item, int sortKey); 
        void *SortedRemove(int sortKey); // remove first item with key==sortKey
        // return NULL if no such item exists
        void ShowList(int type);
        DLLElement * getFirst() { return first; }
        void setFirst(DLLElement *p) { first = p; }
    private: 
        DLLElement *first; // head of the list, NULL if empty
        DLLElement *last; // last element of the list, NULL if empty
        Lock *lock;
        Condition *dllistEmpty;
}; 

void InsertItem(int which, DLList *dllist, int key);
void RemoveItem(int which, DLList *dllist, int key);
void genItem2List(int n, DLList *dllist);
void delItemFromList(int n, DLList *dllist);
void PrintList(int which, DLList *dllist);
#endif
