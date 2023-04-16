#include "include/hash_table.hpp"
#include "include/string_utils.h"
#include <chrono>

int main()
{
    const int is_dbg = 1;

    Text* InputStruct = (Text*) calloc (1, sizeof(Text));
    FILE* input_file = get_file (input_filename, "rb");
    GetLines (InputStruct, input_file);
    close_file (input_file, input_filename);

    HashTable Table = {};

    
    HashTableCtor (&Table, TABLE_SIZE, MURASM_HASH);
    
    // printf ("Hash:    %lu\n", Table.hash_func("abcd", 4));
    // Table.hash_func = MurMurMurHash;
    // printf ("AsmHash: %lu\n", Table.hash_func("abcd", 4));

    // LoadData (InputStruct, &Table);

    // LOG ("Size: %d\n", InputStruct->obj_amount);
    
    // std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();

    // for (int useless_iter = 0; useless_iter < 100; useless_iter++)
    // {
    //     for (int i = 1 ; i < InputStruct->obj_amount; i++)
    //     {

    //         // uint32_t key = Table.hash_func (InputStruct->objects[i].begin, strlen (InputStruct->objects[i].begin));
    //         // printf ("Key: %u, String: %s\n", key, InputStruct->objects[i].begin);
    //         // AddMember (&Table, InputStruct->objects[i].begin);


    //         SearchMemberAVX (&Table, InputStruct->objects[i].begin, InputStruct->objects[i].length);

    //         // LOG ("Search\n");

    //     }
    // }

    // std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();

    // printf ("Elapsed time(ms): %u\n", std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count());
    // // DumpTable (&Table, 100);
    HashTableDtor (&Table);
}