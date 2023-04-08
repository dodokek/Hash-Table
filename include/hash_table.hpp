#ifndef HASH_T
#define HASH_T

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void HashMain();

struct HashTable
{
    HashTableNode* array;
    size_t size;
    uint32_t (*hash_func) (const char*);
};

struct HashTableNode
{
    char* content;
    size_t length;
    HashTableNode* next;
};


#endif