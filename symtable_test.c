#include "symtable.h"
#include <stdio.h>
#include <time.h>

#define ASSERT(con, s) \
    if(!(con)) \
    { printf(s); return ; }

/* 
 * Test SymTable_new function
 * */
void test_SymTable_new(void)
{
    SymTable_t symtable = SymTable_new();
    ASSERT(SymTable_getLength(symtable) == 0, "Test SymTable_new Failed.\n");
    SymTable_free(symtable);
    printf("Test SymTable_new Successfully.\n");
}

/*
 * Test SymTable_free function
 * */
void test_SymTable_free(void)
{
    ASSERT(1, "Test SymTable_free Failed.\n");
    SymTable_t symtable = SymTable_new();
    SymTable_free(symtable);
    printf("Test SymTable_free Successfully.\n");
}

/*
 * Test SymTable_getLength function
 * */
void test_SymTable_getLength(void)
{
    SymTable_t symtable = SymTable_new();
    ASSERT(SymTable_getLength(symtable) == 0, "Test SymTable_getLength Failed.\n");
    ASSERT(SymTable_getLength(NULL) == -1, "Test SymTable_getLength Failed.\n");
    printf("Test SymTable_getLength Successfully.\n");
    SymTable_free(symtable);
}

/*
 * Test SymTable_put function
 * */
void test_SymTable_put(void)
{
    /* symtable == NULL */
    ASSERT(SymTable_put(NULL, "a", (const void *)0) == 0, "Test SymTable_put Failed.\n");
    SymTable_t symtable = SymTable_new();
    ASSERT(SymTable_put(symtable, NULL, (const void *)1) == 0, "Test SymTable_put Failed.\n");
    /* input a == b */ 
    ASSERT(SymTable_put(symtable, "a", (const void *)1) == 1, "Test SymTable_put Failed.\n");
    ASSERT(SymTable_put(symtable, "a", (const void *)2) == 0, "Test SymTable_put Failed.\n");
    /* input a != b */
    ASSERT(SymTable_put(symtable, "b", (const void *)3) == 1, "Test SymTable_put Failed.\n");
    /* input a != c, test rehash */
    ASSERT(SymTable_put(symtable, "c", (const void *)3) == 1, "Test SymTable_put Failed.\n");
    /* input array, test the value */
    printf("Test SymTable_put Successfully.\n");
    
    SymTable_free(symtable);
}

/* 
 * Test SymTable_get function
 * */
void test_SymTable_get(void)
{
    /* symtable == NULL */
    ASSERT(SymTable_get(NULL, "ss") == NULL, "Test SymTable_get Failed.\n");
    /* symtable != NULL */
    SymTable_t symtable = SymTable_new();
    ASSERT(SymTable_get(symtable, "aa") == NULL, "Test SymTable_get Failed.\n");
    SymTable_put(symtable, "a", (const void *)1);
    ASSERT((int)SymTable_get(symtable, "a") == 1, "Test SymTable_get Failed.\n");
    SymTable_put(symtable, "b", (const void *)7);
    ASSERT((int)SymTable_get(symtable, "b") == 7, "Test SymTable_get Failed.\n");
    ASSERT(SymTable_get(symtable, "c") == NULL, "Test SymTable_get Failed.\n");
    ASSERT(SymTable_get(symtable, NULL) == NULL, "Test SymTable_get Failed.\n");
    SymTable_free(symtable);
    printf("Test SymTable_get Successfully.\n");
}    

/* 
 * Test SymTable_contains function
 * */
void test_SymTable_contains(void)
{
    /* symtable == NULL */ 
    ASSERT(SymTable_contains(NULL, "ss") == 0, "Test SymTable_contains Failed.\n");
    /* symtable != NULL */
    SymTable_t symtable = SymTable_new();
    ASSERT(SymTable_contains(symtable, "aa") == 0, "Test SymTable_contains Failed.\n");
    SymTable_put(symtable, "a", (const void *)1);
    ASSERT(SymTable_contains(symtable, "a") == 1, "Test SymTable_contains Failed.\n");
    SymTable_put(symtable, "b", (const void *)7);
    ASSERT(SymTable_contains(symtable, "b") == 1, "Test SymTable_contains Failed.\n");
    ASSERT(SymTable_contains(symtable, "c") == 0, "Test SymTable_contains Failed.\n");
    ASSERT(SymTable_contains(symtable, NULL) == 0, "Test SymTable_contains Failed.\n");
    SymTable_free(symtable);
    printf("Test SymTable_contains Successfully.\n");
    
}

