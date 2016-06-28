CC=g++
LIBS=-lSDL2main -lSDL2
CFLAGS=-Wall -Wextra -Werror -std=c++11
DEPS=squareapp.h
OBJ=main.o squareapp.o

all: squaredl


%.o: %.cpp $(DEPS)
	$(CC) $(CFLAGS) -c -o $@ $< 

squaredl: $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS)

.PHONY: clean

clean:
	-rm -f *.o
	-rm -f squaredl
