.PHONY : all clean 

CFLAGS = -c -g
CC = gcc

objects = \
		  main.o \
		  binary_tree.o \
		  stack.o \
		  avl_tree.o \
		  rb_tree.o

all : $(objects)
	$(CC) -o test $(objects)


main.o: main.c
	$(CC) $(CFLAGS) $^

stack.o: stack.c
	$(CC) $(CFLAGS) $^

binary_tree.o: binary_tree.c
	$(CC) $(CFLAGS) $^

avl_tree.o: avl_tree.c
	$(CC) $(CFLAGS) $^

rb_tree.o: rb_tree.c
	$(CC) $(CFLAGS) $^

clean:
	-rm $(objects)
	-rm test
