CSE.30341.FA17: Project 04
==========================

This is the documentation for [Project 04] of [CSE.30341.FA17].

Members
-------

1. Anthony Luc (aluc@nd.edu)
2. Donald Luc (dluc@nd.edu)
3. Michael Wang (mwang6@nd.edu)

Design
------

> 1. You will need to implement splitting of free blocks:
>
>   - When should you split a block?
>
>   - How should you split a block?

Response.
After searching through the free list and finding a free block large enough for our memory usage.
We split that block into two parts: one just enough for our memory usage and the other having just the remainder memory not used.


> 2. You will need to implement coalescing of free blocks:
>
>   - When should you coalescing block?
>
>   - How should you coalesce a block?

Response.
As we search through the free list we can coalesce free blocks into larger free blocks.
By creating a a block that equals the size of the coalescing blocks, we set the new block's next to the next of the latter coalesced block.

> 3. You will need to implement Next Fit.
>
>   - What information do you need to perform a Next Fit?
>
>   - How would you implement Next Fit?

Response.
By keeping track of where you left off in the free list, you start looking for the first block that will fit the space needed.
By keeping track of the pointer of the last block you can pass the first fit function the pointer.

> 4. You will need to implement Best Fit.
>
>   - What information do you need to perform a Best Fit?
>
>   - How would you implement Best Fit?

Response.
We need the size of the smallest block that fits the required memory.
Iterate through the entire free list and return the pointer of the best fit block and then split said block.

> 5. You will need to implement Worst Fit.
>
>   - What information do you need to perform a Worst Fit?
>
>   - How would you implement Worst Fit?

Response.
We need the size of the largest block that fits the required memory.
Iterate through the entire free list and return the pointer of the worse fit block and then split said block.

> 6. You will need to implement tracking of different information.
>
>   - What information will you want to track?
>
>   - How will you update these trackers?
>
>   - How will you report these trackers when the program ends?

Response.
We will keep track the number of mallocs, frees, reuses, grows, splits, coalesces, blocks, total memory requested, and maximum size of heap.
We will have global variables for each that will be updated in their respective functions.
We will use an atexit() function that calls sprintf and write to create a reporting string.

Demonstration
-------------

> Place a link to your demonstration slides on [Google Slides].

Errata
------

> We were able to successfully run all the test scripts and they outputted success. We checked for segfaults for our testests and benchmarks. We didn't run into any problems when running the benchmarks or test scripts.

Extra Credit
------------

> Describe what extra credit (if any) that you implemented.

[Project 04]:       https://www3.nd.edu/~pbui/teaching/cse.30341.fa17/project04.html
[CSE.30341.FA17]:   https://www3.nd.edu/~pbui/teaching/cse.30341.fa17/
[Google Slides]:     https://docs.google.com/presentation/d/1sFeU5nzkw1ANCcft0e6bysxch8tC4dOMWeSmVHRHRIQ/
