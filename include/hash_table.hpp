#ifndef HASH_T
#define HASH_T

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>

void HashMain();

struct HashTableNode
{
    char* content;
    size_t length;
    HashTableNode* next;
};

struct HashTable
{
    HashTableNode* array;
    size_t size;
    uint32_t (*hash_func) (const char*);
};


const char input_filename[] = "data/input.txt";



#endif