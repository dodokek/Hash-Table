# Hash table. Hash functions study & assembly optimizations

## Overview

Goals of this project:
- Inspect different hash functions and choose the best, by **minimizing** the amount of **collisions**.
- Implement 3 types of **assembly optimizations** to speed up the *searching function*
  
> I assume that reader is familiar with the concept of hash tables and processor architecture.

## Hash table structure

In this project used **chain method** to avoid collisions. 

Elements with the same hash are stored in linked list.
Here is the picture:

![image](https://user-images.githubusercontent.com/57039216/231858087-bd148c90-40c6-4f20-bdd8-14fe31f474ab.png)

With proper size and hash function, we can search elements for O(1).

You can read more about it here:</br>
https://www.geeksforgeeks.org/separate-chaining-collision-handling-technique-in-hashing/


## Part 1. Choosing the best hash function

In this part we will *inspect 7 hash functions*. The main evaluation parameter will be *amount of collisions*.

The **more** collisions we have - the **slower** searching and adding new members will get. That's why this part is really important.

Each time I will load ~14 000 unique words into the Hash Table. Size of the table is set to 1000. This is done on purpose, to get clearer vision of hash functions' capabilities. 

With *python library matplotlib* I will draw graphs to see how much collisions we get. 

### Return 1 Hash

This is *the worst* ever hash function that you can imagine. All elements will be stored in one linked list. 

Code:
~~~C++
uint32_t OneHash (const char* string)
{
    return 1;
}
~~~

![image](https://user-images.githubusercontent.com/57039216/232117825-c30a9f24-dcf6-43f6-889e-5c9d8704f512.png)


The search would be *far from O(1)*. I implemented it for educational purposes only.

### First letter ASCII code hash

We will return the ASCII number of the first character in the string. 

~~~C++
uint32_t FirstLetterHash (const char* string)
{
    return (uint32_t) string[0];
}
~~~

![image](https://user-images.githubusercontent.com/57039216/232112161-f3380d6c-76e4-4a8f-8b6b-742ae0972446.png)



This hash function might be used somewhere. However we have 14000 words, which means a lot of collisions and empty buckets.


### Length Hash

We will return string length. 

~~~C++
uint32_t LengthHash (const char* string)
{
    return (uint32_t) strlen(string);
}
~~~

![image](https://user-images.githubusercontent.com/57039216/232111816-76c26dfa-6f5b-401a-b284-ac565ec42599.png)


We have even more collisions. English language has on average 5 letters in the word, which leads to 2500 collisions at the top. 


### Hashsum

We will return the ASCII sum of characters in the string. 

~~~C++
uint32_t SumHash (const char* string)
{
    uint32_t hashsum = 0;

    while (*string)
    {
        hashsum += *string;
        string++;
    }

    return hashsum;
}
~~~

![image](https://user-images.githubusercontent.com/57039216/232112313-789f9850-72b3-43be-bbca-6d973615aef1.png)


You can notice the huge improvement. This is one of the simplest and effective algorithms. Maximal amount of collisions decreased 400 times :)


### Rotate-right hash

This hash would be calculated with the help of *Rotate function*, which works like that:

~~~
Say binary number 01001001 equals to abcdefgh

abcdefgh --> *RotateRight* --> habcdefg 
~~~

From $01001001_2$ we get $10100100_2$

Here is the code:

~~~C++
uint32_t RorHash (const char* string)
{
    unsigned int hash  = 0;
    size_t       index = 0;

    while (*str)
    {
        hash = RotateRight(hash) ^ str[index];
        str++;
    }

    return hash;
}
~~~

![image](https://user-images.githubusercontent.com/57039216/232112048-4300c910-d112-48df-bc87-acb17a5753be.png)


We didn't get less collisions, but graph looks a bit smoother now. The search should be statistically quicker, if we pick a random element.


### Rotate-left hash

The same algorithm as previous, but bits rotating to the left.

~~~C++
uint32_t RorHash (const char* string)
{
    unsigned int hash  = 0;
    size_t       index = 0;

    while (*str)
    {
        hash = RotateLeft(hash) ^ str[index];
        str++;
    }

    return hash;
}
~~~

![image](https://user-images.githubusercontent.com/57039216/232111990-2e79096b-82ac-4215-a861-1b9ce8463661.png)


Pretty much the same result. If I had to choose from Rotate Hashes and previous Hash functions, I would definitely picked Rotate Hashes.  

However, we have much more efficient and complicated function...

### Murmur hash

The realization is pretty complicated, we will go through it later in part with optimizations.

With the help of this algorithm, we can have as much as 27 collisions maximally. It is *3 times better* than Rotate Hash. 


![image](https://user-images.githubusercontent.com/57039216/232111919-70df2e82-0bd3-4330-b8ee-2356f41c8f83.png)


I will use this *Hash function* for the next part of my project because it has a *great potential for optimizations*.

### Conclusion

Let's look at all functions *on the same* graphic:

![image](https://user-images.githubusercontent.com/57039216/232108285-e3d2b7d7-bb6c-4171-842b-99b35a0888b2.png)

> Green - Murmur hash.
> Black - Length hash.
> Red - Hashsum.
> Brown - First ASCII hash.
> Blue and Cyan - Rotate hashes.
> Return 1 hash is excluded.

Then let's have a **closer look** at our favorites:

![image](https://user-images.githubusercontent.com/57039216/232110503-21f4e0cf-92c4-418a-b26c-962ae22ecbb7.png)

> Green - Murmur hash.
> Blue - Rotate right hash. 
> Cyan - Rotate left hash.
> Red  - Hashsum.

At this point the supremacy of Murmur hash is obvious. If we had less words, I might have used Hashsum, but Murmur hash is much more fun to optimize.

## Part 2. Optimizations

In this part of the work we will **speed up our search function** by analysing *"bottle necks"* of the program.

I will use **valgrind** to get profiling data and **kcachegrind** to visualize it. Also I will implement a stress-test, which searches *each word* in hash table *100 times*.

Environment info: I tested the program in the same room, with the same temperature. Battery settings weren't changing during the experiment. 

System info: Intel Core i5, 5th gen. Honor MagicBook 16 R5/16/512

### Ver.0 - no optimizations

Before start I removed all unneeded functions: 
- I removed of all hashes apart from Murmur Hash
- Removed all sorts of dump
  
Also I used *\<chrono>* library to measure the elapsed time.

>Average search time: 3750 $\pm$ 20 ms

Let's have a look at a profiler and find the most *heated* parts of our program.

![image](https://user-images.githubusercontent.com/57039216/232408535-e38a0e44-a9ab-4ae2-84cf-74215aaac25d.png)

According to profiler, first of all we should optimize *Searching function* itself.

### Ver.1 - AVX optimization of Search func.

From **Length hash** we already know, that there are no words, longer than 20 symbols. It means we can fit each of them into *__m256i* format. 


<details>
<summary>Unoptimized Search code</summary>

~~~C++
bool SearchMember (HashTable* self, const char content[], size_t len)
{
    uint32_t key = self->hash_func(content, len) % (uint32_t) self->size;

    HashTableNode* cur_node = self->array[key];

    int peers = cur_node->peers;

    for (int i = 0; i < peers; i++)
    {
        if (cur_node->peers == 0) break;

        if (strcmp(cur_node->content, content) == 0)
        {
            return SUCCESS_FOUND;
        }
        cur_node = cur_node->next;
    }

    return NOT_FOUND;
}
~~~
</details>

</br>

With the help of AVX2 instructions we'll be able to compare strings much faster. Let's *rewrite Searching function* with them.


<details>
<summary>AVX2 optimized Search code</summary>

~~~C++
bool SearchMemberAVX (HashTable* self, const char content[], size_t len)
{
    uint32_t key = self->hash_func(content, len) % (uint32_t) self->size;
    HashTableNode* cur_node = self->array[key];

    alignas(32) char word_buffer[MAX_WORD_LEN] = "";
    
    strcpy (word_buffer, content);
    __m256i content_avx = _mm256_load_si256 ((__m256i*) word_buffer);  

    int peers = cur_node->peers;
    for (int i = 0; i < peers; i++)
    {
        __m256i cur_val_avx = _mm256_load_si256 ((__m256i*) cur_node->content);
        __m256i cmp_mask    = _mm256_cmpeq_epi8 (content_avx, cur_val_avx);
        uint32_t int_cmp_mask = (uint32_t) _mm256_movemask_epi8(cmp_mask);

        if (int_cmp_mask == 0xFFFFFFFF)
            return SUCCESS_FOUND;

        cur_node = cur_node->next;
    }

    return NOT_FOUND;
}
~~~
</details>
</br>

>Average search time: 1772 $\pm$ 16 ms


| Version     | Abs. speedup      | Relative speedup   | 
| ------      | :---------------: | :------------: | 
| No Op.      | 1                 | 1              | 
| AVX Search  | 2.11              | 2.11           | 

We can see a huge increase in speed, let's look what profiler says:

![image](https://user-images.githubusercontent.com/57039216/232407643-3df80dc2-0de0-4d03-9f81-f369795d559d.png)

The next target is **Hash function**.

### Ver.2 - Hash function with Assembly

To improve the performance of the Murmur hash, we can rewrite it on Assembly.
I will implement it in the separate file and then link it with our main program.

<details>
<summary>Murmur hash assembly code</summary>

~~~C++
section .text

global MurMurAsm


MurMurAsm:
	pop r13				
	push rbp			

	; rdi - string
	; rsi - len

	mov eax, 5bd1e995h	; hash = seed ^ len
	xor rax, rsi

	mov rcx, 0			; coef1 = 0

	
.loop:
	mov rcx, [rdi]		; coef1 = data[0]
	inc rdi

	mov rbx, [rdi]		; coef1 = data[1] << 8
	shl rbx, 8
	or rcx, rbx
	inc rdi

	mov rbx, [rdi]		; coef1 = data[2] << 16
	shl rbx, 16
	or rcx, rbx
	inc rdi

	mov rbx, [rdi]		; coef1 = data[3] << 24
	shl rbx, 24
	or rcx, rbx
	inc rdi

	mov edx, 5bd1e995h
	mul edx		        ; hash *= salt
	xor rax, rcx		; hash ^= coef1

	sub rsi, 4d			; len -=4

	cmp rsi, 4
jge	.loop

	xor rbx, rbx
	                    ; switch (len)
	cmp rsi, 3	        ; case 3
	jne skip3

		mov rbx, [rdi + 2] ; hash ^= data[2] << 16
		shl rbx, 16
		xor rax, rbx

	skip3:
	cmp rsi, 2	        ; case 2
	jne skip2

    mov rbx, [rdi + 1]  ; hash ^= data[2] << 8
    shl rbx, 8
    xor rax, rbx

	skip2:
	cmp rsi, 1	        ; case 1
	jne skip1

    mov rbx, [rdi]		; hash ^= data[0];
    xor rax, rbx
    mov edx, 5bd1e995h
    mul edx		        ; hash *= salt

	skip1:

	mov edx, eax	    ; hash ^= hash >> 13
	shr edx, 13
	xor eax, edx

	mov edx, 5bd1e995h
	mul edx				; hash *= salt

	mov edx, eax	    ; hash ^= hash >> 15
	shr edx, 15
	xor eax, edx

	pop rbp
	push r13

	ret

~~~
</details>

</br>

>Average search time: 1387 $\pm$ 20 ms

| Version     | Abs. speedup      | Rel. speedup   | 
| ------      | :---------------: | :------------: | 
| No Op.      | 1                 | 1              | 
| AVX Search  | 2.11              | 2.11           |
| AVX + Asm. MurmurHash  | 2.78             | 1.27           | 

Performance growth is not as big as in *Version 1*, but still impressive.
Let's once again look at profiler data:

![image](https://user-images.githubusercontent.com/57039216/232408082-cb567fcf-db5e-4ba2-b4a9-ab0046bd2e64.png)

More detailed:

![image](https://user-images.githubusercontent.com/57039216/232338074-ee415db8-65f9-4bbc-be66-f67b44a1960c.png)

>0x000...02820 function - Assembly Murmur hash. 

The last *"bottle neck"* we've got is **strcpy function**, which is used in AVX2 implementation of search function. 

### Ver. 3 Inline assembler

Using GNU inline assembler, I am going to rewrite strcpy function. Theoretically, it also might improve performance.

<details>
<summary>Strcpy inline assembly implementation</summary>

~~~C++
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
~~~
</details>
</br>

>Average search time: 1316 $\pm$ 10 ms

| Version     | Abs. speedup      | Rel. speedup   | 
| ------      | :---------------: | :------------: | 
| No Op.      | 1                 | 1              | 
| AVX Search  | 2.11              | 2.11           |
| AVX + Murmur Hash assembler  | 2.78             | 1.27        | 
| -//- + Inline assembler  | 2.85             | 1.06       | 

Relative growth in performance is only 6%. Inline assembler has a lot of disadvantages, so it is not worth it to use this optimization. I will keep it in education purposes only <3.  

As long as *Search function*, *Hash function* and *strcpy function* are still on the top of the callgrind list, there is no point in other optimizations. I will stop here.

## Conclusion

At the end we got  *Search function*'s performance increase 2.85 times. In my opinion this is quite good result.

Let's calculate Ded's coefficient, to confirm my words:

$Coef_{ded}  = \frac{acceleration}{assembly\space lines} \cdot 1000$

$Coef_{ded} = \frac{2.85}{92} \cdot 1000 \approx 31$


