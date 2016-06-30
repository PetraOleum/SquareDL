CC=g++
LIBS=-lSDL2 -lSDL2_gfx
CFLAGS=-Wall -Wextra -Werror -std=c++11 -Og
DEPS=squareapp.h board.h ai.h
OBJ=main.o squareapp.o board.o ai.o

all: squaredl


%.o: %.cpp $(DEPS)
	$(CC) $(CFLAGS) -c -o $@ $< 

squaredl: $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS)

.PHONY: clean

clean:
	-rm -f *.o
	-rm -f squaredl
