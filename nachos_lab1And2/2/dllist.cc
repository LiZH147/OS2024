#include "dllist.h"
#include <stdio.h>
#include "system.h"

// extern bool yield_flag;
extern int testnum;

DLLElement::DLLElement(void *itemPtr, int sortKey) {
    prev = NULL;
    next = NULL;
    item = itemPtr;
    key = sortKey;
}

DLList::DLList() {
    first = NULL;
    last = NULL;
    lock = new Lock("dllist lock");
    dllistEmpty = new Condition("dllist empty condition");
}

DLList::~DLList() {
    while(first != NULL) {
        DLLElement *p = first;
        first = first->next;
        delete p;
    }
    delete lock;
    delete dllistEmpty;
}

void DLList::Prepend(void *item) {
    if(first == NULL) {
        DLLElement *newnode = new DLLElement(item, 1);
        first = newnode;
        last = newnode;
        return ;
    } else {
        DLLElement *newnode = new DLLElement(item, first->key - 1);
        first->prev = newnode;
        newnode->next = first;
        first = newnode;
        return ;
    }
}

void DLList::Append(void *item) {
    lock->Acquire();
    DLLElement *newnode = new DLLElement(item, 1);
    if(IsEmpty()) {
        first = newnode;
        last = newnode;
    } else {
        last->next = newnode;
        newnode->prev = last;
        last = newnode;
    }
    dllistEmpty->Signal(lock);
    lock->Release();
}

void *DLList::Remove(int *keyPtr) { // remove from head
    lock->Acquire();
    while(IsEmpty()) dllistEmpty->Wait(lock); // condition
    DLLElement *element = first;
    void *thing;
    thing = first->item;
    if(first == last && first->next == NULL) { // only one
        first = NULL;
        last = NULL;
    } else {
        first = element->next;
    }
    if(keyPtr != NULL) *keyPtr = element->key;
    delete element;
    lock->Release();
    return thing;
}

bool DLList::IsEmpty() {
    return(!first && !last) ? true : false;
}

void DLList::SortedInsert(void *item, int sortKey) {
    lock->Acquire();
    DLLElement *newnode = new DLLElement(item, sortKey);
    DLLElement *ptr;
    if(IsEmpty()) { // if is empty, newone is the only one
        first = newnode;
        last = newnode;
    } else if(sortKey < first->key) {
        newnode->next = first;
        // if(yield_flag && (testnum == 2 ||  testnum == 3)) currentThread->Yield();
        first->prev = newnode;
        first = newnode;
        return ;
    } else {
        for(ptr = first; ptr->next != NULL; ptr = ptr->next) {
            if(sortKey < ptr->next->key) {
                newnode->next = ptr->next;
                newnode->prev = ptr;
                // if(yield_flag && (testnum == 2 ||  testnum == 3)) currentThread->Yield();
                ptr->next->prev = newnode;
                ptr->next = newnode;
                return ;
            }
        }
        // insert to the tail
        newnode->prev = last;
        // if(yield_flag && (testnum == 2 ||  testnum == 3)) currentThread->Yield();
        last->next = newnode;
        last = newnode;
    }
    dllistEmpty->Signal(lock);
    lock->Release();
}

void *DLList::SortedRemove(int sortKey) { // find the first elem that the key is equal to sortKey and remove it
    lock->Acquire();
    while(IsEmpty()) dllistEmpty->Wait(lock);
    DLLElement *ptr;
    void *TB_return;
    if(first->key == sortKey) { // if the first is equal to the sortKey, then delete it
        first = NULL;
        last = NULL;
    } else {
        for(ptr = first->next; ptr->next != NULL; ptr = ptr->next) {
            TB_return = ptr->item;
            if(ptr->key == sortKey) {
                ptr->prev->next = ptr->next;
                ptr->next->prev = ptr->prev;
                delete ptr;
            }
        }
        if(ptr->key == sortKey) {
            ptr->prev->next = NULL;
            last = ptr->prev;
            TB_return = ptr->item;
            delete ptr;
        } else return NULL;
    }
    lock->Release();
    return TB_return;
}

void DLList::ShowList(int type) { // show when insert
    DLLElement *p = first;
    if(IsEmpty()) {
        fprintf(stdout, "NULL\n");
        return ;
    }
    if(first == last && first == p) { // only one item
        if(type == INS) fprintf(stdout, "* %d *\n", p->key);
        else if(type == DEL) fprintf(stdout, "@ %d @\n", p->key);
        else if(type == PRI) fprintf(stdout, "# %d #\n", p->key);
        return ;
    }
    if(type == INS) fprintf(stdout, "* %d ", p->key);
    else if(type == DEL) fprintf(stdout, "@ %d ", p->key);
    else if(type == PRI) fprintf(stdout, "# %d ", p->key);
    while(p->next != NULL) {
        p = p->next;
        fprintf(stdout, "%d ", p->key);
    }
    if(type == INS) printf("*\n");
    else if(type == DEL) printf("@\n");
    else if(type == PRI) printf("#\n");
}
