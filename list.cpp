
#include "include/list.hpp"

int ListInsertHead (List* list, elem_t value)
{
    assert (list);

    if (list->size == list->capacity)
    {
        printf ("\nList is full!\n\n");
        return 0;
    }

    // Finding free space, inserting new elem there.
    int new_elem_id = 0;
    node* new_elem = InitNewElem (list, &new_elem_id, value);

    // Size = 0 case
    if (HandleZeroSize (list, new_elem_id)) return new_elem_id;
    
    // Size != 0
    printf ("Inserting to head\n");
    new_elem->next = list->head;
    list->data[list->head].prev = new_elem_id;
    list->head = new_elem_id;

    list->size++;
    list->linear = false;

    return new_elem_id;
}


int ListInsertTail (List* list, elem_t value)
{
    assert (list);

    // Finding free space, inserting new elem there.
    int new_elem_id = 0;
    node* new_elem = InitNewElem (list, &new_elem_id, value);

    // Size = 0 case
    if (HandleZeroSize (list, new_elem_id)) return new_elem_id;
    
    // Size != 0
    // printf ("Inserting to tail\n");
    new_elem->prev = list->tail;
    list->data[list->tail].next = new_elem_id;
    list->tail = new_elem_id;

    list->size++;

    return new_elem_id;
}


int ListInsertRight (List* list, elem_t value, int real_pos)
{
    if (real_pos == list->tail) return ListInsertTail (list, value);
    
    // Finding free space
    int new_elem_id = list->free;
    list->free = list->data[list->free].next;

    // Changing connections of neighbours
    int right_neighbour_id = list->data[real_pos].next;
    list->data[right_neighbour_id].prev = new_elem_id;
    list->data[real_pos].next = new_elem_id;
    
    // Writing inserted elem data
    list->data[new_elem_id].value = value;
    list->data[new_elem_id].prev  = real_pos;
    list->data[new_elem_id].next  = right_neighbour_id;

    list->linear = false;
    list->size++;
    
    return new_elem_id;
}


int ListInsertLeft (List* list, elem_t value, int real_pos)
{
    if (real_pos == list->head) return ListInsertHead (list, value);

    int left_neighbour_pos = list->data[real_pos].prev;

    return ListInsertRight (list, value, left_neighbour_pos);
}


node* InitNewElem (List* list, int* new_elem_id, elem_t value)
{
    // Finding free space
    *new_elem_id = list->free;
    node* new_elem = list->data + *new_elem_id;
    list->free = list->data[list->free].next;

    // Giving default values to node
    new_elem->value = value;
    new_elem->next  = 0;
    new_elem->prev  = 0;  

    return new_elem;
}


int HandleZeroSize (List* list, int new_elem_id)
{
    if (list->size == LIST_INITIAL_SIZE)
    {
        list->head = new_elem_id;
        list->tail = new_elem_id;
        list->size++;

        return 1;
    }

    return 0;
}


int ListDelete (List* list, int elem_id)
{
    node* elem_to_del = list->data + elem_id;

    if (list->size == LIST_INITIAL_SIZE + 1) // One elem in List case
    {
        list->tail = 0;
        list->head = 0;
    }
    else if (elem_id == list->tail) //Deleting tail
    {
        list->tail = list->data[list->tail].prev;
        list->data[list->tail].next = 0;
    }
    else if (elem_id == list->head) //Deleting head
    {
        list->head = list->data[list->head].next;
        list->data[list->head].prev = 0;
        list->linear = false;
    }
    else
    {
        node* prev_elem = list->data + elem_to_del->prev;
        node* next_elem = list->data + elem_to_del->next;

        prev_elem->next = elem_to_del->next;
        next_elem->prev = elem_to_del->prev;
        list->linear = false;
    }

    // Pushing freed elem to the beginning of free-data-list
    list->data[list->free].prev = elem_id;

    elem_to_del->value = POISON_NUM;
    elem_to_del->next = list->free;
    elem_to_del->prev = 0;

    list->free = elem_id;
    list->size--;

    return elem_id;
}


