#include "BoundedBuffer.h"
#include "utility.h"
BoundedBuffer::BoundedBuffer(int maxSize) {
    this->maxSize = maxSize;
    lock = new Lock("Buffer lock");
    full = new Condition("Buffer not full cond");
    empty = new Condition("Buffer not empty cond");
    usedSize = 0;
    head = 0;
    tail = 0;
    buffer = new uchar[maxSize];
}

BoundedBuffer::~BoundedBuffer() {
    delete lock;
    delete full;
    delete empty;
    delete buffer;
}

void BoundedBuffer::Write(void* data, int size) { // write data to buffer and the data size is size
    ASSERT(size <= maxSize);
    lock->Acquire();
    while(maxSize - usedSize < size) { // remain size not enough
        DEBUG('t', "\tRemain size is not enough.\n");
        full->Wait(lock);
    }
    for(int i = 0; i < size; i++) {
        *((uchar*)buffer + (tail + i) % maxSize) = *((uchar*)data + i);
    }
    tail = (tail + size) % maxSize;
    usedSize += size;
    DEBUG('b', "\tFinish Write Buffer. Buffer size : %d\n", usedSize);
    empty->Signal(lock);
    lock->Release();
}

void BoundedBuffer::Read(void* data, int size) {
    lock->Acquire();
    while(usedSize - size < 0) {
        DEBUG('t', "\tNot enough to read\n");
        empty->Wait(lock);
    }
    for(int i = 0; i < size; i++) {
        *((uchar*)data + i) = *((uchar*)buffer + (head + i) % maxSize);
    }
    head = (head + size) % maxSize;
    usedSize -= size;
    DEBUG('b', "\tFinish Read buffer. Buffer size : %d\n", usedSize);
    full->Signal(lock);
    lock->Release();
}

void BoundedBuffer::Print() {
    printf("\t");
    for(int i = head; i != tail; i = (i + 1) % maxSize) {
        printf("%c", buffer[i]);
    }
    printf("\n");
}

int BoundedBuffer::getUsed() {
    return usedSize;
}
