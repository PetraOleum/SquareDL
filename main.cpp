#include "squareapp.h"
#include <iostream>

/// @brief main function: enumerate args, launch app (closes before control returns
///
/// @param argc arg count
/// @param argv[] args
///
/// @return execution status
int main( int argc, char* argv[]) {
	SquareApp theApp;

	std::cout << "Args:" << std::endl;

	for (int i = 0; i < argc; i++) 
		std::cout << i << ": " << argv[i] << std::endl;

	return theApp.OnExecute();
}
