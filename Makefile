CC = gcc -Wall -std=c99 -pedantic -Wextra

LINKED_BIN = test_linked
HASH_BIN = test_hash
LINKED_OBJ = symtable_test.o symtable_link.o
HASH_OBJ = symtable_test.o symtable_hash.o hash.o

all : $(LINKED_BIN) $(HASH_BIN)

$(LINKED_BIN) : $(LINKED_OBJ)
	$(CC) $(LINKED_OBJ) -o $(LINKED_BIN) 

$(HASH_BIN) : $(HASH_OBJ)
	$(CC) $(HASH_OBJ) -o $(HASH_BIN)

symtable_link.o : symtable_link.c
	$(CC) -c symtable_link.c

symtable_hash.o : symtable_hash.c
	$(CC) -c symtable_hash.c

hash.o : hash.c
	$(CC) -c hash.c

symtable_test.o : symtable_test.c
	$(CC) -c symtable_test.c

test :
	./test_linked 
	./test_hash

clean :
	rm $(LINKED_BIN) $(LINKED_OBJ) $(HASH_BIN) $(HASH_OBJ)
