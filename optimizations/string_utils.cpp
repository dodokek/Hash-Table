
#include "include/string_utils.h"


void GetLines (Text* MainText, FILE* input_file)
{
    read_file (input_file, MainText);

    SplitOnWords (MainText);
}


void SplitOnWords (Text* MainText)
{
    MainText->objects = (Line*) calloc (MainText->symbols_amount * 4, sizeof (char));

    int cur_len = 0;
    int words_counter = 0;
    int word_begin = 0;
    
    for (int i = 0; i < MainText->symbols_amount; i++)
    {
        if (isalpha(MainText->buffer[i]))
        {
            //
        }
        else
        {
            if (word_begin == i)
            {
                word_begin = i + 1;
                continue;
            }

            alignas(32) char tmp_str[32] = {};
            
            MainText->buffer[i] = '\0';
            
            strcpy (tmp_str, MainText->buffer + word_begin);
            __m256i* tmp = (__m256i*) aligned_alloc (32, sizeof(__m256i));
            
            memset (tmp, 0, sizeof (__m256i));
            *tmp = _mm256_load_si256 ((__m256i*) tmp_str);
            // printf ("String: %s\n", tmp);
            

            MainText->objects[words_counter].string = tmp;
            MainText->objects[words_counter].length = strlen(MainText->buffer + word_begin);
            word_begin = i + 1;

            // printf ("Word: %s\n", MainText->objects[words_counter].begin);
            words_counter++; 
        }
    }

    MainText->obj_amount = words_counter;
}

char* GetTextBuffer (FILE* file)
{
    fseek(file, 0L, SEEK_END);
    int size = ftell(file);
    fseek(file, 0L, SEEK_SET);

    char* buffer = (char*) calloc (size, sizeof (char));
    
    fread (buffer, sizeof(char), size, file);

    return buffer;
}


void InputDtor (Text *self)
{
    free (self->buffer);
    free (self->objects);
}


void ClearBuffer (char stop_sym)
{
    while (getchar() != stop_sym) ;
}


int read_file (FILE* file, Text *MainText)
{
    assert (file != nullptr);

    fseek (file, 0L, SEEK_END);
    int file_len = ftell (file);
    fseek (file, 0L, SEEK_SET);

    MainText->buffer = (char*) calloc(sizeof(char), file_len);
    MainText->symbols_amount = fread (MainText->buffer, sizeof(char), file_len, file);

    return 1;
}


void TextDestr (Text *self)
{
    free (self->buffer);
    free (self->objects);
}