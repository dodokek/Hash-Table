#ifndef HASH_T
#define HASH_T

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <assert.h>
#include <immintrin.h>
#include <chrono>


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
    MURASM_HASH,
    CRC32,
};

struct  HashTableNode
{
    __m256i* content;
    size_t length;
    uint32_t peers;
    HashTableNode* next;
};

struct HashTable
{
    HashTableNode* array;
    size_t size;
    uint32_t (*hash_func) (const void*, int);
    enum HASH_FUNC_CODES hash_code;
};


enum RETURN_CODES
{
    NOT_FOUND = false,
    SUCCESS_FOUND = true,
    SUCCESS = 2,
    ALREADY_EXISTS,

};

//========================================================

const int MAX_WORD_LEN = 32;
const int TABLE_SIZE = 15013;
const char input_filename[] = "data/input2.txt";


//========================================================

// #define AVX_SEARCH
#define MURASM
// #define ASM_STR


#ifndef NO_LOG
    #define LOG(...) fprintf(stderr, __VA_ARGS__)
#else
    #define LOG(...) ;
#endif


//----------------------------------------------------



void StressTest(Text* Input, HashTable* self);

inline int asm_strcmp (const char* dst, const char* src);

int LoadData (Text* DataStruct, HashTable* self);

void DumpTableInCsv (HashTable* self, FILE* csv_file);

void HashTableCtor (HashTable* self, size_t size, HASH_FUNC_CODES hash_code);

bool SearchMember (HashTable* self, char* content, size_t len);

bool SearchMemberAVX (HashTable* self, __m256i* content, size_t len);

void HashTableDtor (HashTable* self);

int FreeRecurs(HashTableNode* cur_node, bool begin);

HashTableNode* CreateNode (__m256i* content);

int AddMember (HashTable* self, __m256i* content, size_t len);

void DumpTable (HashTable* self, int dump_size);

uint32_t OneHash (const char* string);

uint32_t FirstLetterHash (const char* string);

uint32_t LengthHash (const char* string);

uint32_t SumHash (const char* string);

uint32_t RorFunc (int num, int shift);

uint32_t RolFunc (int num, int shift);

uint32_t RolHash (const char* str);

uint32_t RorHash (const char* str);

uint32_t MurMurMurHash (const void* data, int len);

#endif