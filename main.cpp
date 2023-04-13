#include "include/hash_table.hpp"
#include "include/stringUtils.h"

int main()
{
    Text* InputStruct = (Text*) calloc (1, sizeof(Text));
    FILE* input_file = get_file (input_filename, "rb");
    GetLines (InputStruct, input_file);
    close_file (input_file, input_filename);

    HashTable Table = {};
    FILE* csv_file = get_file (csv_filename, "w+");

    for (HASH_FUNC_CODES code = LENGTH_HASH; code <= MURMUR_HASH; code = HASH_FUNC_CODES(code + 1))
    {
        HashTableCtor (&Table, TABLE_SIZE, code);
        
        LoadData (InputStruct, &Table);
        
        DumpTableInCsv (&Table, csv_file);

        HashTableDtor (&Table);
    }

        // system ("python3 data/graphics_draw/draw_graph.py");

    close_file (csv_file, csv_filename);
}