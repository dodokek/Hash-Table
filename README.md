# Hash table - hash functions study and  assembly optimizations

## Overview

Goals of this project:
- Inspect different hash functions and choose the best, by **minimizing** the amount of **collisions**.
- Implement 3 types of **assembly optimizations** to speed up the *searching function*
  
> I asume that reader is familiar with the concept of hash tables and assembly optimizations.

## Hash table structure

In this project used **chain method** to avoid collisions. 

Elements with the same hash are stored in the linked list.
Here is the picture:

![image](https://user-images.githubusercontent.com/57039216/231858087-bd148c90-40c6-4f20-bdd8-14fe31f474ab.png)

With proper size and hash function, we can search elements by O(1).

You can read more about it here:
https://www.geeksforgeeks.org/separate-chaining-collision-handling-technique-in-hashing/


## Part 1. Choosing the best hash function

In this part we will inspect 6 hash functions. The main evaluation parameter will be amount of collisions.

The **more** collisions we have - the **slower** searching and adding new members will get. That's why this part is really important.

Each time I will load ~15 000 words into the Hash Table.

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



This hash function might be used somewhere. However we have 4000 words, which means a lot of collisions and empty buckets.


### Length Hash

We will return string length. 

~~~C++
uint32_t LengthHash (const char* string)
{
    return (uint32_t) strlen(string);
}
~~~

![image](https://user-images.githubusercontent.com/57039216/232111816-76c26dfa-6f5b-401a-b284-ac565ec42599.png)


We have even more collisions. English language has ~5 letters in the word, which leads to 2500 collisions at the top. 


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


We didn't get fewer collisions, but dispersion became much better. It means the searching would be statistically quicker, if we pick random element.


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


Dispersion became slightly better. If I needed to choose among this functions, this function would be the optimal one. 

However, we have much more effecient and complicated function...

### Murmur hash

The realization is quite difficult for understanding, we will go through it later in part with optimizations.

With the help of this algorithm, we can have as much as 27 collisions maximally. It is *3 times better* than Rotate Hash. 

Dispersion is also the best among all hash functions we studied.


![image](https://user-images.githubusercontent.com/57039216/232111919-70df2e82-0bd3-4330-b8ee-2356f41c8f83.png)


I will use this *Hash function* for the next part of my project because it has a *great potential for optimizations*.

### Conclusion

Let's look at all functions *on the same* graphic:

![image](https://user-images.githubusercontent.com/57039216/232108285-e3d2b7d7-bb6c-4171-842b-99b35a0888b2.png)

> Green - Murmur hash
> Black - Length hash
> Red - Hashsum
> Brown - First ASCII hash
> Blue and Cyan - Rotate hashes
> Return 1 hash is excluded

Then let's have a **closer look** at our favorites:

![image](https://user-images.githubusercontent.com/57039216/232110503-21f4e0cf-92c4-418a-b26c-962ae22ecbb7.png)

> Green - Murmur hash
> Blue - Rotate right hash 
> Cyan - Rotate left hash
> Red  - Hashsum

At this point the supremacy of Murmur hash is obvious.

## Part 2. Optimizations

## Conclusion

## Useful links
