#ifndef HASH_T
#define HASH_T

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <assert.h>


enum HASH_FUNC_CODES
{
    LENGTH_HASH,
    CONST_HASH,
    SUM_HASH,
    FIRST_ASCII_HASH,
    ROR_HASH,
    ROL_HASH,
    MURMUR_HASH,
};

struct HashTableNode
{
    const char* content;
    size_t length;
    uint32_t peers;
    HashTableNode* next;
};

struct HashTable
{
    HashTableNode* array;
    size_t size;
    uint32_t (*hash_func) (const char*);
    enum HASH_FUNC_CODES hash_code;
};


enum RETURN_CODES
{
    NOT_FOUND = false,
    SUCCESS_FOUND = true,
    SUCCESS = 1,
    ALREADY_EXISTS = 2,

};


#define LOG(...) fprintf(stderr, __VA_ARGS__)


const char input_filename[] = "data/input.txt";

//----------------------------------------------------

void HashMain();

uint32_t LengthHash (const char* string);

void HashTableCtor (HashTable* self, size_t size, HASH_FUNC_CODES hash_code);

bool SearchMember (HashTable* self, uint32_t key, const char content[]);

void HashTableDtor (HashTable* self);

HashTableNode* CreateNode (const char content[]);

int AddMember (HashTable* self, const char* content);

void DumpTable (HashTable* self, int dump_size);

uint32_t OneHash (const char* string);

uint32_t FirstLetterHash (const char* string);

uint32_t LengthHash (const char* string);

uint32_t SumHash (const char* string);

uint32_t RorFunc (int num, int shift);

uint32_t RolFunc (int num, int shift);

uint32_t RolHash (const char* str);

uint32_t RorHash (const char* str);

uint32_t MurMurMurHash (const char* str);
#endif