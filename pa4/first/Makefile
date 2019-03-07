all: first

first: first.c
	gcc -lm -Wall -Werror -fsanitize=address first.c -o first

clean:
	rm -rf first
