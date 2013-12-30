#ifndef _SYMTABLE_SYMTABLE_H_
#define _SYMTABLE_SYMTABLE_H_

typedef struct SymTable *SymTable_t;

/* 
 * Create a new SymTable structure.
 * If not enough memory is available, return NULL,
 * else return a pointer to the SymTable.
 * */
SymTable_t SymTable_new(void);

/*
 * Free all the memory occupied by the symbol table pointed to by oSymTable
 * */
void SymTable_free(SymTable_t oSymTable);

/*
 * Return the total number of bindings within oSymTable
 * */
int SymTable_getLength(SymTable_t oSymTable);

/*
 * Put a binding into the symbol table.
 * The binding is created with pcKey and pvValue.
 * If the pcKey is already in the symbol table or the memory is not available, the function
 * leave the table and return 0, else return 1
 * */
int SymTable_put(SymTable_t oSymTable, const char *pcKey, const void *pvValue);

/* 
 * Locate the binding with pcKey through oSymTable.
 * If successful, return the binding's value.
 * Else leaves return a NULL pointer.
 * */
void *SymTable_get(SymTable_t oSymTable, const char *pcKey);

/* 
 * Search through oSymTable to locate the binding with pcKey.
 * If successful, return 1
 * Else return 0
 * */
int SymTable_contains(SymTable_t oSymTable, const char *pcKey);

/*
 * Iterate through the entire SymTable and apply pfApply on each binding.
 * */
void SymTable_map(SymTable_t oSymTable, void (*pfApply)(const char *pcKey, const void *pvValue, void *pvExtra), const void *pvExtra);

/* 
 * Search through oSymTable to locate the binding with pcKey.
 * If successful, remove the binding and return an opaque pointer.
 * Else return a NULL pointer.
 * */
void *SymTable_remove(SymTable_t oSymTable, const char *pcKey);

/*
 * Search through oSymTable to locate the binding with pcKey.
 * If successful, replace the binding's old value with pcValue and return an opaque pointer.
 * Else return NULL pointer.
 * */
void *SymTable_replace(SymTable_t oSymTable, const char *pcKey, const void *pvValue);
#endif
