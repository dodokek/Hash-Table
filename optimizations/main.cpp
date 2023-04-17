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

    
    HashTableCtor (&Table, TABLE_SIZE, MURMUR_HASH);
    LoadData (InputStruct, &Table);

    StressTest (InputStruct, &Table);
    
   // DumpTable (&Table, 100);
    HashTableDtor (&Table);
}