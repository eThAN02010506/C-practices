
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#define MAX_NAME_LENGTH 32

typedef struct Person
{
    char *name;
    int age;
} Person;

typedef struct HashNode 
{
    const char *key;
    unsigned int hashValue;
    void *data;
    int status;
} HashNode;

typedef struct HashTable 
{
    HashNode *buckets;
    int size;
} HashTable;

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

unsigned int calcHashValue(HashTable *ht, const char *key) 
{
    unsigned int hashValue = 0;
    int n = strlen(key);
    for (int i = 0; i < n; i++) 
    {
        hashValue = (hashValue * 31 + key[i]);
    }
    return hashValue % ht->size;
}

void initHashTable(HashTable *ht, int size)
{
    ht->size = size;
    ht->buckets = (HashNode *)malloc(sizeof(HashNode) * size);
    memset(ht->buckets, 0, sizeof(HashNode) * size);
}

int findHashNode(HashTable *ht, const char *key, int *addIndex, int *hashValue)
{
    int i;
    int hV = calcHashValue(ht, key);
    if (hashValue != NULL)
        *hashValue = hV;
    if (addIndex != NULL)
        *addIndex = -1;
    HashNode *hashNode;
    for (i = 0; i < ht->size; i++)
    {
        int index = hV + i;
        hashNode = &ht->buckets[index];
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
        else if (hashNode->status  == 1 && strcmp(hashNode->key, key) == 0)
            return index;
    }
    return -1;
}

int addHashNode(HashTable *ht, void *data, const char *key)
{ 
    int addIndex;
    int hashValue;
    int foundIndex = findHashNode(ht, key, &addIndex, &hashValue);
    if (foundIndex == -1)
    {
        if(addIndex != -1)
        {
            ht->buckets[addIndex].data = data;
            ht->buckets[addIndex].key = key;
            ht->buckets[addIndex].hashValue = hashValue;
            ht->buckets[addIndex].status = 1;
            return 1;
        }
    }
    return 0;
}

void *deleteHashNode(HashTable *ht, const char *key)
{
    int index = findHashNode(ht, key, NULL, NULL);
    void *data;
    if (index != -1)
    {
        data = ht->buckets[index].data;
        ht->buckets[index].data = NULL;
        ht->buckets[index].key = NULL;
        ht->buckets[index].status = -1;
        return data;
    }
    return NULL;
}

void expandHashTable(HashTable *ht, int targetSize)
{
    HashTable hashTable;
    hashTable.size = targetSize;
    hashTable.buckets = (HashNode *)malloc(sizeof(HashNode) * targetSize);
    memset(hashTable.buckets, 0, sizeof(HashNode) * targetSize);
    for (int i = 0; i < ht->size; i++)
        addHashNode(&hashTable, ht->buckets[i].data, ht->buckets[i].key);
    free(ht->buckets);
    ht->buckets = hashTable.buckets;
    ht->size = hashTable.size;
}


struct SearchTreeNode
{
    struct SearchTreeNode *left;
    struct SearchTreeNode *right;
    int value;
};

typedef struct SearchTreeNode *TreeTop;
typedef struct SearchTreeNode SearchTreeNode;

void initTop(TreeTop *treeTop, SearchTreeNode *node)
{
    *treeTop = node; 
}

int treeSearch(TreeTop *treeTop, int value, SearchTreeNode **parentNode)
{
    if (*treeTop == NULL)
        return -1;
    *parentNode = NULL;
    SearchTreeNode *node = *treeTop;
    while (node != NULL && node->value != value)
    {
        *parentNode = node;
        if (value < node->value)
            node = node->left;
        else
            node = node->right;
    }
    if (node->value == value)
        return 1;
    return 0;
}

