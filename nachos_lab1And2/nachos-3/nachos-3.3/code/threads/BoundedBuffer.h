#ifndef __BOUNDEDBUFFER_H
#define __BOUNDEDBUFFER_H
#include "synch.h"
#define uchar unsigned char

class BoundedBuffer {
public:
    BoundedBuffer(int maxSize);
    ~BoundedBuffer();
    void Read(void *data, int size);
    void Write(void *data, int size);
    void Print();
    int getUsed();
private:
    int maxSize; // size of buffer
    int head; // pointer to the first place of buffer used
    int tail; // pointer to the last place of buffer used
    int usedSize;
    uchar *buffer;
private:
    Lock *lock;
    Condition *full; // ensure buffer not full
    Condition *empty; // ensure buffer not empty
};

#endif
