
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#define MAX_NAME_LENGTH 32

int kmpSearch(char *sequence, char *target) 
{
    int N = strlen(sequence);
    int M = strlen(target);
    int *next = (int *)malloc(M * sizeof(int));
    int i = 0; 
    int j = 0;
    // 先循环要查找的内容
    while (j < M) 
    {
        //检测字符匹配
        if (target[j] == target[i]) 
        {
            //前缀后缀长度++
            i++;
            //next的
            next[j] = i;
            //索引++
            j++;
        } 
        else 
        {
            //如果前缀后缀长度>0
            if (i != 0) 
            {
                //长度=长度-1位置的长度
                i = next[i - 1];
            } 
            //其他情况下
            else 
            {
                //
                next[j] = 0;
                j++;
            }
        }
    }
    
    i = 0; 
    j = 0; 
    while (i < N) 
    {
        if (target[j] == sequence[i]) 
        {
            i++;
            j++;
        }
        if (j == M) 
        {
            return i - j;
            j = next[j - 1];
        } 
        else if (i < N && target[j] != sequence[i]) 
        {
            if (j != 0) 
            {
                j = next[j - 1];
            } 
            else 
            {
                i++;
            }
        }
    }
    free(next);
    return 0;
}

int sundaySearch(char *sequence, char *target) 
{
    //target length
    int length_t = strlen(target);
    //sequence length
    int length_s = strlen(sequence);
    //偏移表
    int shiftTable[length_t];
    //初始化要移动几位（默认移动长度+1）
    for (int i = 0; i < 256; i++) 
    {
        shiftTable[i] = length_t + 1;
    }
    //根据target中字符出现的位置调整要移动多少
    for (int i = 0; i < length_t; i++) 
    {
        shiftTable[target[i]] = length_t - i;
    }

    int position = 0;
    while (position <= length_s - length_t) 
    {
        int i;
        for (i = 0; i < length_t; i++) 
        {
            if (sequence[position + i] != target[i]) 
            {
                break;
            }
        }
        if (i == length_t) 
        {
            return position;
        }
        if (position + length_t < length_s) 
        {
            position += shiftTable[sequence[position + length_t]];
        } 
        else 
        {
            break;
        }
    }
    return -1;
}

int bmSearch(char *sequence, char *target) 
{
    int length_t = strlen(target);
    int length_s = strlen(sequence);
    int badCharTable[256];

    for (int i = 0; i < 256; i++) 
    {
        badCharTable[i] = -1;
    }
    for (int i = 0; i < length_t; i++) 
    {
        badCharTable[target[i]] = i;
    }

    int position = 0;
    while (position <= length_s - length_t) 
    {
        int i = length_t - 1;
        while (i >= 0 && target[i] == sequence[position + i]) 
        {
            i--;
        }
        if (i < 0) 
        {
            return position;
        } 
        else 
        {
            int badCharShift = i - badCharTable[sequence[position + i]];
            if (badCharShift > 1) 
            {
                position += badCharShift;
            } 
            else 
            {
                position += 1;
            }
        }
    }
    return -1;
}

int hashSearch(char *sequence, char *target)
{
    int tLength = strlen(target);
    int sLength = strlen(sequence);
    int tHashValue = 0;
    int sHashValue = 0;
    int k = 0;
    int j = 0;
    int muiltiplier = 1;
    
    if (tLength > sLength)
        return -1;
    
    for (int i = 0; i < tLength; i++)
    {
        tHashValue = target[i] + tHashValue * tLength;
    }

    for (int i = 0; i < tLength; i++)
    {
        sHashValue = sequence[i] + sHashValue * tLength;
        muiltiplier *= tLength;
    }
 
    while (j < sLength)
    {
        if (sHashValue == tHashValue)
        {
            for (k = 0; k < tLength; k++)
            {
                if (sequence[j + k] != target[k])
                    break;
            }
            if (k == tLength)
                return j;
        }
        if (j + tLength > sLength)
            return -1;
        sHashValue = sHashValue * tLength - sequence[j] * muiltiplier + sequence[j + tLength];
        j++;        
    }
    return -1;
}



typedef struct Person
{
    char name[MAX_NAME_LENGTH];
    int age;
} Person;

typedef struct DoubleLinkNode
{
    Person *person;
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
        if (strcmp(dl->person->name, name) != 0)
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
        free(dl->person);
        free(dl);
        return 1;
    }
    return 0;
}

int dlink_add(DoubleLink *dlink, const char *name, int age)
{

    DoubleLinkNode *dl = dlink_find(dlink, name);
    DoubleLinkNode *dln;

    if (dl != NULL)
        return 0;

    dln = (DoubleLinkNode *)malloc(sizeof(DoubleLinkNode));
    Person *person = (Person*)malloc(sizeof(Person)); 
    strncpy(person->name, name, sizeof(person->name)); 
    person->age = age;
    dln->person = person;
    dlinkAdd(dlink, dl, dln);
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
        return dl->person;

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