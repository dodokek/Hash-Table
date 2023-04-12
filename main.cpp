#include "include/hash_table.hpp"
#include "include/stringUtils.h"

int main()
{
    // Text InputStruct = {};
    // FILE* input_file = get_file (input_filename, "r");

    // GetLines (&InputStruct, input_file);

    // PrintLines (InputStruct.objects, InputStruct.lines_amount);
    printf ("Building Hash table");

    HashTable* Table = (HashTable*) calloc (1, sizeof (HashTable));
    HashTableCtor (Table, 1000, LENGTH_HASH);

    AddMember (Table, "Bebra");

    // close_file (input_file, input_filename);
}