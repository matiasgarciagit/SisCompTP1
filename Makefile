CFLAGS=-Wall -pedantic -Werror -pthread -Wextra -Wconversion -std=gnu11

main: main.c
	gcc $(CFLAGS) -o main main.c

clean:
	rm -f main socket srv *.o *.a *.so