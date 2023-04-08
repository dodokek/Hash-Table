#include "include/hash_table.hpp"

void HashTableCtor (HashTable* self, size_t size, uint32_t (* hash_func)(const char*))
{
    assert (size > 0);

    self->size = size;
    self->array = (HashTableNode*) calloc (size, sizeof (HashTable));
    self->hash_func = hash_func;
}


void HashTableDtor (HashTable* self)
{
    free(self->array);
    self->array     = nullptr;
    self->hash_func = nullptr;
}


void AddMember (HashTable* self, char* member)
{
    uint32_t index = self->hash_func(member);

    

}


uint32_t LengthHash (char* string)
{
    return strlen(string);
}