all: third

third: third.c
	gcc -g -Wall -Werror -fsanitize=address third.c -o third

clean:
	rm -rf third
