all: second

second: second.c
	gcc -Wall -Werror -lm -fsanitize=address second.c -o second

clean:
	rm -rf second
