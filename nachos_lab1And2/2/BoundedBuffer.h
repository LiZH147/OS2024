#ifndef __BOUNDEDBUFFER_H
#define __BOUNDEDBUFFER_H
#include "synch.h"
#define uchar unsigned char

class BoundedBuffer
{
public:
  // create a bounded buffer with a limit of 'maxsize' bytes
  BoundedBuffer(int maxsize);
  ~BoundedBuffer();
  // read 'size' bytes from the bounded buffer, storing into 'data'.
  // ('size' may be greater than 'maxsize')
  void Read(void *data, int size);

  // write 'size' bytes from 'data' into the bounded buffer.
  // ('size' may be greater than 'maxsize')
  void Write(void *data, int size);
  void Print();
  int getUsed();

private:
  int maxSize;
  int head;
  int tail;
  int usedSize;
  uchar *buffer;

  Lock *lock;
  Condition *full;
  Condition *empty;
};

#endif //__BOUNDEDBUFFER_H
