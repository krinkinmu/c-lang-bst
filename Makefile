deafult: avl rb

avl: main_avl.o avl.o
	gcc $^ -o $@

rb: main_rb.o rb.o
	gcc $^ -o $@

%.o: %.c
	gcc $< -c -o $@

.PHONY: clean
clean:
	rm -rf *.o avl rb
