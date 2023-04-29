#include "include/hash_table.hpp"
#include "include/string_utils.h"

int main()
{
    Text* InputStruct = (Text*) calloc (1, sizeof(Text));
    FILE* input_file = get_file (input_filename, "rb");
    GetLines (InputStruct, input_file);
    close_file (input_file, input_filename);

    HashTable Table = {};
    FILE* csv_file = get_file (csv_filename, "w+");

    uint32_t (*FuncArray[])(const char*, int) =  
    {
        OneHash,
        FirstLetterHash,    
        LengthHash,
        SumHash,
        RorHash,
        RolHash,
        MurMurMurHash
    };

    for (int i = 0; i < HASHF_AMOUNT; i++)
    {
        HashTableCtor (&Table, TABLE_SIZE, FuncArray[i]);
        
        LoadData (InputStruct, &Table);
        
        DumpTableInCsv (&Table, csv_file);

        HashTableDtor (&Table);
    }

        // system ("python3 data/graphics_draw/draw_graph.py");

    close_file (csv_file, csv_filename);
}