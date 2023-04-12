#include "include/hash_table.hpp"
#include "include/stringUtils.h"

int main()
{
    // Text InputStruct = {};
    // FILE* input_file = get_file (input_filename, "r");

    // GetLines (&InputStruct, input_file);

    // PrintLines (InputStruct.objects, InputStruct.lines_amount);

    HashTable* Table = (HashTable*) calloc (1, sizeof (HashTable));
    HashTableCtor (Table, 1000, LENGTH_HASH);

    AddMember (Table, "Bebra");
    AddMember (Table, "Bebro");
    AddMember (Table, "Kek");
    AddMember (Table, "Lol");
    SearchMember (Table, 5, "Bebra");

    DumpTable (Table, 6);
    // close_file (input_file, input_filename);
}