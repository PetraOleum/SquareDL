#include "squareapp.h"
#include <iostream>

int main( int argc, char* argv[]) {
	SquareApp theApp;

	std::cout << "Args:" << std::endl;

	for (int i = 0; i < argc; i++) 
		std::cout << i << ": " << argv[i] << std::endl;

	return theApp.OnExecute();
}
