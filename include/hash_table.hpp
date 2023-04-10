#ifndef HASH_T
#define HASH_T

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <assert.h>

void HashMain();

struct HashTableNode
{
    char* content;
    size_t length;
    HashTableNode* next;
};

struct HashTable
{
    HashTableNode** array;
    size_t size;
    uint32_t (*hash_func) (const char*);
};


const char input_filename[] = "data/input.txt";

//----------------------------------------------------

uint32_t LengthHash (char* string);

void HashTableCtor (HashTable* HashT, size_t size, void (* hashfunc)(char*));


#endif