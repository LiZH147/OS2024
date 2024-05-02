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
#include "synchlist.h"
#include "Table.h"
#include "BoundedBuffer.h"

extern void InsertList(DLList *list, int N);
extern void RemoveList(DLList *list, int N);
// testnum is set in main.cc
int testnum = 1;
int F = 1;
int threadNum = 2;
int opNum = 2;

// 初始化一个链表
DLList *testList = new DLList();
SynchList *slist = new SynchList();
Table *testTable = new Table(5);
// 测试empty情况
// BoundedBuffer *bb = new BoundedBuffer(10);
// 测试full情况
BoundedBuffer *bb = new BoundedBuffer(3);

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

char *getThreadName(int i)
{
    switch (i)
    {
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

void SynchTest(int which)
{
    if (which % 2 == 0)
    { // even first remove then add
        fprintf(stdout, "Thread %d before remove\n", which);
        int *item = (int *)slist->Remove();
        fprintf(stdout, "Thread %d after remove %d\n", which, *item);
        *item = which;
        fprintf(stdout, "Thread %d before append %d\n", which, *item);
        slist->Append((void *)item);
        fprintf(stdout, "Thread %d after append %d\n", which, *item);
        delete item;
    }
    else
    { // odd first add then remove
        int *item = new int();
        *item = which;
        /* Append */
        fprintf(stdout, "Thread %d before append %d\n", which, *item);
        slist->Append((void *)item);
        fprintf(stdout, "Thread %d after append %d\n", which, *item);
        currentThread->Yield();
        /* Remove */
        fprintf(stdout, "Thread %d before remove %d\n", which, *item);
        slist->Remove();
        fprintf(stdout, "Thread %d after remove %d\n", which, *item);
    }
}
// 测试双向链表安全性
void DLListTestLab2(int which)
{
    if (which % 2 == 0)
    {
        fprintf(stdout, "Thread %d before remove\n", which);
        testList->Remove(NULL);
        fprintf(stdout, "Thread %d after remove\n", which);
        fprintf(stdout, "Thread %d before append\n", which);
        testList->Append(NULL);
        fprintf(stdout, "Thread %d after append\n", which);
    }
    else
    {
        /* Append */
        fprintf(stdout, "Thread %d before append\n", which);
        testList->Append(NULL);
        fprintf(stdout, "Thread %d after append\n", which);
        currentThread->Yield();
        /* Remove */
        fprintf(stdout, "Thread %d before remove\n", which);
        testList->Remove(NULL);
        fprintf(stdout, "Thread %d after remove\n", which);
    }
}
// 测试Table安全性
void TestTable(int which)
{
    ASSERT(opNum <= testTable->getSize());
    int *object = new int();
    int index = 0;
    if (which % 2 == 1)
    {
        for (int i = 0; i < opNum; i++)
        {
            *object = which + i;
            index = testTable->Alloc((void *)object);
            printf("Thread %d : Added object to table[%d]\n", which, index);
        }
        testTable->Print();
        printf("Now the number of objects is %d\n", testTable->cnt);
    }
    else
    {
        for (int i = 0; i < opNum; i++)
        {
            printf("Thread %d ask for delete object\n", which);
            testTable->Release(index);
            printf("Thread %d : delete object from table[%d]\n", which, index);
            index += 1;
        }
        testTable->Print();
        printf("Now the number of objects is %d\n", testTable->cnt);
    }
}
// 测试受限的缓冲区
// 初始时是对empty的测试，先对空缓冲区读取
void TestBoundedBuffer(int which)
{
    if (which % 2)
    {
        char *str = "Test\n";
        printf("The length of str is %d\n", strlen(str));
        for (int i = 0; i < 100; i++)
        {
            char temp = str[i];
            fprintf(stdout, "Thread %d before Write\n", which);
            bb->Write(&temp, 1);
            fprintf(stdout, "Thread %d after Write\n", which);
            // fprintf(stdout, "Thread %d before Print\n", which);
            bb->Print();
            // fprintf(stdout, "Thread %d after Print\n", which);
            if (temp == '\n')
            {
                break;
            }
        }
    }
    else
    {
        for (int i = 0; i < 100; i++)
        {
            char *temp = new char[3];
            temp[2] = '\0';
            fprintf(stdout, "Thread %d before Read\n", which);
            bb->Read(temp, 2);
            fprintf(stdout, "Thread %d after Read\n", which);
            // fprintf(stdout, "Thread %d before Print\n", which);
            bb->Print();
            if(bb->getUsed() == 0) break;
            // fprintf(stdout, "Thread %d after Print\n", which);
            printf("temp : %s\n", temp);
        }
    }
}

//----------------------------------------------------------------------
// ThreadTest1
// 	Set up a ping-pong between two threads, by forking a thread
//	to call SimpleThread, and then calling SimpleThread ourselves.
//----------------------------------------------------------------------

void createThreads(VoidFunctionPtr func)
{
    DEBUG('t', "Entering toDLListTest\n");
    Thread *t;
    for (int i = 0; i < threadNum; i++)
    {
        t = new Thread(getThreadName(i));
        t->Fork(func, i);
    }
}

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

    // testList = new DLList(); // 初始化一个双向链表

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
    case 4:
        createThreads(SynchTest);
        break;
    case 3:
        printf("实验2\n");
        createThreads(DLListTestLab2);
        break;
    case 5:
        printf("Table\n");
        createThreads(TestTable);
        break;
    case 6:
        printf("BoundedBuffer\n");
        createThreads(TestBoundedBuffer);
        break;
    default:
        printf("No test specified.\n");
        break;
    }
}
