CC = gcc
CFLAGS = -Wall -Wextra -Wpedantic -std=c11 -g
LDFLAGS = -lrt -lpthread

GEN_EXE = generator
SUP_EXE = supervisor

SHARED_SRCS = graphs.c circ_buf.c
SHARED_OBJS = $(SHARED_SRCS:.c=.o)

GEN_SRCS = generator.c
SUP_SRCS = supervisor.c

GEN_OBJS = $(GEN_SRCS:.c=.o) $(SHARED_OBJS)
SUP_OBJS = $(SUP_SRCS:.c=.o) $(SHARED_OBJS)


all: $(GEN_EXE) $(SUP_EXE)


$(GEN_EXE): $(GEN_OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)


$(SUP_EXE): $(SUP_OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)


%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@


clean:
	rm -f *.o $(GEN_EXE) $(SUP_EXE)


.PHONY: all clean
