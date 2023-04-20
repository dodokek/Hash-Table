# Hash table. Hash functions study & assembly optimizations

## Overview

Goals of this project:
- Inspect different hash functions and choose the best, by **minimizing** the amount of **collisions**.
- Implement 3 types of **assembly optimizations** to speed up the *searching function*
  
> I assume that reader is familiar with the concept of hash tables and processor architecture.

## Hash table structure

In this project I used **chain method** to avoid collisions. 

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

**System info:** Intel Core i5, 5th gen. Honor MagicBook 16 R5/16/512 </br>
**Compilator info:** g++ (Ubuntu 11.3.0-1ubuntu1~22.04) 11.3.0	</br>
**Optimization flag:** -O2.	</br>
>I've chosen -O2 optimization flag, because I will use AVX2 instructions. You can check my<a href="https://github.com/dodokek/Mandelbrot-Fractal"> previous project</a> for full explanation.

### Version 0 - no optimizations

Before start I removed all unneeded functions: 
- I removed all hashes apart from Murmur Hash
- Removed all sorts of dump
  
Also I used *\<chrono>* library to measure the elapsed time.

>Average search time: 3142 $\pm$ 20 ms

### Version 1 - Replacing Murmur hash with its assembly version.

Let's have use a profiler and find the most *heated* parts of our program.

<img src="https://user-images.githubusercontent.com/57039216/232879463-e4540b75-9166-47e7-bfb7-bf1d400e74a0.png" width="500px">


According to profiler, Murmur Hash affects the performance the most.
We can rewrite it on Assembly. I will implement it in the separate file and then link it with our main program.

<details>
<summary>Murmur hash assembly code</summary>

~~~C++
MurMurAsm:
	push rbp				; saving base pointer 	
	
	push rbx

	; rdi - string
	; rsi - len

	mov eax, 5bd1e995h	; hash = seed ^ len
	xor rax, rsi

	mov rcx, 0			; coef1 = 0

	
.loop:
	mov rcx, [rdi]		; coef1 = data[0]

	mov rbx, [rdi + 1]		; coef1 = data[1] << 8	
	mov rdx, [rdi + 2]		; coef1 = data[1] << 8
	mov r11, [rdi + 3]		; coef1 = data[1] << 8
	
	shl rbx, 8
	shl rdx, 16
	shl r11, 24

	or rcx, rbx
	or rcx, rdx
	or rcx, r11

	add rdi, 4

	mov edx, 5bd1e995h
	mul edx				; hash *= salt
	xor rax, rcx		; hash ^= coef1

	sub rsi, 4d			; len -=4

	cmp rsi, 4
jge	.loop

	; switch (len)
	cmp rsi, 3	; case 3
	jne skip3

		mov rbx, [rdi + 2]		; hash ^= data[2] << 16
		shl rbx, 16
		xor rax, rbx

	skip3:
	cmp rsi, 2	; case 2
	jne skip2

		mov rbx, [rdi + 1]		; hash ^= data[2] << 8
		shl rbx, 8
		xor rax, rbx

	skip2:
	cmp rsi, 1	; case 1
	jne skip1

		mov rbx, [rdi]		; hash ^= data[0];
		xor rax, rbx
		mov edx, 5bd1e995h
		mul edx		; hash *= salt

	skip1:

	mov edx, eax	; hash ^= hash >> 13
	shr edx, 13
	xor eax, edx

	mov edx, 5bd1e995h
	mul edx				; hash *= salt

	mov edx, eax	; hash ^= hash >> 15
	shr edx, 15
	xor eax, edx

	pop rbx

	pop rbp

	ret


~~~
</details>
I've tried to combine together operations with memory work and bites shuffling for better conveyer work. This gave me 5% relative performance boost.
</br>
</br>

>Average search time: 2850 $\pm$ 20 ms

| Version     | Abs. speedup      | Rel. speedup   | 
| ------      | :---------------: | :------------: | 
| -O2      | 1                 | 1              | 
| Assembly Hash  | 1.11              | 1.11           |

### Replacing strcmp with inline assembly

Let's once again look on profiler data:

