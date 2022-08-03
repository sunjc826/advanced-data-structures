.PHONY: test_stack_v2
test_stack_v2: stack_v2.out
	./stack_v2.out

.PHONY: test_stack_v3
test_stack_v3: stack_v3.out
	./stack_v3.out

.PHONY: test_stack_v4
test_stack_v4: stack_v4.out
	./stack_v4.out

%.out: %.c
	cc -o $@ $^ -Wall $(CCFLAGS)

.PHONY: clean
clean:
	rm *.out