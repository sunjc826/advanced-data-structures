# advanced-data-structures
## Introduction
This repository implements the datastructures introduced in Advanced Data Structures by Peter Brass. The c files in the folders are pretty much copied from the book itself, whereas the datastructures in the project root are my own implementations based on the book, with tests and all. The software engineering aspect is significantly improved.

## Docs

As there are many implementations of the same abstract data type, one of these implementations need to be chosen as the implementation of the header file for that ADT.
- [Stack](stack.h): [v3](stack_v3.c)
- [Bounded Stack](bounded_stack.h): A mix of [v1](stack_v1.c) and [v2](stack_v2.c)
- [Queue](queue.h): [v4](queue_v4.c)
- [BBST](bbst.h): [height_balanced_tree](height_balanced_tree.c)

Stack, Bounded Stack, Queue are generic data structures, whereas BBST relies on (void *) to allow any sort of object at the leaves. As a result, BBST can only be included once, whereas the generic data structures are allowed to be included any number of times, with different namespaces specified.


In order to have a good understanding of a data structure, one ideally needs an algebraic characterization of it. This book characterizes data structures by implementing them, for instance, the model 1 search tree.
A characterization by implementation means that as long as the order of operations on the ADT represented by the data structure is valid (for e.g., we don't try to pop an empty stack), the implicit algebraic characteristics of the data structure should be maintained.
Nevertheless, the book does not give said algebraic characterizations, so it is good to make an attempt to characterize them.
