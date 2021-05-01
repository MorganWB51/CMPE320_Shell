OBJS = runner.c sush.c internal.c cmdline.c list.c env.c
CC = gcc -o

all: runner

runner: $(OBJS)
	$(CC) sush $(OBJS)

clean:
	rm sush