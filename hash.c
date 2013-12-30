#include "hash.h"
#include <stdlib.h>
#include <assert.h>

int SymTable_hash(const char *pcKey, int iBucketCount)
{
    enum { HASH_MULTIPLIER = 65599 };
    int i;
    unsigned int uiHash = 0U;
    assert(pcKey != NULL);
    for(i = 0; pcKey[i] != '\0'; i++)
        uiHash = uiHash * (unsigned int)HASH_MULTIPLIER + (unsigned int)pcKey[i];
    return (int)(uiHash%(unsigned int)iBucketCount);
}
