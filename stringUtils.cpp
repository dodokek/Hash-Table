
#include "include/stringUtils.h"


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

            MainText->objects[words_counter].begin = MainText->buffer + word_begin;
            MainText->buffer[i] = '\0';
            word_begin = i + 1;

            // printf ("Word: %s\n", MainText->objects[words_counter].begin);
            words_counter++; 
        }
    }

    MainText->obj_amount = words_counter;
}


void PrintLines (Line objects[], int obj_amount)
{
    assert (objects != NULL && obj_amount > 0);
    for (int i = 0; i < obj_amount; i++)
    {
        //printf("%.*s", objects[i].length, objects[i].begin);
        //printf ("Line length %d\n", objects[i].length);
        puts (objects[i].begin);

    }
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