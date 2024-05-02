#include "Table.h"
#include "synch.h"

Table::Table(int size) {
    TableSize = size;
    cnt = 0; // initailize
    lock = new Lock("table lock");
    tableEmpty = new Condition("tableEmpty condition");
    tableBase = new void*[size];
    for(int i = 0; i < size; i++) {
        tableBase[i] = NULL;
    }
}

Table::~Table() {
    TableSize = 0;
    cnt = 0;
    delete tableBase;
    delete lock;
    delete tableEmpty;
}

int Table::Alloc(void *object) { // add object to tableBase
    lock->Acquire();
    int ret;
    int index = 0;
    while(tableBase[index] != NULL && index < TableSize) {
        index++;
    }
    if(index >= TableSize) {
        ret = -1;
    } else {
        tableBase[index] = object;
        ret = index;
    }
    ASSERT(ret != -1);
    cnt += 1;
    tableEmpty->Signal(lock);
    lock->Release();
    return ret;
}

void Table::Release(int index) { // release item from tableBase
    lock->Acquire();
    while(tableBase[index] == NULL) tableEmpty->Wait(lock);
    tableBase[index] = NULL;
    cnt -= 1;
    lock->Release();
}

void *Table::Get(int index) { // return item on tableBase[index]
    lock->Acquire();
    void *ret;
    if(index >= TableSize) {
        ret = NULL;
    } else {
        ret = tableBase[index];
    }
    lock->Release();
    return ret;
}

int Table::getSize() {
    return TableSize;
}

void Table::Print() {
    for(int i = 0; i < TableSize; i++) {
        if(tableBase[i] == NULL) printf("tableBase[%d] : 0\n", i);
        else printf("tableBase[%d] : %d\n", i, (int)tableBase[i]);
    }
}
