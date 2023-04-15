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
    FILE* csv_file = get_file (csv_filename, "w+");

    
    HashTableCtor (&Table, TABLE_SIZE, MURMUR_HASH);
    
    LoadData (InputStruct, &Table);

    LOG ("Size: %d\n", InputStruct->obj_amount);
    for (int i = 1 ; i < 3; i++)
    {
        LOG ("Search %d\n", i);

        uint32_t key = Table.hash_func (InputStruct->objects[i].begin);

        SearchMember (&Table, key, InputStruct->objects[i].begin);
        LOG ("Search\n");

    }

    HashTableDtor (&Table);
}