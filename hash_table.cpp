#include "include/hash_table.hpp"


void HashTableCtor (HashTable* self, size_t size, HASH_FUNC_CODES hash_code)
{
    LOG ("Building Hash table\n");

    assert (size > 0 && self != nullptr);

    self->size = size;
    self->array = (HashTableNode*) calloc (size, sizeof (HashTableNode));
    self->hash_code = hash_code;

    LOG ("Setting array\n");

    for (int i = 0; i < self->size; i++)
    {
        self->array[i].content = nullptr;
        self->array[i].length  = 0;
        self->array[i].next    = nullptr;
        self->array[i].peers = 0;
    }

    LOG ("Setting function ptr\n");


    switch (hash_code)
    {
    case LENGTH_HASH:
        self->hash_func = LengthHash;
        break;
    
    default:
        break;
    }

    LOG ("End of build\n");

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
    LOG ("Adding member %s\n", content);

    uint32_t key = self->hash_func(content) % self->size;

    if (SearchMember (self, key, content) == NOT_FOUND)
    {
        LOG ("New member, key %u\n", key);

        if (self->array[key].peers == 0)        // Corner case
        {
            self->array[key].content = content;
            self->array[key].peers   = 1;
            return SUCCESS;
        }
        
        HashTableNode* cur_node = &(self->array[key]);
    
        while (cur_node->next != nullptr)
            cur_node = cur_node->next;           // Skipping to end of chain

        cur_node->next = CreateNode (content);   // Hanging new nodes
        self->array[key].peers += 1;

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
    HashTableNode* cur_node = &(self->array[key]);

    LOG ("\tSearching member %s\n", content);

    while (cur_node != nullptr && cur_node->peers != 0)
    {
        if (strcmp(cur_node->content, content) == 0)
        {
            LOG ("=== Found %s, key: %u\n", content, key);
            return SUCCESS_FOUND;
        }
        cur_node = cur_node->next;
    }

    LOG ("xxx Not found %s, key: %u\n", content, key);

    return NOT_FOUND;
}


// ==========================================================
// Dump of table --------------------------------------------
// ==========================================================

void DumpTable (HashTable* self, int dump_size)
{
    printf ("\n====================================\n");
    printf ("Hash Table Dump\n");
    printf ("====================================\n");

    printf ("Hash function code: %d\n", self->hash_code);
    printf ("Size: %lu\n", self->size);
    printf (">>> Elements\n");

    for (int i = 0; i < dump_size; i++)
    {
        HashTableNode* cur_node = &(self->array[i]);
        if (cur_node->content == nullptr)
            continue;

        printf ("Key %d (%d):\n\t", i, self->array[i].peers);
        
        while (cur_node->next != nullptr)
        {
            printf ("%s->", cur_node->content);
            cur_node = cur_node->next;
        }

        printf ("%s\n", cur_node->content);
        
    }
    printf ("========== End of Dump =============\n");

}

// ==========================================================
// All sort of Hashes ---------------------------------------
// ==========================================================

uint32_t OneHash (const char* string)
{
    return 1;
}

uint32_t FirstLetterHash (const char* string)
{
    return (uint32_t) string[0];
}


uint32_t LengthHash (const char* string)
{
    return strlen(string);
}


uint32_t SumHash (const char* string)
{
    uint32_t hashsum = 0;

    while (*string != '\0')
    {
        hashsum += *string;
        string++;
    }

    return hashsum;
}


uint32_t RorFunc (int num, int shift)
{
    return (num >> shift) | (num << (32 - shift));
}

uint32_t RolFunc (int num, int shift)
{
    return (num << shift) | (num >> (32 - shift));
}

uint32_t RolHash (const char* str)
{
    uint32_t hash  = 0;
    size_t   index = 0;

    while (*(str + index))
    {
        hash = RolFunc(hash, 1) ^ str[index];
    }

    return hash;
}

uint32_t RorHash (const char* str)
{
    unsigned int hash  = 0;
    size_t       index = 0;

    while (*(str + index))
    {
        hash = RorFunc(hash, 1) ^ str[index];
    }

    return hash;
}


uint32_t MurMurHash (const char* str)
{
    const unsigned int m = 0x5bd1e995;
    const unsigned int seed = 0;
    const int r = 24;
    const int len = strlen(str);

    unsigned int h = seed ^ len;

    const unsigned char * data = (const unsigned char *)key;
    unsigned int k = 0;

    while (len >= 4)
    {
        k  = data[0];
        k |= data[1] << 8;
        k |= data[2] << 16;
        k |= data[3] << 24;

        k *= m;
        k ^= k >> r;
        k *= m;

        h *= m;
        h ^= k;

        data += 4;
        len -= 4;
    }

    switch (len)
    {
    case 3:
        h ^= data[2] << 16;
    case 2:
        h ^= data[1] << 8;
    case 1:
        h ^= data[0];
        h *= m;
    };

    h ^= h >> 13;
    h *= m;
    h ^= h >> 15;

    return h;
}

