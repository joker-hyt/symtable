#include "symtable.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Define link list */
struct Bind
{
    char *key;
    void *val;
    struct Bind *next;
};

/* Define the linked symbol table struct */
struct SymTable
{
    struct Bind *bind;
    int numBinds;	
};

SymTable_t SymTable_new(void)
{
    struct SymTable *symtable = (struct SymTable *)malloc(sizeof(struct SymTable));
    if(!symtable)
        return NULL;
    symtable->bind = (struct Bind *)malloc(sizeof(struct Bind));
    if(!(symtable->bind))
    {
        free(symtable);
        return NULL;
    }
    symtable->bind->next = NULL;
    symtable->bind->key = NULL;
    symtable->numBinds = 0;
    return symtable;
}

void SymTable_free(SymTable_t oSymTable)
{
    struct Bind *bind, *tmpBind;
    if(!oSymTable)
        return ;
    bind = oSymTable->bind;
    while(bind)
    {
        tmpBind = bind;
        bind = bind->next;
        if(tmpBind->key)
            free(tmpBind->key);
        free(tmpBind);
    }
    free(oSymTable);
}

int SymTable_getLength(SymTable_t oSymTable)
{
    if(!oSymTable)
        return -1;
    return oSymTable->numBinds;
}

int SymTable_put(SymTable_t oSymTable, const char *pcKey, const void *pvValue)
{
    struct Bind *bind;
    if(!oSymTable)
        return 0;
    if(!pcKey)
        return 0;
    bind = oSymTable->bind;
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
    struct Bind *bind;
    if(!oSymTable)
        return NULL;
    if(!pcKey)
        return NULL;
    bind = oSymTable->bind;
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
    struct Bind *bind;
    if(!oSymTable)
        return 0;
    if(!pcKey)
        return 0;
    bind = oSymTable->bind;
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
    struct Bind *bind;
    if(!oSymTable)
        return ;
    if(!pfApply)
        return ;
    bind = oSymTable->bind;
    while(bind->next)
    {
        bind = bind->next;
        pfApply((const char *)bind->key, (const void *)bind->val, (void *)pvExtra);
    }
}

void *SymTable_remove(SymTable_t oSymTable, const char *pcKey)
{
    struct Bind *bind;
    if(!oSymTable)
        return NULL;
    if(!pcKey)
        return NULL;
    bind = oSymTable->bind;
    while(bind->next)
    { 
        if(!strcmp(bind->next->key, pcKey))
        {
            struct Bind *tmpBind = bind->next;
            void *val = tmpBind->val;
            bind->next = tmpBind->next;
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
    if(!oSymTable)
        return NULL;
    if(!pcKey)
        return NULL;
    bind = oSymTable->bind;
    while(bind->next)
    {
        bind = bind->next;
        if(!strcmp(bind->key, pcKey))
        {
            void *tmpVal = bind->val;
            bind->val = (void *)pvValue;
            return tmpVal;
        }
    }
    return NULL;
}