void ListResize (List* list, int new_capacity)
{
    if (new_capacity <= list->capacity) printf ("Can't shrink the list, data may corrupt.\n");
    
    // Allocating space for the new list
    node* new_data = (node*) calloc (new_capacity, sizeof (node));
    assert (new_data != NULL);
    
    // Rewriting every element in linear order
    int cur_elem_id = list->head;

    for (int i = 1; i < list->size; i++)
    {
        new_data[i] = list->data[cur_elem_id];
        cur_elem_id = list->data[cur_elem_id].next;
    }

    for (int elem_id = list->size; elem_id < new_capacity; elem_id++)
    {
        new_data[elem_id].value = POISON_NUM;
    }

    for (int i = 1; i < new_capacity - 1; i++)
    {
        new_data[i].prev = i - 1;
        new_data[i].next = i + 1;
    }

    new_data[list->size - 1].next = 0;
    new_data[new_capacity - 1].next = new_capacity - 1;
    new_data[new_capacity - 1].prev = new_capacity - 2;
    
    // Rewriting list info
    list->linear = true;
    list->capacity = new_capacity;
    list->free = list->size;
    list->head = 1;
    list->tail = list->size - 1;

    free (list->data);
    list->data = new_data;

    return;
}


void ListLinearize (List* list)
{
    // Rewriting every element in linear order
    int cur_elem_id = list->head;

    for (int i = 1; i < list->size + 1; i++)
    {
        list->data[i] = list->data[cur_elem_id];
        cur_elem_id = list->data[cur_elem_id].next;
    }

    for (int elem_id = list->size + 1; elem_id < list->capacity; elem_id++)
    {
        list->data[elem_id].value = POISON_NUM;
    }

    for (int i = 1; i < list->capacity - 1; i++)
    {
        list->data[i].prev = i - 1;
        list->data[i].next = i + 1;
    }

    list->data[list->capacity - 1].next = list->capacity - 1;
    list->data[list->capacity - 1].prev = list->capacity - 2;

    // Rewriting list info
    list->linear = true;
    list->free = list->size + 1;
    list->head = 1;
    list->tail = list->size;
}


int GetRealPos (List* list, int id)
{
    if (id > list->size - 1) return -1;
    if (list->linear) return id;
    
    // printf ("Why don't you just save the address?\n");

    int cur_elem_id = list->head;

    for (int i = 0; i < id - 1; i++)
    {
        cur_elem_id = list->data[cur_elem_id].next;
    }

    return cur_elem_id;
}

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wformat-nonliteral"
void _ListDump (List* list, const char* /* filename[] */, const char func_name[],
                const int line, char* reason, ...)
{

    va_list args;
    va_start (args, reason);
    printf (reason, args);
    
    printf ("<pre>\n");
    printf ("\n--------------------------------------\n");
    printf ("Call of function ListDump at %s, line %d:\n", func_name, line);

    printf ("Head: %d, Tail: %d, Free: %d\n",
            list->head, list->tail, list->free);

    printf ("Size: %d\nCapacity: %d\n", list->size, list->capacity);

    printf ("\n--------------------------------------\n");

    va_end (args);
}
#pragma GCC diagnostic pop

void ListCtor (List* list, int capacity)
{
    // Initializing the array of elems
    list->data = (node*) calloc (capacity, sizeof(node));
    assert (list->data != NULL);

    list->size = LIST_INITIAL_SIZE; // 0 at the end of list
    list->capacity = capacity;

    // Handling the beginning and the end of list
    list->data[0].next = 0;
    list->data[0].prev = 0;

    list->data[capacity - 1].next  = capacity - 1;
    list->data[capacity - 1].value = POISON_NUM;
    list->data[capacity - 1].prev  = capacity - 2;

    // Making connections with free elems
    for (int elem_id = 1; elem_id < capacity - 1; elem_id++)
    {
        list->data[elem_id].value = POISON_NUM;
        list->data[elem_id].next = elem_id + 1;
        list->data[elem_id].prev = elem_id - 1;
    }
    
    // Initialising the rest of stuff
    list->head = 0;
    list->tail = 0;
    list->free = 1;

    list->linear = true;
}


void ListDtor (List* list)
{
    FREE(list->data);
    list->size = -1;

    list->head = 0;
    list->tail = 0;
    list->free = 0;
    list->linear = false;
}


