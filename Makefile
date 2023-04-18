.PHONY: all
all: task stdinExample my_code


task:	codec.h basic_main.c
	gcc basic_main.c -L. -l Codec -o encoder

stdinExample:	stdin_main.c
		gcc stdin_main.c -L. -l Codec -o tester

my_code:	my_stdin.c
		gcc my_stdin.c -L. -l Codec -o myStdin

.PHONY: clean
clean:
	-rm encoder tester
