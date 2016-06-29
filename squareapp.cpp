#include "squareapp.h"
#include <iostream>
#include <cassert>

/// @brief Constructor - currently only sets running to true
SquareApp::SquareApp() {
	running = true;
}

/// @brief Execute the program, start to finish
///
/// @return Exit status
int SquareApp::OnExecute() {
	if (OnInit() == false)
		return -1;

	SDL_Event event;

	while (running) {
		Uint32 lstart = SDL_GetTicks();
		while (SDL_PollEvent(&event))
			OnEvent(&event);
		OnLoop();
		OnRender();
		Uint32 lend = SDL_GetTicks();
		if ((lend - lstart) < LOOP_TIME)
			SDL_Delay(LOOP_TIME - (lend - lstart));
	}

	OnCleanup();

	return 0;
}

/// @brief Initialise window
///
/// @return success or fail
bool SquareApp::OnInit() {
	currentPosition = Board();
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		std::cout << "Error in initialising! SDL error: " << SDL_GetError() << std::endl;
		return false;
	}
	int imgFlags = IMG_INIT_PNG;
	if (!(IMG_Init(imgFlags) & imgFlags)) {
		std::cout << "SDL image library could not be initialised! SDL_Image error: " << IMG_GetError() << std::endl;
		return false;
	}
	window = SDL_CreateWindow("SDL test", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
	if (window == NULL) {
		std::cout << "Window could not be created! SDL error: " << SDL_GetError() << std::endl;
		return false;
	}
	screensurface = SDL_GetWindowSurface(window);
	if ((renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED)) == NULL) {
		std::cout << "Renderer could not be created! SDL error: " << SDL_GetError() << std::endl;
		return false;
	}
	xDistribution = std::uniform_int_distribution<int>(0, SQUARES_X);
	yDistribution = std::uniform_int_distribution<int>(0, SQUARES_Y);
	horverDistribution = std::uniform_int_distribution<int>(0, 1);
	return true;
}

/// @brief Code for action during loop
void SquareApp::OnLoop() {
	Move mv = proposeMove();
	std::cout << (int)currentPosition.CurrentPlayer() << ": " << (int)mv.orientation << "; " << mv.x << "; " << mv.y << std::endl;
	currentPosition = currentPosition.moveResult(mv);
	std::cout << (int)currentPosition.CurrentPlayer() << std::endl;
	std::cout << "Score: " << currentPosition.score(Player::ONE) << " / " << currentPosition.score(Player::TWO) << " (" << currentPosition.score(Player::NONE) << ")" << std::endl;
}

/// @brief Renderer
void SquareApp::OnRender() {
	SDL_SetRenderDrawColor(renderer, redval, greenval, blueval, 0xFF);
	SDL_RenderClear(renderer);
	SDL_RenderPresent(renderer);
}

/// @brief Handle events
///
/// @param Event Pointer to event
void SquareApp::OnEvent(SDL_Event* Event) {
	switch (Event->type) {
		case SDL_QUIT:
			std::cout << "Quitting!" << std::endl;
			running = false;
			break;
		case 1024:
			break;
		case SDL_KEYDOWN:
			KeyPress(Event->key.keysym);
			break;
		default:
			std::cout << Event->type << std::endl;
			break;
	}
}

void SquareApp::KeyPress(SDL_Keysym keyp) {
	switch (keyp.sym) {
		case SDLK_b:
			if (keyp.mod & KMOD_SHIFT)
				blueval = 0xFF;
			else
				blueval = 0;
			break;
		case SDLK_r:
			if (keyp.mod & KMOD_SHIFT)
				redval = 0xFF;
			else
				redval = 0;
			break;
		case SDLK_g:
			if (keyp.mod & KMOD_SHIFT)
				greenval = 0xFF;
			else
				greenval = 0;
			break;
		case SDLK_q:
			running = false;
			break;
		default:
			break;
	}
}

/// @brief Close window etc
void SquareApp::OnCleanup() {
	std::cout << "Quitting (nb may segfault)." << std::endl;
//	SDL_FreeSurface(imagesurface);
//	imagesurface = NULL;
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	renderer = NULL;
	window = NULL;
	IMG_Quit();
	SDL_Quit();
	assert(running == false);
	std::cout << "Did not segfault (this time)." << std::endl;
}

Move SquareApp::proposeMove() {
	Move mv = {(horverDistribution(generator) ? Orientation::HORIZONAL : Orientation::VERTICAL), xDistribution(generator), yDistribution(generator)};
	return mv;
}
