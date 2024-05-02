#include "system.h"

#define START_KEY 100

class DLLElement {
public:
    DLLElement( void *itemPtr, int sortKey );   // initialize a list element
    DLLElement *next;
    DLLElement *prev;
    int key;
    void *item;
};

class DLList {
public:
    DLList();
    ~DLList();

    DLLElement *getHead();
    void *getItem(int keyPtr);
    void Prepend(void *item);
    void Append(void *item);
    void *Remove(int *keyPtr);


    bool IsEmpty();

    void SortedInsert(void *item, int sortKey);
    void *SortedRemove(int sortKey);

private:
    DLLElement *first;
    DLLElement *last;
};

void InsertList(DLList *list, int N);
void RemoveList(DLList *list, int N);
