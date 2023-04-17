#include "include/hash_table.hpp"
#include "include/string_utils.h"

int main()
{
    const int is_dbg = 1;

    Text* InputStruct = (Text*) calloc (1, sizeof(Text));
    FILE* input_file = get_file (input_filename, "rb");
    GetLines (InputStruct, input_file);
    close_file (input_file, input_filename);

    HashTable Table = {};

    #ifdef MURASM
        HashTableCtor (&Table, TABLE_SIZE, MURASM_HASH);
    #else
        HashTableCtor (&Table, TABLE_SIZE, MURMUR_HASH);
    #endif
    LoadData (InputStruct, &Table);

    // char bebra[32] = "abccd";
    // char bebro[32] = "abccd";
    // __m256i kek1 = _mm256_load_si256 ((__m256i*) bebra);
    // __m256i kek2 = _mm256_load_si256 ((__m256i*) bebro);

    // printf ("Hash1 : %u, Hash2: %u\n", 
    //         Table.hash_func(&kek1, 19),
    //         Table.hash_func(&kek2, 19));

    StressTest (InputStruct, &Table);
    
   // DumpTable (&Table, 100);
    HashTableDtor (&Table);
}