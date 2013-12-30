#ifndef _SYMTABLE_HASH_H_
#define _SYMTABLE_HASH_H_

/*
 * Return a hash code for pcKey that is between 0 and iBucketCount-1, inclusive.
 * */
int SymTable_hash(const char *pcKey, int iBucketCount);
#endif
