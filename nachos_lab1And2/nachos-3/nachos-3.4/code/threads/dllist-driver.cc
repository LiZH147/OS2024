
#include "dllist.h"

void InsertList(DLList *list, int N)
{
    for (int i = 0; i < N; i++)
    {
        void *item;
        int key = Random() % 100;
        // int key = 1;
        int *items = new int[1];
        items[0] = Random() % 100;
        item = items;
        // printf("%s Insert:key->%d value->%d\n",currentThread->getName(),key,*(int*)item);
        printf("Insert:key->%d value->%d\n", key, *(int *)item);

        list->SortedInsert(item, key);
        // void *item0 = list->getItem(key);
        // printf("Insert over key%d\n", *(int *)item0);
    }
}

void RemoveList(DLList *list, int N)
{
    for (int i = 0; i < N; i++)
    {
        int key;
        void *item;
        // printf("Remove:key->%d \n",key);
        item = list->Remove(&key);
        printf("Remove:key->%d value->%d\n", key, *(int *)item);

        // printf("value->%d\n",*(int*)item);
        // currentThread->Yield();
    }
}
