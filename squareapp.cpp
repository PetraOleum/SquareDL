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
		while (SDL_PollEvent(&event))
			OnEvent(&event);
		OnLoop();
		OnRender();
		SDL_Delay(200);
	}

	OnCleanup();

	return 0;
}

/// @brief Initialise window
///
/// @return success or fail
bool SquareApp::OnInit() {
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		std::cout << "Error in initialising! SDL error " << SDL_GetError() << std::endl;
		return false;
	}
	window = SDL_CreateWindow("SDL test", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
	if (window == NULL) {
		std::cout << "Window could not be created! SDL error " << SDL_GetError() << std::endl;
		return false;
	}
	screensurface = SDL_GetWindowSurface(window);
	SDL_Surface* loadedsurface = NULL;
	if ((loadedsurface = SDL_LoadBMP("test.bmp")) == NULL) {
		std::cout << "Image could not be loaded! SDL error " << SDL_GetError() << std::endl;
		return false;
	}
	if ((imagesurface = SDL_ConvertSurface(loadedsurface, screensurface->format, 0)) == NULL) {
			std::cout << "Image could not be optimised! SDL error " << SDL_GetError() << std::endl;
	}
	posrect.x = (SCREEN_WIDTH - imagesurface->w) / 2;
	posrect.y = (SCREEN_HEIGHT - imagesurface->h) / 2;
	posrect.w = imagesurface->w;
	posrect.h = imagesurface->h;
	distribution = new std::uniform_int_distribution<int>(-9,9);
	return true;
}

/// @brief Code for action during loop (empty)
void SquareApp::OnLoop() {
	posrect.y += (*distribution)(generator);
	posrect.x += (*distribution)(generator);
	if (posrect.y < 0)
		posrect.y = 0;
	if (posrect.y + posrect.h > SCREEN_HEIGHT)
		posrect.y = SCREEN_HEIGHT - posrect.h;
	if (posrect.x < 0)
		posrect.x = 0;
	if (posrect.x + posrect.w > SCREEN_WIDTH)
		posrect.x = SCREEN_WIDTH - posrect.w;
}

/// @brief Renderer
void SquareApp::OnRender() {
	SDL_FillRect(screensurface, NULL, SDL_MapRGB(screensurface->format, redval, greenval, blueval));
	SDL_BlitSurface(imagesurface, NULL, screensurface, &posrect);
	SDL_UpdateWindowSurface(window);
}

/// @brief Close window etc
void SquareApp::OnCleanup() {
	SDL_FreeSurface(imagesurface);
	imagesurface = NULL;
	SDL_DestroyWindow(window);
	SDL_Quit();
	delete distribution;
	assert(running == false);
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
