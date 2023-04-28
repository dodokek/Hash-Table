#include "include/hash_table.hpp"


extern "C" inline uint32_t MurMurAsm (const void* key, int length);


void StressTest(Text* Input, HashTable* self)
{
     std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();

    for (int useless_iter = 0; useless_iter < 100; useless_iter++)
    {
        for (int i = 1 ; i < Input->obj_amount; i++)
        {
            #ifdef AVX_SEARCH
                SearchMemberAVX (self, Input->objects[i].string, Input->objects[i].length);
            #else
                SearchMember (self, (char*) Input->objects[i].string, Input->objects[i].length);
            #endif
        }
    }

    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();

    printf ("Elapsed time(ms): %u\n", std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count());
    
}


void HashTableCtor (HashTable* self, size_t size, uint32_t (*hash_func) (const void* data, int len))
{
    LOG (">>>>>>Building Hash table\n");

    assert (size > 0 && self != nullptr);

    self->size = size;
    self->array = (HashTableNode*) aligned_alloc  (32, size * sizeof (HashTableNode));

    for (size_t i = 0; i < self->size; i++)
    {
        self->array[i].content = nullptr;
        self->array[i].next    = nullptr;
        self->array[i].length  = 0;
        self->array[i].peers = 0;
    }

    self->hash_func = hash_func;

    LOG ("End of build\n");

}


void HashTableDtor (HashTable* self)
{
    for (size_t i = 0; i < self->size; i++)
    {
        FreeRecurs (&(self->array[i]), true);
    }

    free(self->array);

    self->array     = nullptr;
    self->hash_func = nullptr;

    LOG ("<<<<<<Destroying table\n");
}


int FreeRecurs(HashTableNode* cur_node, bool begin)
{
    if (begin) return SUCCESS;

    if (cur_node->next == nullptr)
    {
        free(cur_node);
        return SUCCESS;
    }

    FreeRecurs (cur_node->next, false);
    
    free (cur_node);
    cur_node = nullptr;

    return SUCCESS;
}


// ==========================================================
// Control functions ----------------------------------------
// ==========================================================


int LoadData (Text* DataStruct, HashTable* self)
{
    for (int i = 0; i < DataStruct->obj_amount; i++)
    {
        AddMember (self, DataStruct->objects[i].string, DataStruct->objects[i].length);
    }

    return SUCCESS;
}


int AddMember (HashTable* self, __m256i* content, size_t len)
{
    // LOG ("Adding member <%s>\n", content);
    // assert (self != nullptr && content != nullptr);

    uint32_t key = self->hash_func(content, len) % (uint32_t) self->size;
        // LOG ("\tSTRING %s\n", (char*) content);

    if (SearchMember (self, (char*) content, len) == NOT_FOUND)
    {

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


bool SearchMember (HashTable* self, char* content, size_t len)
{
    uint32_t key = self->hash_func(content, len) % (uint32_t) self->size;

    return SearchInList (&(self->array[key]), content);    
}


bool SearchInList (HashTableNode* list_node, char* content)
{
    int peers = list_node->peers;

    for (int i = 0; i < peers; i++)
    {
        if (strcmp((char*)list_node->content, content) == 0)
            return SUCCESS_FOUND;
        
        list_node = list_node->next;
    }

    return NOT_FOUND;
}


bool SearchMemberAVX (HashTable* self, __m256i* content, size_t len)
{
    uint32_t key = self->hash_func(content, len) % (uint32_t) self->size;

    return SearchInListAVX(&(self->array[key]), content);  
}


bool SearchInListAVX (HashTableNode* list_node, __m256i* content)
{
    int peers = list_node->peers;

    for (int i = 0; i < peers; i++)
    {
        int cmp_mask = _mm256_testnzc_si256 (*content, *(list_node->content));

        if (cmp_mask == 0)
        {
            return SUCCESS_FOUND;
        }
        list_node = list_node->next;
    }

    return NOT_FOUND;
}



inline int asm_strcmp (const char* dst, const char* src)
{
    int result = 0;
    asm(".intel_syntax noprefix;"
        "mov rsi, %1;"
        "mov rdi, %2;"

        "pcmpeqb xmm0, xmm0;"

        "movdqu xmm1, [rsi];"
        "movdqu xmm2, [rdi];"
        "pcmpeqb xmm1, xmm2;"
        "xorps xmm1, xmm0;"
        "pmovmskb eax, xmm1;"
        
        "test eax, eax;"
        "jnz not_equal;"

        "pcmpeqb xmm0, xmm0;"
        "movdqu xmm1, [rsi + 16];"
        "movdqu xmm2, [rdi + 16];"
        "pcmpeqb xmm1, xmm2;"
        "xorps xmm1, xmm0;"
        "pmovmskb eax, xmm1;"
        "test eax, eax;"

        "jz equal;"
        "jnz not_equal;"

        "equal:\n"
        "xor rax, rax;"

        "not_equal:\n"
        "mov rax, 1;"

        ".att_syntax\n"
        : "=r" (result) 
        : "r" (dst), "r" (src) 
        : "rbx", "rsi", "rdi", "r10", "r11", "xmm1", "xmm2", "xmm0"
    );
    return result;
}







// ==========================================================
// Dump of table --------------------------------------------
// ==========================================================

// Removed

// ==========================================================
// All sort of Hashes ---------------------------------------
// ==========================================================


uint32_t MurMurMurHash (const void* data_, int len)
{
    const char* data = (char*) data_;

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


HashTableNode* CreateNode (__m256i* content)
{
    HashTableNode* new_node = (HashTableNode*) calloc (1, sizeof(HashTableNode));

    new_node->content = content;
    new_node->peers   = 1;                  // initially, node is the only peer in the cell
    new_node->next    = nullptr;

    return new_node;
}
