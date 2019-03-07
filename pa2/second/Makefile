all: second

second: second.c
	gcc -g -Wall -Werror -fsanitize=address second.c -o second

clean:
	rm -rf second
