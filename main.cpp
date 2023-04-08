#include "include/hash_table.hpp"
#include "include/stringUtils.h"

int main()
{
    Text InputStruct = {};
    FILE* input_file = get_file (input_filename, "r");

    GetLines (&InputStruct, input_file);

    PrintLines (InputStruct.objects, InputStruct.lines_amount);

}