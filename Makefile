CC := gcc
CFLAGS := -Wall -Werror

csrc = $(wildcard src/*.c) \
	   $(wildcard src/thread_pool/*.c)

obj = $(csrc: .c = .o) 

main: $(obj)
	$(CC) $(CFLAGS) -o $@ $^

clean: 
	rm -rf *o main