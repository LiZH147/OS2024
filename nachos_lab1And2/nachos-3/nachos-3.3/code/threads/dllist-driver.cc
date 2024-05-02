#include "dllist.h"
#include <cstdio>
#include <ctime>
#define NUM_RANGE 65535

using namespace std;
int seed;

void InsertItem(int which, DLList *dllist, int keyv) {
    fprintf(stdout, "Thread %d : Insert %d to the dllist\n", which, keyv);
    dllist->SortedInsert(NULL, keyv);
}

void RemoveItem(int which, DLList *dllist, int keyv)
{
    fprintf(stdout, "Thread %d : Remove %d from the dllist\n", which, keyv);
    dllist->SortedRemove(keyv);
}

void PrintList(int which, DLList* dllist) {
    fprintf(stdout, "Print dllist in thread %d\n", which);
    dllist->ShowList(PRI);
}

void genItem2List(int n, DLList *dllist) { // generate n random keys and the dllist points to the list
    /*int *item, key;
    if(!seed) {
        srand(unsigned(time(0)));
        seed = 1;
    }
    for(int i = 0; i < n; i++) {
        item = new int;
        *item = rand();
        key = rand() % NUM_RANGE;
        printf("============Inserting %5d into the list......============\n", key);
        dllist->SortedInsert((void *)item, key);
        dllist->ShowList(INS);
        printf("============Inserting %5d into the list complete=========\n\n", key);
    }*/
}

void delItemFromList(int n, DLList *dllist) { // removes N items starting from the head of the list
    /*for(int i = 0; i < n; i++) {
        if(!dllist->IsEmpty()) {
            printf("============Delete from the head......=====================\n");
            int keyval;
            dllist->Remove(&keyval);
            dllist->ShowList(DEL);
            printf("============Delete %5d from the list complete============\n\n", keyval);
        } else {
            printf("The list is empty\n");
            return ;
        }
    }*/
}
