
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#define MAX_NAME_LENGTH 32

typedef struct Person
{
    char name[MAX_NAME_LENGTH];
    int age;
} Person;

typedef struct DoubleLinkNode
{
    Person person;
    struct DoubleLinkNode *next;
    struct DoubleLinkNode *prev;
} DoubleLinkNode;

typedef struct DoubleLink
{
    struct DoubleLinkNode *head;
    struct DoubleLinkNode *tail;
} DoubleLink;

typedef struct HashTable
{
    DoubleLink *buckets;
    int size;
} HashTable;

void initDoubleLink(DoubleLink *list) 
{
    list->head = NULL;
    list->tail = NULL;
}

void initHashTable(HashTable *ht, int size)
{
    ht->buckets = (DoubleLink *)malloc(size * sizeof(DoubleLink));
    for (int i = 0; i < size; i++)
    {
        initDoubleLink(&ht->buckets[i]);
    }
    ht->size = size;
}

unsigned int calcHashValue(HashTable *ht, const char *c)
{
    unsigned int hashValue = 0;
    int n = strlen(c);
    for (int i = 0; i < n; i++)
    {
        hashValue = (hashValue * 31 + c[i]); 
    }
    return hashValue % ht->size;
}

void dlinkAdd(DoubleLink *dlink, DoubleLinkNode *nodeAfter, DoubleLinkNode *node)
{
    if (nodeAfter == NULL) 
    {
        node->next = dlink->head;
        node->prev = NULL;
        if (dlink->head != NULL) 
        {
            dlink->head->prev = node;
        }
        dlink->head = node;
        if (dlink->tail == NULL) 
        {
            dlink->tail = node;
        }
    } 
    else 
    {
        node->next = nodeAfter->next;
        node->prev = nodeAfter;
        if (nodeAfter->next != NULL) 
        {
            nodeAfter->next->prev = node;
        } 
        else 
        {
            dlink->tail = node;
        }
        nodeAfter->next = node;
    }
}

int dlinkDelete(DoubleLink *dlink, DoubleLinkNode *node)
{
    if (dlink->head == NULL)
    {
        return 0;
    }
    if (node->prev != NULL)
    {
        node->prev->next = node->next;
    }
    else 
    {
        dlink->head = node->next;
    }
    if (node->next != NULL)
    {
        node->next->prev = node->prev;
    }
    else 
    {
        dlink->tail = node->prev;
    }
    return 1;
}

DoubleLinkNode *dlink_find(DoubleLink *dlink, const char *name)
{
    DoubleLinkNode *dl = dlink->head;

    while (dl != NULL)
    {
        if (strcmp(dl->person.name, name) != 0)
        {
            dl = dl->next;
        } 
        else
        {
            return dl;
        }
    }

    return 0;
}


int dlink_delete(DoubleLink *dlink, const char *name)
{
    DoubleLinkNode *dl = dlink_find(dlink, name);
    if (dl != NULL)
    {
        dlinkDelete(dlink, dl);
        free(dl);
        return 1;
    }
    return 0;
}

int dlink_add(DoubleLink *dlink, const char *name, int age)
{

    DoubleLinkNode *dl = dlink_find(dlink, name);

    if (dl != NULL)
        return 0;

    dl = (DoubleLinkNode *)malloc(sizeof(DoubleLinkNode));
    strncpy(dl->person.name, name, sizeof(dl->person.name)); 
    dl->person.age = age;
    dlinkAdd(dlink, NULL, dl);
    return 1;
}

int hashInsert( HashTable *ht, const char* name, int age)
{
    int hashV = calcHashValue(ht, name);
    return dlink_add(&(ht->buckets[hashV]), name, age);
} 

int hashDelete( HashTable *ht, const char *name)
{
    int hashV = calcHashValue(ht, name);
    return dlink_delete(&(ht->buckets[hashV]), name);
}

Person *hashFind(HashTable *ht, const char *name)
{
    int hashV = calcHashValue(ht, name);
    DoubleLinkNode *dl = dlink_find(&(ht->buckets[hashV]), name);

    if (dl != NULL)
        return &(dl->person);

    return NULL;
}

int main(int argc, char *arrgv[])
{
    char s[10] = "123456789";
    char a[5] = "789";
    //printf("%d",hashSearch(s,a));
    HashTable ht;
    initHashTable(&ht, 100);
    printf("%d\r\n ",hashInsert(&ht, "Cecilia", 18));
    printf("%d\r\n ",hashInsert(&ht, "Cecilia", 18));
    printf("%s %d\r\n ",hashFind(&ht, "Cecilia")->name, hashFind(&ht, "Cecilia")->age);
    printf("%d\r\n ",hashDelete(&ht, "Cecilia"));
    printf("%d\r\n ",hashDelete(&ht, "Cecilia"));


    return 0;
}