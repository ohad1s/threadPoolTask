.PHONY: all
all: task

task:	 main.c codec.h
		gcc main.c -o coder -L. ./libCodec.so -pthread

.PHONY: clean
clean:
	-rm coder 2>/dev/null
