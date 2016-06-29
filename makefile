CC=g++
LIBS=-lSDL2 -lSDL2_image
CFLAGS=-Wall -Wextra -Werror -std=c++11 -g
DEPS=squareapp.h board.h
OBJ=main.o squareapp.o board.o

all: squaredl


%.o: %.cpp $(DEPS)
	$(CC) $(CFLAGS) -c -o $@ $< 

squaredl: $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS)

.PHONY: clean

clean:
	-rm -f *.o
	-rm -f squaredl
