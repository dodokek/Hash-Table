#include "include/hash_table.hpp"

extern "C" inline uint32_t MurMurAsm (const char* key, int length);

void HashTableCtor (HashTable* self, size_t size, HASH_FUNC_CODES hash_code)
{
    LOG (">>>>>>Building Hash table\n");
    LOG ("\tCode: %d\n", hash_code);

    assert (size > 0 && self != nullptr);

    self->size = size;
    self->array = (HashTableNode*) calloc (size, sizeof (HashTableNode));
    self->hash_code = hash_code;

    // LOG ("Setting array\n");

    for (size_t i = 0; i < self->size; i++)
    {
        self->array[i].content = nullptr;
        self->array[i].next    = nullptr;
        self->array[i].length  = 0;
        self->array[i].peers = 0;
        self->array[i].is_head = true;
    }

    switch (hash_code)
    {
    case MURMUR_HASH:
        self->hash_func = MurMurMurHash;
        break;

    case MURASM_HASH:
        self->hash_func = MurMurAsm;
        break;
    
    default:
        break;
    }

    LOG ("End of build\n");

}


void HashTableDtor (HashTable* self)
{
    for (size_t i = 0; i < self->size; i++)
    {
        FreeRecurs (&(self->array[i]));
    }

    free(self->array);
    self->array     = nullptr;
    self->hash_func = nullptr;

    LOG ("<<<<<<Destroying table\n");
}


int FreeRecurs(HashTableNode* cur_node)
{
    if (cur_node->is_head) return SUCCESS;

    if (cur_node->next == nullptr)
    {
        free(cur_node);
        return SUCCESS;
    }

    FreeRecurs (cur_node->next);
    free (cur_node);

    return SUCCESS;
}


// ==========================================================
// Control functions ----------------------------------------
// ==========================================================


int LoadData (Text* DataStruct, HashTable* self)
{
    for (int i = 0; i < DataStruct->obj_amount; i++)
    {
        AddMember (self, DataStruct->objects[i].begin, DataStruct->objects[i].length);
    }

    return SUCCESS;
}


int AddMember (HashTable* self, const char* content, size_t len)
{
    // LOG ("Adding member <%s>\n", content);
    assert (self != nullptr && content != nullptr);

    uint32_t key = self->hash_func(content, len) % (uint32_t) self->size;
    
    if (SearchMember (self, content, len) == NOT_FOUND)
    {
        // LOG ("New member, key %u\n", key);

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
    new_node->is_head = false;

    return new_node;
}


bool SearchMember (HashTable* self, const char content[], size_t len)
{
    uint32_t key = self->hash_func(content, len) % (uint32_t) self->size;

    HashTableNode* cur_node = &(self->array[key]);

    int peers = cur_node->peers;

    for (int i = 0; i < peers; i++)
    {
        if (cur_node->peers == 0) break;

        if (strcmp(cur_node->content, content) == 0)
        {
            // LOG ("=== Found %s, key: %u\n", content, key);
            return SUCCESS_FOUND;
        }
        cur_node = cur_node->next;
    }

    // LOG ("xxx Not found %s, key: %u\n", content, key);

    return NOT_FOUND;
}

bool SearchMemberAVX (HashTable* self, const char content[], size_t len)
{
    uint32_t key = self->hash_func(content, len) % (uint32_t) self->size;
    HashTableNode* cur_node = &(self->array[key]);

    alignas(32) char word_buffer[MAX_WORD_LEN] = "";
    
    strcpy (word_buffer, content);
    __m256i content_avx = _mm256_load_si256 ((__m256i*) word_buffer);  

    int peers = cur_node->peers;
    for (int i = 0; i < peers; i++)
    {
        // strcpy (word_buffer, cur_node->content);
        __m256i cur_val_avx = _mm256_load_si256 ((__m256i*) word_buffer);
        __m256i cmp_mask    = _mm256_cmpeq_epi8 (content_avx, cur_val_avx);
        uint32_t int_cmp_mask = (uint32_t) _mm256_movemask_epi8(cmp_mask);

        // printf ("Cmp mask: %x\n", int_cmp_mask);
        if (int_cmp_mask == 0xFFFFFFFF)
        { 
            LOG ("+++ Found %s, key: %u\n", content, key);
            return SUCCESS_FOUND;
        }

        cur_node = cur_node->next;
    }

    LOG ("xxx Not found %s, key: %u\n", content, key);

    return NOT_FOUND;
}


void asm_strcpy (char* dst, const char* src)
{
    asm(".intel_syntax noprefix;"
        
        "dec rdi;"
        "dec rsi;"

        "loop:"
            "mov r10b, byte [rsi];"
    	    
            "cmp r10b, 0;"
    	    "je end;"

            "mov byte [rdi], r10b;"
    	    
            "inc rdi;"
    	    "inc rsi;"
    	    "jmp loop;"

        "end:"
        "mov ah, 0;"
        "mov byte [rdi], ah;"

        ".att_syntax"
    );
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

    for (int i = 0; i < self->size; i++)
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


void DumpTableInCsv (HashTable* self, FILE* csv_file)
{

    for (size_t i = 0; i < self->size; i++)
        fprintf (csv_file, "%lu ", i);

    fprintf (csv_file, "\n");

    for (size_t i = 0; i < self->size; i++)
        fprintf (csv_file, "%d ", self->array[i].peers);
    
    fprintf (csv_file, "\n-------\n");

}

// ==========================================================
// All sort of Hashes ---------------------------------------
// ==========================================================


uint32_t MurMurMurHash (const char* data, int len)
{
    const unsigned int salt = 0x5bd1e995;
    const unsigned int seed = 0;
    const int offst = 24;

    unsigned int hash = seed ^ len;

    unsigned int coef1 = 0;

    while (len >= 4)
    {
        coef1  = data[0];
        coef1 |= data[1] << 8;
        coef1 |= data[2] << 16;
        coef1 |= data[3] << 24;

        coef1 *= salt;
        coef1 ^= coef1 >> offst;
        coef1 *= salt;

        hash *= salt;
        hash ^= coef1;

        data += 4;
        len -= 4;
    }

    switch (len)
    {
    case 3:
        hash ^= data[2] << 16;
    case 2:
        hash ^= data[1] << 8;
    case 1:
        hash ^= data[0];
        hash *= salt;
    };

    hash ^= hash >> 13;
    hash *= salt;
    hash ^= hash >> 15;

    return hash;
}

