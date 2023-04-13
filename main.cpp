#include "include/hash_table.hpp"
#include "include/stringUtils.h"

int main()
{
    Text* InputStruct = (Text*) calloc (1, sizeof(Text));
    FILE* input_file = get_file (input_filename, "rb");

    GetLines (InputStruct, input_file);

    LOG ("Read file\n");
    // PrintLines (InputStruct->objects, InputStruct->obj_amount);
    
    HashTable Table = {};
    HashTableCtor (&Table, 200, FIRST_ASCII_HASH);
    for (int i = 0; i < InputStruct->obj_amount; i++)
    {
        LOG ("Adding member\n");
        AddMember (&Table, InputStruct->objects[i].begin);
    }


    DumpTable (&Table, 200);
    close_file (input_file, input_filename);
}