CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -fsanitize=address
LDFLAGS = -lSDL2 -lSDL2_image -lm -lSDL2_ttf

SRC = main.c
EXEC = main

all: $(EXEC)

$(EXEC): $(SRC)
	$(CC) $(SRC) -o $(EXEC) $(CFLAGS) $(LDFLAGS)

run: all
	./$(EXEC)

clean:
	rm -f $(EXEC)