/*
 * Test SymTable_map function
 * */
static void printSymTable(const char *pcKey, const void *pvValue, void *pvExtra)
{
    if(pvExtra)
        printf("%s\t%d\n", pcKey, (int)pvValue);
}

void test_SymTable_map(void)
{
    //ASSERT(SymTable_map(NULL, printKey, NULL));

    SymTable_t symtable = SymTable_new();
    SymTable_put(symtable, "a", (const void *)0);
    SymTable_put(symtable, "b", (const void *)1);
    SymTable_put(symtable, "c", (const void *)2);
    SymTable_put(symtable, "d", (const void *)3);
    SymTable_put(symtable, "e", (const void *)4);
    SymTable_map(symtable, printSymTable, (const void *)1);
    SymTable_free(symtable);
    printf("Test SymTable_map Successfully.\n");
}

/*
 * Test SymTable_remove function
 * */
void test_SymTable_remove(void)
{
    /* symtable == NULL */
    ASSERT(SymTable_remove(NULL, "a") == NULL, "Test SymTable_remove Failed.\n");
    SymTable_t symtable = SymTable_new();
    ASSERT(SymTable_remove(symtable, "a") == NULL, "Test SymTable_remove Failed.\n");
    SymTable_put(symtable, "a", (const void *)0);
    ASSERT(SymTable_remove(symtable, "b") == NULL, "Test SymTable_remove Failed.\n");
    SymTable_put(symtable, "b", (const void *)1);
    ASSERT((int)SymTable_remove(symtable, "b") == 1, "Test SymTable_remove Failed.\n");
    ASSERT(SymTable_remove(symtable, "b") == NULL, "Test SymTable_remove Failed.\n");
    ASSERT(SymTable_remove(symtable, NULL) == NULL, "Test SymTable_remove Failed.\n");
    SymTable_free(symtable);
    printf("Test SymTable_remove Successfully.\n");
}

/*
 * Test SymTable_replace function
 * */
void test_SymTable_replace(void)
{
    ASSERT(SymTable_replace(NULL, "a", (const void *)0) == NULL, "Test SymTable_replace Failed.\n");
    SymTable_t symtable = SymTable_new();
    ASSERT(SymTable_replace(symtable, "a", (const void *)0) == NULL, "Test SymTable_replace Failed.\n");
    SymTable_put(symtable, "a", (const void *)1);
    SymTable_put(symtable, "b", (const void *)2);
    ASSERT((int)SymTable_replace(symtable, "b", (const void *)3) == 2, "Test SymTable_replace Failed.\n");
    ASSERT((int)SymTable_replace(symtable, "b", (const void *)4) == 3, "Test SymTable_replace Failed.\n");
    ASSERT(SymTable_replace(symtable, NULL, (const void *)4) == NULL, "Test SymTable_replace Failed.\n");
    SymTable_free(symtable);
    printf("Test SymTable_replace Successfully.\n");
}

/*
 * Test the performance 
 * */
void test_Time(void)
{
    int i;
    char s[20];
    clock_t tBegin, tEnd, timePut, timeGet;

    SymTable_t symtable = SymTable_new();
    tBegin = clock();
    for(i = 0; i < 65521*2; ++i)
    {
        sprintf(s, "%d", i);
        SymTable_put(symtable, (const char *)s, (const void *)i);
    }
    tEnd = clock();
    timePut = tEnd - tBegin;
    tBegin = clock();
    for(i = 0; i < 65521*2; ++i)
    {
        sprintf(s, "%d", i);
        SymTable_get(symtable, s);
    }
    tEnd = clock();
    timeGet = tEnd - tBegin;
    SymTable_free(symtable);

    printf("Data scale is 65521*2, time of putting is %fs\n", (double)timePut/CLK_TCK);
    printf("Data scale is 65521*2, time of getting is %fs\n", (double)timeGet/CLK_TCK);
}

int main()
{
    test_SymTable_new();
    test_SymTable_free();
    test_SymTable_getLength();
    test_SymTable_put();
    test_SymTable_get();
    test_SymTable_contains();
    test_SymTable_map();
    test_SymTable_remove();
    test_SymTable_replace();
    test_Time();
    return 0;
}
