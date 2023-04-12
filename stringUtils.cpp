
#include "include/stringUtils.h"


void GetLines (Text* MainText, FILE* input_file)
{
    read_file (input_file, MainText);

    SplitOnWords (MainText);
}


void SplitOnWords (Text* MainText)
{
    MainText->objects = (Line*) calloc (MainText->symbols_amount * 2, sizeof (char));

    int cur_len = 0;
    int words_counter = 0;
    
    for (int i = 0; i < MainText->symbols_amount; i++)
    {
        if (isalpha(MainText->buffer[i]))
        {
            cur_len++;
        }
        else
        {
            if (cur_len == 0) continue;

            MainText->objects[words_counter].begin = MainText->buffer + i - cur_len;
            MainText->buffer[i] = '\0';
            cur_len = 0;
            words_counter++; 

            // printf ("Word: %s\n", MainText->objects[words_counter].begin);
        }
    }

    MainText->obj_amount = words_counter - 1;
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