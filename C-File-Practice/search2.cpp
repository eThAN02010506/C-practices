#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

typedef struct Person
{
    char *name;
    int age;
} Person;

typedef struct Book
{
    char *name;
    char *author;
} Book;

typedef struct DoubleLinkNode
{
    const void *data;
    struct DoubleLinkNode *next;
    struct DoubleLinkNode *prev;
} DoubleLinkNode;

typedef struct DoubleLink
{
    struct DoubleLinkNode *head;
    struct DoubleLinkNode *tail;
} DoubleLink;

typedef struct HashTable HashTable;
typedef int (*fn_compare)(const void *data, const void *data2);
typedef unsigned int (*fn_hashValue)(HashTable *ht, const void *data);

struct HashTable
{
    DoubleLink *buckets;
    fn_compare cmp;
    fn_hashValue hv;
    int size;
};

void initDoubleLink(DoubleLink *list) 
{
    list->head = NULL;
    list->tail = NULL;
}

void initHashTable(HashTable *ht, int size, fn_compare cmp, fn_hashValue hv)
{
    ht->buckets = (DoubleLink *)malloc(size * sizeof(DoubleLink));
    for (int i = 0; i < size; i++)
    {
        initDoubleLink(&ht->buckets[i]);
    }
    ht->size = size;
    ht->cmp = cmp;
    ht->hv = hv;
}

int person_cmp(const void *data, const void *data2)
{
    return strcmp(((Person*)data)->name,((Person*)data2)->name);
}

int book_cmp(const void *data, const void *data2)
{
    return strcmp(((Book*)data)->name, ((Book*)data2)->name);
}

unsigned int person_hashvalue(HashTable *ht, const void *data)
{
    unsigned int hashValue = 0;
    const char *c = ((Person*)data)->name;
    int n = strlen(c);
    for (int i = 0; i < n; i++)
    {
        hashValue = (hashValue * 31 + c[i]); 
    }
    return hashValue % ht->size;
}

unsigned int book_hashValue(HashTable *ht, const void *data)
{
    unsigned int hashValue = 0;
    const char *c = ((Book*)data)->name;
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

DoubleLinkNode *hashFind(HashTable *ht, const void *data2, DoubleLink **dlink)
{
    int hashV = ht->hv(ht, data2);

    DoubleLinkNode *dl = ht->buckets[hashV].head;
    if (dlink != NULL)
        *dlink = &(ht->buckets[hashV]);

    while (dl != NULL)
    {
        if (ht->cmp(dl->data, data2) != 0)
        {
            dl = dl->next;
        } 
        else
        {
            return dl;
        }
    }

    return NULL;
}

int hashInsert( HashTable *ht, const void *data)
{
    DoubleLink *dlink = NULL;
    DoubleLinkNode *dl = hashFind(ht, data, &dlink);

    if (dl != NULL)
        return 0;

    dl = (DoubleLinkNode *)malloc(sizeof(DoubleLinkNode));
    dl->data = data;
    dlinkAdd(dlink, NULL, dl);
    return 1;
} 

const void* hashDelete( HashTable *ht, const void *data)
{
    DoubleLink *dlink = NULL;
    DoubleLinkNode *dl = hashFind(ht, data, &dlink);
    const void *data2 = NULL;
    if (dl != NULL)
    {
        dlinkDelete(dlink, dl);
        data2 = dl->data;
        free(dl);
    }
    return data2;
}

Person *new_person(const char *name, int age)
{
    int length = strlen(name);
    Person *p = (Person*)malloc(sizeof(Person) + length + 1);
    p->name = (char*)p + sizeof(Person);
    //p->name = (char*)(p + 1);
    strcpy(p->name, name);
    p->age = age;
    return p;
}

void free_person(Person *person)
{
    free(person);
}

Book *new_book(const char *name, const char *author)
{
    int length = strlen(name);
    int length2 = strlen(author);

    Book *p = (Book*)malloc(sizeof(Book) + length + 1 + length2 + 1);
    p->name = (char*)p + sizeof(Book);
    //p->name = (char*)(p + 1);
    strcpy(p->name, name);
    p->author = (char*)p + sizeof(Book) + length + 1;
    strcpy(p->author, author);
    return p;
}

void free_book(Book *book)
{
    free(book);
}

int main(int argc, char *arrgv[])
{
    /**
    char s[10] = "123456789";
    char a[5] = "789";
    //printf("%d",hashSearch(s,a));
    HashTable ht;
    Person *p1 = new_person("C", 19);
    initHashTable(&ht, 100, person_cmp, person_hashvalue);
    printf("%d\r\n ",hashInsert(&ht, p1));
    Person *p2 = new_person("C", 19);
    if (hashInsert(&ht, p2) == 0)
    {
        printf("0\r\n");
        free_person(p2);
    }

    DoubleLinkNode *dln = hashFind(&ht, p1, NULL);
    Person *p3 = (Person*)dln->data;

    printf("%s %d\r\n ",p3->name, p3->age);

    p1 = (Person*)hashDelete(&ht, p1);
    if (p1 != NULL)
        printf("1\r\n");
    else
        printf("0\r\n");
    p1 = (Person*)hashDelete(&ht, p1);
    if (p1 != NULL)
        printf("1\r\n");
    else
        printf("0\r\n");

        //printf("%d",hashSearch(s,a));

    **/
    HashTable ht;
    Book *p1 = new_book("C", "Cc");
    initHashTable(&ht, 100, book_cmp, book_hashValue);
    printf("%d\r\n ",hashInsert(&ht, p1));
    Book *p2 = new_book("C", "Cc");
    if (hashInsert(&ht, p2) == 0)
    {
        printf("0\r\n");
        free_book(p2);
    }

    DoubleLinkNode *dln = hashFind(&ht, p1, NULL);
    Book *p3 = (Book*)dln->data;

    printf("%s %s\r\n ",p3->name, p3->author);

    p1 = (Book*)hashDelete(&ht, p1);
    if (p1 != NULL)
    {
        printf("1\r\n");
        free_book(p1);
    }
    else
        printf("0\r\n");
    p1 = (Book*)hashDelete(&ht, p1);
    if (p1 != NULL)
    {
        printf("1\r\n");
        free_book(p1);
    }
    else
        printf("0\r\n");

    return 0;
}