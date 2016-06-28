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
	SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
	SDL_Surface* loadedsurface = NULL;
	if ((loadedsurface = IMG_Load("test2.png")) == NULL) {
		std::cout << "Image could not be loaded! SDL error: " << SDL_GetError() << std::endl;
		return false;
	}
	if ((imagetexture = SDL_CreateTextureFromSurface(renderer, loadedsurface)) == NULL) {
		std::cout << "Texture could not be created! SDL error: " << SDL_GetError() << std::endl;
		return false;
	}
//	if ((imagesurface = SDL_ConvertSurface(loadedsurface, screensurface->format, 0)) == NULL) {
//		std::cout << "Image could not be optimised! SDL error: " << SDL_GetError() << std::endl;
//		return false;
//	}
	SDL_FreeSurface(loadedsurface);
	loadedsurface = NULL;
//	SDL_QueryTexture(imagetexture, &imform, &imacc, &imwidth, &imheight);
	posrect.x = (SCREEN_WIDTH - IM_WIDTH) / 2;
	posrect.y = (SCREEN_HEIGHT - IM_HEIGHT) / 2;
	posrect.w = IM_WIDTH;
	posrect.h = IM_HEIGHT;
	distribution = new std::uniform_int_distribution<int>(-9,9);
	return true;
}

/// @brief Code for action during loop
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
//	Uint32 rstart = SDL_GetTicks();
//	SDL_FillRect(screensurface, NULL, SDL_MapRGB(screensurface->format, redval, greenval, blueval));
	SDL_RenderClear(renderer);
	SDL_RenderCopy(renderer, imagetexture, NULL, &posrect);
	SDL_RenderPresent(renderer);
//	SDL_BlitSurface(imagesurface, NULL, screensurface, &posrect);
//	SDL_UpdateWindowSurface(window);
//	std::cout << "Ticks spent rendering: " << SDL_GetTicks() - rstart << std::endl;
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
	SDL_DestroyTexture(imagetexture);
	imagetexture = NULL;
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	renderer = NULL;
	window = NULL;
	IMG_Quit();
	SDL_Quit();
	delete distribution;
	assert(running == false);
	std::cout << "Did not segfault (this time)." << std::endl;
}
