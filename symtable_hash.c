#include "symtable.h"
#include "hash.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

/* The array of the bucket counts for test */
/* static int bucketCountsArr[] = { 2, 4, 5 }; */

/* The array of the bucket counts */
static int bucketCountsArr[] = { 509, 1021, 2039, 4093, 8191, 16381, 32749, 65521 };

/* Define link list */
struct Bind
{
    char *key;
    void *val;
    struct Bind *next;
};

/* Define symbol table */
struct SymTable
{
    struct Bind *bind;
    int numBinds;
    int *bucketCounts;
};

SymTable_t SymTable_new(void)
{
    int i;
    struct SymTable *symtable = (struct SymTable *)malloc(sizeof(struct SymTable));
    if(!symtable)
        return NULL;
    symtable->bucketCounts = bucketCountsArr;
    symtable->bind = (struct Bind *)malloc(symtable->bucketCounts[0]*sizeof(struct Bind));
    if(!(symtable->bind))
    {
        free(symtable);
        return NULL;
    }
    for(i = 0; i < symtable->bucketCounts[0]; ++i)
    {
        symtable->bind[i].next = NULL;
        symtable->bind[i].key = NULL;
    }
    symtable->numBinds = 0;
    return symtable;
}

void SymTable_free(SymTable_t oSymTable)
{
    struct Bind *bind, *tmpBind;
    int i;
    if(!oSymTable)
        return ;
    for(i = 0; i < oSymTable->bucketCounts[0]; ++i)
    {
        bind = oSymTable->bind[i].next;
        while(bind)
        {
            tmpBind = bind;
            bind = bind->next;
            if(tmpBind->key)
                free(tmpBind->key);
            free(tmpBind);
        }
    }
    free(oSymTable->bind);
    free(oSymTable);
}

int SymTable_getLength(SymTable_t oSymTable)
{
    if(!oSymTable)
        return -1;
    return oSymTable->numBinds;
}

static void SymTable_rehash(SymTable_t oSymTable)
{
    int i, hashCode;
    struct Bind *oldBind = oSymTable->bind;
    struct Bind *tmpBind, *newBind, *bind;
    int oldBucketCounts = oSymTable->bucketCounts[0];
    ++(oSymTable->bucketCounts);
    oSymTable->bind = (struct Bind *)malloc(oSymTable->bucketCounts[0]*sizeof(struct Bind));
    if(!(oSymTable->bind))
    {
        oSymTable->bind = oldBind;
        --(oSymTable->bucketCounts);
        return ;
    }
    for(i = 0; i < oSymTable->bucketCounts[0]; ++i)
    {
        oSymTable->bind[i].next = NULL;
        oSymTable->bind[i].key = NULL;
    }
    for(i = 0; i < oldBucketCounts; ++i)
    {
        bind = oldBind[i].next;
        while(bind)
        {
            tmpBind=bind->next;
            hashCode = SymTable_hash(bind->key, oSymTable->bucketCounts[0]);
            newBind = &(oSymTable->bind[hashCode]);
            while(newBind->next)
                newBind = newBind->next;
            newBind->next = bind;
            bind->next = NULL;
            bind = tmpBind;
        }
    }
    free(oldBind);
}

int SymTable_put(SymTable_t oSymTable, const char *pcKey, const void *pvValue)
{
    int hashCode;
    struct Bind *bind;
    if(!oSymTable)
        return 0;
    if(!pcKey)
        return 0;
    if(oSymTable->numBinds >= oSymTable->bucketCounts[0] && oSymTable->bucketCounts[0] < 65521)
        SymTable_rehash(oSymTable);
    hashCode = SymTable_hash(pcKey, oSymTable->bucketCounts[0]);
    bind = &(oSymTable->bind[hashCode]);
    while(bind->next)
    {
        bind = bind->next;
        if(!strcmp(bind->key, pcKey))
            return 0;
    }
    bind->next = (struct Bind *)malloc(sizeof(struct Bind));
    if(!(bind->next))
        return 0;
    bind->next->key = (char *)malloc(strlen(pcKey) + 1);
    if(!(bind->next->key))
    {
        free(bind->next);
        bind->next = NULL;
        return 0;
    }
    bind = bind->next;
    bind->key[strlen(pcKey)] = '\0';
    strcpy(bind->key, pcKey);
    bind->val = (void *)pvValue;
    bind->next = NULL;
    ++(oSymTable->numBinds);

    return 1;
}

void *SymTable_get(SymTable_t oSymTable, const char *pcKey)
{
    int hashCode;
    struct Bind *bind;
    if(!oSymTable)
        return NULL;
    if(!pcKey)
        return NULL;
    hashCode = SymTable_hash(pcKey, oSymTable->bucketCounts[0]);
    bind = &(oSymTable->bind[hashCode]);
    while(bind->next)
    {
        bind = bind->next;
        if(!strcmp(bind->key, pcKey))
            return bind->val;
    }
    return NULL;
}

int SymTable_contains(SymTable_t oSymTable, const char *pcKey)
{
    int hashCode;
    struct Bind *bind;
    if(!oSymTable)
        return 0;
    if(!pcKey)
        return 0;
    hashCode = SymTable_hash(pcKey, oSymTable->bucketCounts[0]);
    bind = &(oSymTable->bind[hashCode]);
    while(bind->next)
    {
        bind = bind->next;
        if(!strcmp(bind->key, pcKey))
            return 1;
    }
    return 0;
}

void SymTable_map(SymTable_t oSymTable, void (*pfApply)(const char *pcKey, const void *pvValue, void *pvExtra), const void *pvExtra)
{
    int i;
    struct Bind *bind;
    if(!oSymTable)
        return ;
    if(!pfApply)
        return ;
    for(i = 0; i < oSymTable->bucketCounts[0]; ++i)
    {
        bind = &(oSymTable->bind[i]);
        while(bind->next)
        {
            bind = bind->next;
            pfApply((const char *)bind->key, (const void *)bind->val, (void *)pvExtra);
        }
    }
}


void *SymTable_remove(SymTable_t oSymTable, const char *pcKey)
{
    struct Bind *bind;
    int hashCode;
    if(!oSymTable)
        return NULL;
    if(!pcKey)
        return NULL;
    hashCode = SymTable_hash(pcKey, oSymTable->bucketCounts[0]);
    bind = &(oSymTable->bind[hashCode]);
    while(bind->next)
    { 
        if(!strcmp(bind->next->key, pcKey))
        {
            struct Bind *tmpBind = bind->next;
            bind->next = tmpBind->next;
            void *val = tmpBind->val;
            free(tmpBind->key);
            free(tmpBind);
            --(oSymTable->numBinds);
            return val;
        }
        bind = bind->next;
    }
    return NULL;
}

void *SymTable_replace(SymTable_t oSymTable, const char *pcKey, const void *pvValue)
{
    struct Bind *bind;
    int hashCode;
    if(!oSymTable)
        return NULL;
    if(!pcKey)
        return NULL;
    hashCode = SymTable_hash(pcKey, oSymTable->bucketCounts[0]);
    bind = &(oSymTable->bind[hashCode]);
    while(bind->next)
    { 
        bind = bind->next;
        if(!strcmp(bind->key, pcKey))
        {
            void *tmpVal = bind->val;
            bind->val = (void *)pvValue;
            return tmpVal;
        }
        bind = bind->next;
    }
    return NULL;
}
