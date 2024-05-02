#include "BoundedBuffer.h"
#include "utility.h"

BoundedBuffer::BoundedBuffer(int maxSize)
{
  this->maxSize = maxSize;
  lock = new Lock("Buffer lock");
  full = new Condition("Buffer not full condition");
  empty = new Condition("Buffer not empty condition");
  usedSize = 0;
  head = tail = 0;
  buffer = new uchar[maxSize];
}
BoundedBuffer::~BoundedBuffer()
{
  delete lock;
  delete empty;
  delete full;
  delete buffer;
}

void BoundedBuffer::Write(void *data, int size)
{
  ASSERT(size <= maxSize);
  lock->Acquire();
  while (maxSize - usedSize < size)
  {
    // 缓冲区不够写，条件变量等待
    printf("Remain size is not enough.\n");
    full->Wait(lock);
  }
  // 开写
  for (int i = 0; i < size; i++)
  {
    *((uchar *)buffer + (tail + i) % maxSize) = *((uchar *)data + i);
  }
  tail = (tail + size) % maxSize;
  usedSize += size;
  empty->Signal(lock); // 改变空的条件变量
  lock->Release();
}

void BoundedBuffer::Read(void *data, int size)
{
  lock->Acquire();
  while (usedSize - size < 0)
  {
    printf("Not enough to read\n");
    empty->Wait(lock);
  }
  for (int i = 0; i < size; i++)
  {
    *((uchar *)data + i) = *((uchar *)buffer + (head + i) % maxSize);
  }
  head = (head + size) % maxSize;
  usedSize -= size;
  printf("\tFinish Read buffer. Buffer size : %d\n", usedSize);
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

