#ifndef LIST_H
#define LIST_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <malloc.h>
#include <assert.h>
#include <stdint.h>
#include <stdarg.h>

#include "file_utils.h"

//----------------------------------------------

#define FREE(x) free(x), x = nullptr

#define ListDump(X, reason, ...) _ListDump(X, __FILE__, __PRETTY_FUNCTION__, __LINE__ reason, __VA_ARGS__)

typedef int elem_t;

//----------------------------------------------

enum ListFlags
{
    NOT_STATED = -1,
};

enum Sizes
{
    LIST_INITIAL_CAPACITY = 10,
    LIST_INITIAL_SIZE = 1,
};

enum PushType
{
    TAIL,
    HEAD
};

enum VerificationMode
{
    LOUD,
    QUIET
};

//----------------------------------------------

const int MAX_IMG_SRC_LEN = 150;
const int POISON_NUM = 0xBEBDA;

//----------------------------------------------


struct node
{
    elem_t value;
    int prev;
    int next;
};


struct List
{
    node* data;
    int size;
    int capacity;

    int head;
    int tail;
    int free;
    
    bool linear;
};

int ListInsertTail (List* list, elem_t value);

int ListInsertHead (List* list, elem_t value);

node* InitNewElem (List* list, int* new_elem_id, elem_t value);

void ListVerificate (List* list, FILE* log_file);

int HandleZeroSize (List* list, int new_elem_id);

int ListInsertRight (List* list, elem_t value, int elem_id);

int ListInsertLeft (List* list, elem_t value, int elem_id);

void ListResize (List* list, int new_capacity);

void ListLinearize (List* list);

int GetRealPos (List* list, int id);

int ListDelete (List* list, int elem_id);

void ListCtor (List* list, int capacity);

void ListDtor (List* list);

void _ListDump (List* list, const char* /*filename[]*/, const char func_name[], const int line, char* reason, ...);

void DrawList (List* list, FILE* log_file);

#endif