#include "include/hash_table.hpp"


void HashTableCtor (HashTable* self, size_t size, HASH_FUNC_CODES hash_code)
{
    printf ("Building Hash table");

    assert (size > 0 && self != nullptr);

    self->size = size;
    self->array = (HashTableNode**) calloc (size, sizeof (HashTable));
    self->hash_code = hash_code;

    for (int i = 0; i < self->size; i++)
    {
        self->array[i]->content = nullptr;
        self->array[i]->length  = 0;
        self->array[i]->next    = nullptr;
        self->array[i]->peers = 0;
    }

    switch (hash_code)
    {
    case LENGTH_HASH:
        self->hash_func = LengthHash;
        break;
    
    default:
        break;
    }

    printf ("End of build");

}


void HashTableDtor (HashTable* self)
{
    free(self->array);
    self->array     = nullptr;
    self->hash_func = nullptr;
}


// ==========================================================
// Control functions ----------------------------------------
// ==========================================================


int AddMember (HashTable* self, const char* content)
{
    printf ("Adding member %s", content);


    uint32_t key = self->hash_func(content) % self->size;

    if (SearchMember (self, key, content) == NOT_FOUND)
    {
        if (self->array[key]->peers == 0)
        {
            self->array[key] = CreateNode (content);
            return SUCCESS;
        }
        
        HashTableNode* cur_node = self->array[key];
    
        while (cur_node->next != nullptr)
        {
            cur_node = cur_node->next;
        }
        cur_node->next = CreateNode (content);
        self->array[key]->peers += 1;

        return SUCCESS;

    }

    return ALREADY_EXISTS;

}


HashTableNode* CreateNode (const char content[])
{
    HashTableNode* new_node = (HashTableNode*) calloc (1, sizeof(HashTableNode));

    new_node->content = content;
    new_node->length  = strlen (content);
    new_node->peers   = 1;                  // initially, node is the only peer in the cell
    new_node->next    = nullptr;

    return new_node;
}


bool SearchMember (HashTable* self, uint32_t key, const char content[])
{
    HashTableNode* cur_node = self->array[key];

    while (cur_node != nullptr)
    {
        if (strcmp(cur_node->content, content) == 0)
            return SUCCESS_FOUND;
        
        cur_node = cur_node->next;
    }

    return NOT_FOUND;
}




// ==========================================================
// All sort of Hashes ---------------------------------------
// ==========================================================


uint32_t LengthHash (const char* string)
{
    return strlen(string);
}