#include "include/hash_table.hpp"
#include "include/string_utils.h"

int main()
{
    Text* InputStruct = (Text*) calloc (1, sizeof(Text));
    FILE* input_file = get_file (input_filename, "rb");
    GetLines (InputStruct, input_file);
    close_file (input_file, input_filename);

    HashTable Table = {};

    #ifdef MURASM
        HashTableCtor (&Table, TABLE_SIZE, CRC32);
    #else
        HashTableCtor (&Table, TABLE_SIZE, MURMUR_HASH);
    #endif
    
    LoadData (InputStruct, &Table);

    StressTest (InputStruct, &Table);
    
    HashTableDtor (&Table);
    InputDtor (InputStruct);
}