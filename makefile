CC=gcc
CFLAGS=-Wall -g
LDFLAGS=-lR -lm
MEMFLAGS=-fsanitize=address -static-libasan
EXEC=molecular3d
SRC= $(wildcard *.c)
OBJ= $(SRC:.c=.o)

all: $(EXEC)

molecular3d: $(OBJ)
	$(CC) -o $@ $^ $(LDFLAGS)

%.o: %.c
	$(CC) -o $@ -c $< $(CFLAGS)

.PHONY: clean mrproper all mem

clean:
	rm -rf *.o

mrproper: clean
	rm -rf $(EXEC) Results
