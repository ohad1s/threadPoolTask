.PHONY: all
all: task

task:	 main.c codec.h
		gcc main.c -o main -L. ./libCodec.so -pthread

.PHONY: clean
clean:
	-rm main 2>/dev/null
