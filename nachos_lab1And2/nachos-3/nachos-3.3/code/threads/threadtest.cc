// threadtest.cc 
//	Simple test case for the threads assignment.
//
//	Create two threads, and have them context switch
//	back and forth between themselves by calling Thread::Yield, 
//	to illustratethe inner workings of the thread system.
//
// Copyright (c) 1992-1993 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation 
// of liability and disclaimer of warranty provisions.

#include "copyright.h"
#include "system.h"
#include "dllist.h"
#include "synchlist.h"
#include "Table.h"
#include "BoundedBuffer.h"

// testnum is set in main.cc
// default
int testnum = 1;
int threadnum = 2;
int oprnum = 2;
bool yield_flag = false; // mark the thread can be yield or not

DLList *dllist = new DLList(); // the list that will be oprated in diffrent thread commonly
SynchList *slist = new SynchList();
Table *table = new Table(5);
BoundedBuffer *bb = new BoundedBuffer(10);
int testData[100];
//----------------------------------------------------------------------
// getThreadName
// get the thread name of a thread to create a new thread max idx is 9
// the range of idx is 0 ~ 9
//----------------------------------------------------------------------
char*
getThreadName(int i) {
    switch (i) {
        case 0:
            return "Fork Thread 0";
        case 1:
            return "Fork Thread 1";
        case 2:
            return "Fork Thread 2";
        case 3:
            return "Fork Thread 3";
        case 4:
            return "Fork Thread 4";
        case 5:
            return "Fork Thread 5";
        case 6:
            return "Fork Thread 6";
        case 7:
            return "Fork Thread 7";
        case 8:
            return "Fork Thread 8";
        case 9:
            return "Fork Thread 9";
        default:
            return "Fork Thread X";
    }
}

void DLListTest(int which) {
    if(which % 2 == 0) { // even first remove then add
        fprintf(stdout, "Thread %d before remove\n", which);
        dllist->Remove(NULL);
        fprintf(stdout, "Thread %d after remove\n", which);
        fprintf(stdout, "Thread %d before append\n", which);
        dllist->Append(NULL);
        fprintf(stdout, "Thread %d after append\n", which);
    } else { // odd first add then remove
        /* Append */
        fprintf(stdout, "Thread %d before append\n", which);
        dllist->Append(NULL);
        fprintf(stdout, "Thread %d after append\n", which);
        currentThread->Yield();
        /* Remove */
        fprintf(stdout, "Thread %d before remove\n", which);
        dllist->Remove(NULL);
        fprintf(stdout, "Thread %d after remove\n", which);
    }
}

void DLListTest1(int which) {
    fprintf(stdout, "Insert items in thread %d\n", which);
    genItem2List(oprnum, dllist);
    if(yield_flag == true) currentThread->Yield();
    fprintf(stdout, "Remove items in thread %d\n", which);
    delItemFromList(oprnum, dllist);
}

void DLListTest2(int which) {
    fprintf(stdout, "Insert items in thread %d\n", which);
    genItem2List(oprnum, dllist);
}

void DLListTest3(int which) { // out of order
    fprintf(stdout, "In thread %d\n", which);
    if(which == 0) {
        InsertItem(which, dllist, 1);
        PrintList(which, dllist);
        InsertItem(which, dllist, 10);
        PrintList(which, dllist);
        InsertItem(which, dllist, 0);
        PrintList(which, dllist);
        InsertItem(which, dllist, 5);
        PrintList(which, dllist);
        InsertItem(which, dllist, 3);
        PrintList(which, dllist);
        InsertItem(which, dllist, 7);
        PrintList(which, dllist);
    } else { // which == 1
        PrintList(which, dllist);
        InsertItem(which, dllist, 3);
        PrintList(which, dllist);
    }
}

void SynchTest(int which) {
    if(which % 2 == 0) { // even first remove then add
        fprintf(stdout, "Thread %d before remove\n", which);
        int* item = (int*)slist->Remove();
        fprintf(stdout, "Thread %d after remove %d\n", which, *item);
        *item = which;
        fprintf(stdout, "Thread %d before append %d\n", which, *item);
        slist->Append((void*)item);
        fprintf(stdout, "Thread %d after append %d\n", which, *item);
        delete item;
    } else { // odd first add then remove
        int* item = new int();
        *item = which;
        /* Append */
        fprintf(stdout, "Thread %d before append %d\n", which, *item);
        slist->Append((void*)item);
        fprintf(stdout, "Thread %d after append %d\n", which, *item);
        currentThread->Yield();
        /* Remove */
        fprintf(stdout, "Thread %d before remove %d\n", which, *item);
        slist->Remove();
        fprintf(stdout, "Thread %d after remove %d\n", which, *item);
        delete item;
    }
}

void TableTest(int which) {
    for(int i = 0; i < threadnum; i++) testData[i] = i;
    ASSERT(oprnum <= table->getSize());
    int *object = new int();
    int index = 0;
    if(which % 2) {
        for(int i = 0; i < oprnum; i++) {
            *object = testData[which + i];
            index = table->Alloc((void*)object);
            printf("Thread %d : Added object to table[%d]\n", which, index);
        }
        table->Print();
        printf("Now the number of objects is %d\n", table->cnt);
    } else {
        for(int i = 0; i < oprnum; i++) {
            printf("Thread %d ask for delete object\n", which);
            table->Release(index);
            printf("Thread %d : delete object from table[%d]\n", which, index);
            index += 1;
        }
        table->Print();
        printf("Now the number of objects is %d\n", table->cnt);
    }
}

void BoundedBufferTest(int which) {
    if(which % 2) {
        char *str = "This is a test\n";
        printf("The length of str is %d\n", strlen(str));
        for(int i = 0; i < 100; i++) {
            char temp = str[i];
            bb->Write(&temp, 1);
            bb->Print();
            if(temp == '\n') {
                break;
            }
        }
    } else {
        for(int i = 0; i < 100; i++) {
            char *temp = new char[3];
            temp[2] = '\0';
            bb->Read(temp, 2);
            bb->Print();
            printf("temp : %s\n", temp);
            if(bb->getUsed() == 0) break;
        }
    }
}
//----------------------------------------------------------------------
// toDLListTest
// Enter diffrent dllist test
//----------------------------------------------------------------------
    void
toDLListTest(VoidFunctionPtr func) // VoidFunctionPtr is defined in utility.h   typedef void (*VoidFunctionPtr)(int arg)
{
    DEBUG('t', "Entering toDLListTest\n");
    Thread *t;
    for(int i = 0; i < threadnum; i++) {
        t = new Thread(getThreadName(i));
        t->Fork(func, i);
    }
}

//----------------------------------------------------------------------
// ThreadTest
// Invoke a test routine.
//----------------------------------------------------------------------
    void
ThreadTest()
{
    fprintf(stdout, "Entering test %d\n", testnum);
    switch (testnum) {
        case 0:
            toDLListTest(DLListTest);
            break;
        case 1:
            toDLListTest(DLListTest1);
            break;
        case 2:
            toDLListTest(DLListTest2);
            break;
        case 3:
            toDLListTest(DLListTest3);
            break;
        case 4:
            toDLListTest(SynchTest);
            break;
        case 5:
            // toDLListTest(TableTest);
            break;
        case 6:
            toDLListTest(BoundedBufferTest);
            break;
        default:
            printf("No test specified.\n");
            break;
    }
}