int treeAdd(TreeTop *treeTop, int value)
{
    SearchTreeNode *node;
    SearchTreeNode *treeNode = (SearchTreeNode*)malloc(sizeof(treeNode)+sizeof(int));
    treeNode->value = value;
    int i = treeSearch(treeTop, value, &node);
    if (i == 0)
    {
        if (value > node->value)
        {
            node->left = treeNode;
        }
        else if (value < node->value)
        {
            node->right = treeNode;
        }
        return 1;
    }
    else if (i == -1)
    {
        initTop(treeTop, treeNode);
        return 1;
    }
    return 0;
}   

int treeDelete(TreeTop *treeTop, int value)
{
    SearchTreeNode *node;
    int i = treeSearch(treeTop, value, &node);
    int leftOrRight = 0;
    if (i != 1)
    {
        return 0;
    }
    else
    {
        SearchTreeNode *temp;
        SearchTreeNode *tempParentNode;
        if (node->left->value == value)
        {
            temp = node->left->left;
            while (temp->right != NULL)
            {
                tempParentNode = temp;
                temp = temp->right;
            }
            tempParentNode->right = temp->left;
        }
        else
        {
            temp = node->right->right;    
            while (temp->left != NULL)
            {
                tempParentNode = temp;
                temp = temp->left;
            }
            tempParentNode->left = temp->right;
        }
        node->value = temp->value;
        temp->value = NULL;
        //右节点的最左边或者左节点的最右边
    }
    return 1;
}

SearchTreeNode *rotateRight(SearchTreeNode *node)
{
    SearchTreeNode *newRoot = node->left;
    node->left = newRoot->right;
    newRoot->right = node;
    return newRoot;
}

SearchTreeNode *rotateLeft(SearchTreeNode *node)
{
    SearchTreeNode *newRoot = node->right;
    node->right = newRoot->left;
    newRoot->left = node;
    return newRoot;
}

int treeSort(SearchTreeNode *node, int leftCount, int rightCount)
{
    if (leftCount < 0 && rightCount < 0)
        leftCount = 0, rightCount = 0;

    if (node == NULL)
        return 0;

    if (node->left != NULL)
    {
        treeSort(node->left, leftCount++, rightCount);
    }

    if (node->right != NULL)
    {
        treeSort(node->right, leftCount++, rightCount);
    }

    if (leftCount > rightCount + 1)
    {
        node = rotateRight(node);
    }
    else if (rightCount > leftCount + 1)
    {
        node = rotateLeft(node);
    }

    return 1; 
}
/**
int DinaryToBinary(int n)
{
    int temp[10];
    int i = 0;
    while (n > 0)
    {
        temp[i] = n % 2;
        i++;
        n /= 2;
    }
    int j = 0;
    for (i = 0; i < 10; i++)
    {
        j+= temp[i] * 10 ^ i;
    }
    return j;
}

int BinaryToDinary(int n)
{
    int temp[10];
    int i = 0;
    int count = 0;
    while (n > 0)
    {
        n -= 10 ^ i;
        i++;
        count++;
    }
    for (; count > 0; count--)
    {
        n += 2 ^ count;
    }
    return n;
}

int left(int index) 
{
//十进制x2然后在转成二进制，计算具体位置
    return index * 2;
}

int right(int index) 
{
    return index * 2 + 1;
}

void SearchBinaryTree_listadd(int list[], int n)
{
    int i = 1;
    while(1)
    {
        i = DinaryToBinary(i);
        if (list[i] < n)
        {
            i = list[left(BinaryToDinary(i))];
        }
        if (list[i] > n)
        {
            i = list[right(BinaryToDinary(i))];
        }

    }
}
**/


int main(int argc, char *arrgv[])
{
    HashTable ht;
    initHashTable(&ht, 10);

    Person *p1 = newPerson("E", 18);
    Person *p2 = newPerson("Ed", 128);
    Person *p3 = newPerson("Eth", 1238);

    addHashNode(&ht, p1, p1->name);
    addHashNode(&ht, p2, p2->name);
    addHashNode(&ht, p3, p3->name);

    Person *result = (Person *)deleteHashNode(&ht, p1->name);
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
    free(ht.buckets);

    return 0;
}