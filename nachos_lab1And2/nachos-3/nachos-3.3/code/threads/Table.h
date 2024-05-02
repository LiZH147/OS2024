#ifndef __TABLE_H
#define __TABLE_H
#include "synch.h"
class Table {
public:
    Table(int size);
    ~Table();

    int Alloc(void *object);
    void Release(int index);
    void *Get(int index);
    void Print(); // print all item in tableBase
    int getSize(); // get the current table size
    int cnt;
private:
    int TableSize;
    void** tableBase;
    Lock* lock;
    Condition* tableEmpty;
};

#endif // __TABLE_H

