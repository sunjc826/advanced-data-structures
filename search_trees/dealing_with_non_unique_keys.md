Find and insert are straightforward as described in the text.
The only slightly confusing part is with regard to deletion.

Aim: To delete all items of a particular key in time independent of the number of such items.

The linked list in discussion is a singly-linked list (SLL). We may not need a doubly linked list since it requires additional space.

Consider what happens when we delete the SLL $a_1\rightarrow a_2 \rightarrow a_3$ from a tree. Note that the leaf node $v$ has a pointer to $a_1$, the start of the SLL. We can remove the reference by setting `v = NULL`. However, consider what happens when we attempt to move the SLL starting at $a_1$ to the [dynamic memory allocation structure](../elementary_data_structures/node_management.c), `return_list_node(a_1)`.
The dynamic memory allocation structure contains a SLL of free nodes. To connect $a_1\rightarrow a_2 \rightarrow a_3$ to these free nodes, we either prepend it, which requires setting `a_3.next` to the head of `free_nodes`, or we can append it, setting the tail of free_nodes `xxx.next = a_1`.

Assuming we are prepending, we need a pointer to $a_3$ as well, to avoid traversing the SLL in O(number of items removed) time. 
