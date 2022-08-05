.PHONY: test_stack_v2
test_stack_v2: stack_v2.out
	./stack_v2.out

.PHONY: test_stack_v3
test_stack_v3: stack_v3.out
	./stack_v3.out

.PHONY: test_stack_v4
test_stack_v4: stack_v4.out
	./stack_v4.out

.PHONY: test_queue_v1
test_queue_v1: queue_v1.out
	./queue_v1.out

.PHONY: test_queue_v2
test_queue_v2: queue_v2.out
	./queue_v2.out

.PHONY: test_queue_v3
test_queue_v3: queue_v3.out
	./queue_v3.out

.PHONY: test_queue_v4
test_queue_v4: queue_v4.out
	./queue_v4.out

.PHONY: test_queue_v5
test_queue_v5: queue_v5.out
	./queue_v5.out

queue_v5.out: memory_allocator.h memory_allocator.c

.PHONY: test_search_tree_v1
test_search_tree_v1: search_tree_v1.out
	./search_tree_v1.out

search_tree_v1.out: stack.h

%.out: %.c common.h
	cc -o $@ $^ -Wall $(CCFLAGS)

.PHONY: clean
clean:
	rm *.out


