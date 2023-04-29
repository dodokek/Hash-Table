#ifndef HASH_T
#define HASH_T

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <assert.h>

#include "string_utils.h"

enum HASH_FUNC_CODES
{
    CONST_HASH,
    LENGTH_HASH,
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
    uint32_t (*hash_func) (const char*, int len);
};


enum RETURN_CODES
{
    NOT_FOUND = false,
    SUCCESS_FOUND = true,
    SUCCESS = 2,
    ALREADY_EXISTS,

};

const int HASHF_AMOUNT = 7;

//========================================================

const int TABLE_SIZE = 15013;
const char input_filename[] = "data/input.txt";
const char csv_filename[]   = "data/csv_file.csv";

//========================================================

// #define NO_LOG

#ifndef NO_LOG
    #define LOG(...) fprintf(stderr, __VA_ARGS__)
#else
    #define LOG(...) ;
#endif


//----------------------------------------------------

void HashMain();

int LoadData (Text* DataStruct, HashTable* self);

void DumpTableInCsv (HashTable* self, FILE* csv_file);

void HashTableCtor (HashTable* self, size_t size, uint32_t (*hash_func) (const char* data, int len));

bool SearchMember (HashTable* self, uint32_t key, const char content[]);

void HashTableDtor (HashTable* self);

int FreeRecurs(HashTableNode* cur_node, bool begin);

HashTableNode* CreateNode (const char content[]);

int AddMember (HashTable* self, const char* content);

void DumpTable (HashTable* self, int dump_size);

uint32_t OneHash (const char* string, int len);

uint32_t FirstLetterHash (const char* string, int len);

uint32_t LengthHash (const char* string, int len);

uint32_t SumHash (const char* string, int len);

uint32_t RorFunc (int num, int shift);

uint32_t RolFunc (int num, int shift);

uint32_t RolHash (const char* str, int len);

uint32_t RorHash (const char* str, int len);

uint32_t MurMurMurHash (const char* str, int len);
#endif