![image](https://user-images.githubusercontent.com/57039216/232726675-bdfd4ac9-d853-4b0e-853f-ddac212d7697.png)

According to profiler, the next target is strcmp function. I will rewrite it with **inline assembly**.

<details>
<summary>Strncpy inline assembly implementation</summary>

~~~C++
asm(".intel_syntax noprefix;"
        "loop:"
            "push r9;"
            "push r10;"

            "mov r9b, [rsi];"
            "mov r10b, [rdi];"

    	    "cmp r9b, 0;"
    	    "je end;"
    	    "cmp r10b, 0;"
    	    "je end;"

    	    "cmp r9b, r10b;"
    	    "jne end;"
    	    "inc rdi;"
    	    "inc rsi;"
    	    "jmp loop;"
        "end:"
    	    "movzx rax, r9b;"
    	    "movzx rbx, r10b;"
    	    "sub rax, rbx;"

            "pop r10;"
            "pop r9;"


        ".att_syntax" 
        : "=a" (result)
        : "S" (dst), "D" (src) : "rax", "rbx", "rsi", "rdi", "r9", "r10"
    );
~~~
</details>
</br>

>Average search time: 5416 $\pm$ 30 ms

| Version     | Abs. speedup      | Rel. speedup   | 
| ------      | :---------------: | :------------: | 
| -O2      | 1                 | 1              | 
| Assembly Hash  | 1.11              | 1.11           |
| Assembly strcmp  | 0.58              | 0.52          |

It turns out, that **original** version of *strcmp* is **much faster**. Unfortunately, this optimisation is useless. I won't count it in the final ratings.

We need to find another way to speed up the program.

## Using AVX2 instructions in Search function

From **Length hash** we already know, that there are no words, longer than 20 symbols. It means we can fit each of them into *__m256i* format. 

<details>
<summary>Unoptimized Search code</summary>

~~~C++
bool SearchMember (HashTable* self, const char content[], size_t len)
{
    uint32_t key = self->hash_func(content, len) % (uint32_t) self->size;

    HashTableNode* cur_node = &(self->array[key]);

    int peers = cur_node->peers;
    for (int i = 0; i < peers; i++)
    {
        if (strcmp((char*)cur_node->content, content) == 0)
            return SUCCESS_FOUND;
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
    strncpy (word_buffer, content, len);

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

To get the most out of it, I will transform array of *char** input words into *_mm256i*. With that, we can ged rid of strncpy function, which slows the program.

<details>
<summary>AVX2 optimized Search code with modified input data </summary>

~~~C++
bool SearchMemberAVX (HashTable* self, __m256i* content, size_t len)
{
    uint32_t key = self->hash_func(content, len) % (uint32_t) self->size;

    HashTableNode* cur_node = &(self->array[key]);

    int peers = cur_node->peers;
    for (int i = 0; i < peers; i++)
    {
        int cmp_mask = _mm256_testnzc_si256 (*content, *(cur_node->content));

        if (cmp_mask == 0)
            return SUCCESS_FOUND;

        cur_node = cur_node->next;
    }

    return NOT_FOUND;
}
~~~
</details>
</br>



>Average search time: 2056 $\pm$ 20 ms


| Version     | Abs. speedup      | Rel. speedup   | 
| ------      | :---------------: | :------------: | 
| -O2      | 1                 | 1              | 
| Assembly Hash  | 1.11              | 1.11        |
| AVX Search| 1.56              | 1.35          |


![image](https://user-images.githubusercontent.com/57039216/232727215-a838c29f-2d64-4905-8b8d-1d510cb31b8e.png)
>0x00...1ef0 - Murmur Hash assembly function

The full list of functions in kcachegrind.

![image](https://user-images.githubusercontent.com/57039216/232727382-007cd349-23de-4e75-81e1-bdaaaa108b9d.png)

According to profiler, there are no more *"bottle necks"* where we can get noticeable rise in performance.

## Increasing Hash table size

For all this time, we had 1000 cells in our Hast table. Thanks to that, we got all this optimization ideas. It is time to increace amount of cells from 1000 to 150000. 

Amount of collisions will drop tremendeously and the performance will rise.

>Average search time: 1236 $\pm$ 10 ms

| Version     | Abs. speedup      | Rel. speedup   | 
| ------      | :---------------: | :------------: | 
| -O2      | 1                 | 1              | 
| Assembly Hash  | 1.11              | 1.11        |
| AVX Search| 1.56              | 1.35          |
| Size increace | 2.54              | 1.66          |

## Conclusion

It turns out, that **not every optimization speeds up** the program, **sometimes** it only **slows it down**.

At the end we got  *Search function*'s performance increase 1.56 times.
Let's calculate Ded's coefficient:

$Coef_{ded}  = \frac{acceleration}{assembly\space lines} \cdot 1000$

$Coef_{ded} = \frac{1.56}{80} \cdot 1000 \approx 20$


