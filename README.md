# Hash table - hash functions study and  assembly optimizations

## Overview

Goals of this project:
- Inspect different hash functions and choose the best, by **minimizing** the amount of **collisions**.
- Implement 3 types of **assembly optimizations** to speed up the *searching function*
  
> I asume that reader is familiar with the concept of hash tables and CPU main features.

## Hash table structure

In this project used **chain method** to avoid collisions. 

Elements with the same hash are stored in linked list.
Here is the picture:

![image](https://user-images.githubusercontent.com/57039216/231858087-bd148c90-40c6-4f20-bdd8-14fe31f474ab.png)

With proper size and hash function, we can search elements by O(1).

You can read more about it here:
https://www.geeksforgeeks.org/separate-chaining-collision-handling-technique-in-hashing/


## Choosing the best hash function

In this part we will inspect 6 hash functions. The main evaluation parameter will be amount of collisions.

The **more** collisions we have - the **slower** searching and adding new members will get. That's why this part is really important.

We will load in Hash table 2000 words and test it's limits.

### Return 1 Hash

This is *the worst* ever hash function that you can imagine. All elements will be stored in one linked list. 

Code:
~~~C++
uint32_t OneHash (const char* string)
{
    return 1;
}
~~~

![image](https://user-images.githubusercontent.com/57039216/231859825-955f33bc-6b8f-48a1-bcad-b4bac816ec7f.png)


The search would be *far from O(1)*. I implemented it for educational purposes only.

### First letter ASCII code hash

We will return the ASCII number of the first character in the string. 

~~~C++
uint32_t FirstLetterHash (const char* string)
{
    return (uint32_t) string[0];
}
~~~

![image](https://user-images.githubusercontent.com/57039216/231859825-955f33bc-6b8f-48a1-bcad-b4bac816ec7f.png)



This hash function might be used somewhere. However we have 4000 words, wich means a lot of collisions and empty buckets.


### Length Hash

We will return string length. 

~~~C++
uint32_t LengthHash (const char* string)
{
    return (uint32_t) strlen(string);
}
~~~

![image](https://user-images.githubusercontent.com/57039216/231859825-955f33bc-6b8f-48a1-bcad-b4bac816ec7f.png)


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

![image](https://user-images.githubusercontent.com/57039216/231859825-955f33bc-6b8f-48a1-bcad-b4bac816ec7f.png)


You can notice the huge improvement. This is one of the simpliest and effective algorithms. Maximal amount of collisions decreaced 400 times :)


### Rotate-right hash

This hash would be calculated according to this formula:

$ Bebrocka  1000-7$

~~~C++
uint32_t RorHash (const char* string)
{
    unsigned int hash  = 0;
    size_t       index = 0;

    while (*str)
    {
        hash = RorFunc(hash, 1) ^ str[index];
        str++;
    }

    return hash;
}
~~~

![image](https://user-images.githubusercontent.com/57039216/231859825-955f33bc-6b8f-48a1-bcad-b4bac816ec7f.png)


Despite the amount of collisions didn't decreace much, we can see the improvement in the other parametr - dispersion.... Ya zadolbalsya
