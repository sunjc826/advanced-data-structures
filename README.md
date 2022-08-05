# advanced-data-structures

As there are many implementations of the same abstract data type, one of these implementations need to be chosen as the implementation of the header file for that ADT.
- [Stack](stack.h): [v3](stack_v3.c)
- [Queue](queue.h): [v4](queue_v4.c)


In order to have a good understanding of a data structure, one ideally needs an algebraic characterization of it. This book characterizes data structures by implementing them, for instance, the model 1 search tree.
A characterization by implementation means that as long as the order of operations on the ADT represented by the data structure is valid (for e.g., we don't try to pop an empty stack), the implicit algebraic characteristics of the data structure should be maintained.
Nevertheless, the book does not give said algebraic characterizations, so it is good to make an attempt to characterize them.
