
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#define MAX_NAME_LENGTH 32
#define HASHTABLE_SIZE 10001

typedef struct Person
{
    char *name;
    int age;
} Person;

typedef struct HashNode 
{
    Person *p;
    int status;
} HashNode;

Person *newPerson(const char *name, int age)
{
    int len = strlen(name);
    Person *p = (Person *)malloc(sizeof(Person) + len + 1);
    p->age = age;
    p->name = (char*)p + sizeof(Person);
    strcpy(p->name, name);
    return p;
}

void freePerson(Person *p)
{
    free(p);
}

unsigned int calcHashValue(const char *key) 
{
    unsigned int hashValue = 0;
    int n = strlen(key);
    for (int i = 0; i < n; i++) 
    {
        hashValue = (hashValue * 31 + key[i]);
    }
    return hashValue % HASHTABLE_SIZE;
}

void initHashTable(HashNode *buckets)
{
    buckets = (HashNode *)malloc(sizeof(HashNode) * HASHTABLE_SIZE);
    memset(buckets, 0, sizeof(HashNode) * HASHTABLE_SIZE);
}

int findHashNode(HashNode *buckets, const char *key, int *addIndex, int *hashValue)
{
    int i;
    int hV = calcHashValue(key);
    if (hashValue != NULL)
        *hashValue = hV;
    if (addIndex != NULL)
        *addIndex = -1;
    HashNode *hashNode;
    for (i = 0; i < HASHTABLE_SIZE; i++)
    {
        int index = hV + i;
        hashNode = &buckets[index];
        if (hashNode->status == -1)
        {
            if (addIndex != NULL && *addIndex == -1)
                *addIndex = index;
            continue;
        }
        else if (hashNode->status  == 0)
        {
            if (addIndex != NULL && *addIndex == -1)
                *addIndex = index;
            break;
        }
        else if (hashNode->status  == 1 && strcmp(hashNode->p->name, key) == 0)
            return index;
    }
    return -1;
}

int addHashNode(HashNode *buckets, Person *p)
{ 
    int addIndex;
    int hashValue;
    int foundIndex = findHashNode(buckets, p->name, &addIndex, &hashValue);
    if (foundIndex == -1)
    {
        if(addIndex != -1)
        {
            buckets[addIndex].p = p;
            buckets[addIndex].status = 1;
            return 1;
        }
    }
    return 0;
}

Person *deleteHashNode(HashNode *buckets, const char *key)
{
    int index = findHashNode(buckets, key, NULL, NULL);
    Person *p;
    if (index != -1)
    {
        p = buckets[index].p;
        buckets[index].p = NULL;
        return p;
    }
    return NULL;
}



int main(int argc, char *arrgv[])
{
    HashNode buckets[HASHTABLE_SIZE];
    initHashTable(buckets);

    Person *p1 = newPerson("E", 18);
    Person *p2 = newPerson("Ed", 128);
    Person *p3 = newPerson("Eth", 1238);

    addHashNode(buckets, p1);
    addHashNode(buckets, p2);
    addHashNode(buckets, p3);

    Person *result = (Person *)deleteHashNode(buckets, p1->name);
    if (result != NULL)
    {
        printf("Deleted: %s, Age: %d\n", result->name, result->age);
    }
    else
    {
        printf("Key not found.\n");
    }

    // Free the persons
    freePerson(p1);
    freePerson(p2);
    freePerson(p3);

    // Free the hash table
    //free(ht.buckets);

    return 0;
}