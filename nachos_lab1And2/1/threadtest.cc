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
#include "hello.h"
#include "dllist.h"

// testnum is set in main.cc
int testnum = 1;
int F = 1;

// 初始化一个链表
DLList *testList;
// testList.Append((void *)3);

// extern void genItem2List(DLList *list, int n), delItemFromList(DLList *list, int n);

//----------------------------------------------------------------------
// SimpleThread
// 	Loop 5 times, yielding the CPU to another ready thread
//	each iteration.
//
//	"which" is simply a number identifying the thread, for debugging
//	purposes.
//----------------------------------------------------------------------

void SimpleThread(int which)
{
    int num;

    for (num = 0; num < 3; num++)
    {
        printf("*** thread %d looped %d times\n", which, num);
        if (which == 1)
        {
            // testList.Append((void *)3);
            // genItem2List(&testList, 3);
            InsertList(testList, 3);
        }
        if (which == 0)
        {
            // testList.Append((void *)4);
            // genItem2List(&testList, 3);
            // delItemFromList(&testList, 4);
            // InsertList(testList, 5);
        }

        // printHello();
        currentThread->Yield();
    }
}

void MyTestDLList1(int which)
{
    // 共享内存，两线程交替操作即可，不会互相影响
    // printf("正常\n");
    for (int i = 0; i < 3; i++)
    {
        if (which == 0)
        {
            printf("*** thread %d insert items %d times\n", which, i);
            InsertList(testList, 1);
        }
        else
        {
            printf("*** thread %d delete items %d times\n", which, i);
            RemoveList(testList, 1);
        }
        currentThread->Yield();
    }
}

void MyTestDLList2(int which)
{
    // 共享内存，覆盖
    // for (int i = 0; i < 3; i++)
    // {
    //     if (which == 0)
    //     {
    //         if (i % 2 == 0)
    //         {
    //             printf("*** thread %d insert items %d times\n", which, i);
    //             InsertList(testList, 3);
    //         } else {
    //             printf("*** thread %d remove items %d times\n", which, i);
    //             RemoveList(testList, 3);
    //         }
    //     // }
    //     // else
    //     // {
    //     //     printf("*** thread %d remove items %d times\n", which, i);
    //     //     // InsertList(testList, 1);
    //     //     RemoveList(testList, 2);
    //     }
    //     currentThread->Yield();
    // }

    printf("*** thread %d insert items\n", which);
    InsertList(testList, 3);
    currentThread->Yield();
    printf("*** thread %d remove items\n", which);
    RemoveList(testList, 3);
}

void MyTestDLList3(int which)
{
    // 共享内存，非法删除
    if (which == 0)
    {
        printf("*** thread %d delete items\n", which);
        RemoveList(testList, 6);
    }
    else
    {
        printf("*** thread %d delete items\n", which);
        RemoveList(testList, 6);
    }
    // currentThread->Yield();
}
//----------------------------------------------------------------------
// ThreadTest1
// 	Set up a ping-pong between two threads, by forking a thread
//	to call SimpleThread, and then calling SimpleThread ourselves.
//----------------------------------------------------------------------

void ThreadTest1()
{
    DEBUG('t', "Entering ThreadTest1");

    Thread *t = new Thread("forked thread");

    printf("ThreadTest1\n");

    t->Fork(SimpleThread, 1);
    SimpleThread(0);
}
void ThreadTest2()
{
    DEBUG('t', "Entering ThreadTest2");

    testList = new DLList(); // 初始化一个双向链表

    Thread *t = new Thread("forked thread");

    printf("ThreadTest2\n");
    // 新建两个线程操作一个链表
    switch (F)
    {
    case 1:
        printf("正常\n");
        t->Fork(MyTestDLList1, 1);
        MyTestDLList1(0);
        break;
    case 2:
        printf("共享内存导致的操作干涉\n");
        t->Fork(MyTestDLList2, 1);
        MyTestDLList2(0);
        break;
    case 3:
        printf("非法删除\n");
        InsertList(testList, 6);
        t->Fork(MyTestDLList3, 1);
        MyTestDLList3(0);
        break;
    default:
        printf("No test specified.\n");
        break;
    }
    // DLLElement *cur = testList->getHead();
    // while (cur != NULL)
    // {
    //     printf("%d->%d\n", cur->key, *(int *)cur->item);
    //     cur = cur->next;
    // }
}

//----------------------------------------------------------------------
// ThreadTest
// 	Invoke a test routine.
//----------------------------------------------------------------------

void ThreadTest()
{
    switch (testnum)
    {
    case 1:
        ThreadTest1();
        break;
    case 2:
        ThreadTest2();
        break;
    default:
        printf("No test specified.\n");
        break;
    }
}
