CC=g++
LIBS=-lSDL2main -lSDL2
CFLAGS=-Wall -Wextra -Werror -std=c++11 -x c++ $(LIBS)
DEPS=squareapp.h
OBJ=main.o squareapp.o

all: squaredl


%.o: %.cpp $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

squaredl: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

.PHONY: clean

clean:
	-rm -f *.o
	-rm -f squaredl
