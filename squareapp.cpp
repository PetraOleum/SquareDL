#include "squareapp.h"
#include <iostream>
#include <cassert>
#include <ctime>

/// @brief Constructor - currently only sets running to true
SquareApp::SquareApp() {
	generator = std::mt19937(rd());
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
	std::cout << "Score: " << currentPosition.score(Player::ONE) << " / " << currentPosition.score(Player::TWO) << " (" << currentPosition.score(Player::NONE) << ")" << std::endl;
	std::cout << "Turn: " << currentPosition.turns() << std::endl;
}

/// @brief Renderer
void SquareApp::OnRender() {
	SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xFF);
	SDL_RenderClear(renderer);
	for (int x = 0; x < SQUARES_X; x++)
		for (int y = 0; y < SQUARES_Y; y++) {
			SDL_Rect squareRect = {x * LINE_WIDTH, y * LINE_WIDTH, LINE_WIDTH, LINE_WIDTH};
			Player op = currentPosition.squareState(x, y);
			switch (op) {
				case Player::ONE:
					SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0xFF, 0xFF);
					break;
				case Player::TWO:
					SDL_SetRenderDrawColor(renderer, 0xFF, 0x00, 0x00, 0xFF);
					break;
				default:
					SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xFF);
			}
			SDL_RenderFillRect(renderer, &squareRect);
		}
	for (int x = 0; x < SQUARES_X; x++)
		for (int y = 0; y <= SQUARES_Y; y++) {
				SDL_Rect lrect = {x * LINE_WIDTH + LINE_HEIGHT / 2, y * LINE_WIDTH - LINE_HEIGHT / 2, LINE_WIDTH - LINE_HEIGHT, LINE_HEIGHT};
				if (currentPosition.horLineState(x, y)) {
					SDL_SetRenderDrawColor(renderer, 0xFF, 0x00, 0xFF, 0xFF);
					SDL_RenderFillRect(renderer, &lrect);
					filledTrigonRGBA(renderer, x * LINE_WIDTH, y * LINE_WIDTH, x * LINE_WIDTH + LINE_HEIGHT / 2, y * LINE_WIDTH + LINE_HEIGHT / 2, x * LINE_WIDTH + LINE_HEIGHT / 2, y * LINE_WIDTH - LINE_HEIGHT / 2, 0xFF, 0x00, 0xFF, 0xFF);
					filledTrigonRGBA(renderer, (x + 1) * LINE_WIDTH, y * LINE_WIDTH, (x + 1) * LINE_WIDTH - LINE_HEIGHT / 2, y * LINE_WIDTH + LINE_HEIGHT / 2, (x + 1) * LINE_WIDTH - LINE_HEIGHT / 2, y * LINE_WIDTH - LINE_HEIGHT / 2, 0xFF, 0x00, 0xFF, 0xFF);
				}
				else {
					SDL_SetRenderDrawColor(renderer, 0x30, 0x30, 0x30, 0xFF);
					SDL_RenderFillRect(renderer, &lrect);
					filledTrigonRGBA(renderer, x * LINE_WIDTH, y * LINE_WIDTH, x * LINE_WIDTH + LINE_HEIGHT / 2, y * LINE_WIDTH + LINE_HEIGHT / 2, x * LINE_WIDTH + LINE_HEIGHT / 2, y * LINE_WIDTH - LINE_HEIGHT / 2, 0x30, 0x30, 0x30, 0xFF);
					filledTrigonRGBA(renderer, (x + 1) * LINE_WIDTH, y * LINE_WIDTH, (x + 1) * LINE_WIDTH - LINE_HEIGHT / 2, y * LINE_WIDTH + LINE_HEIGHT / 2, (x + 1) * LINE_WIDTH - LINE_HEIGHT / 2, y * LINE_WIDTH - LINE_HEIGHT / 2, 0x30, 0x30, 0x30, 0xFF);
				}
			}
	for (int x = 0; x <= SQUARES_X; x++)
		for (int y = 0; y < SQUARES_Y; y++) {
				SDL_Rect lrect = {x * LINE_WIDTH - LINE_HEIGHT / 2, y * LINE_WIDTH + LINE_HEIGHT / 2, LINE_HEIGHT, LINE_WIDTH - LINE_HEIGHT};
				if (currentPosition.verLineState(x, y)) {
					SDL_SetRenderDrawColor(renderer, 0xFF, 0x00, 0xFF, 0xFF);
					SDL_RenderFillRect(renderer, &lrect);
					filledTrigonRGBA(renderer, x * LINE_WIDTH, y * LINE_WIDTH, x * LINE_WIDTH + LINE_HEIGHT / 2, y * LINE_WIDTH + LINE_HEIGHT / 2, x * LINE_WIDTH - LINE_HEIGHT / 2, y * LINE_WIDTH + LINE_HEIGHT / 2, 0xFF, 0x00, 0xFF, 0xFF);
					filledTrigonRGBA(renderer, x * LINE_WIDTH, (y + 1) * LINE_WIDTH, x * LINE_WIDTH + LINE_HEIGHT / 2, (y + 1) * LINE_WIDTH - LINE_HEIGHT / 2, x * LINE_WIDTH - LINE_HEIGHT / 2, (y + 1) * LINE_WIDTH - LINE_HEIGHT / 2, 0xFF, 0x00, 0xFF, 0xFF);
				}
				else {
					SDL_SetRenderDrawColor(renderer, 0x30, 0x30, 0x30, 0xFF);
					SDL_RenderFillRect(renderer, &lrect);
					filledTrigonRGBA(renderer, x * LINE_WIDTH, y * LINE_WIDTH, x * LINE_WIDTH + LINE_HEIGHT / 2, y * LINE_WIDTH + LINE_HEIGHT / 2, x * LINE_WIDTH - LINE_HEIGHT / 2, y * LINE_WIDTH + LINE_HEIGHT / 2, 0x30, 0x30, 0x30, 0xFF);
					filledTrigonRGBA(renderer, x * LINE_WIDTH, (y + 1) * LINE_WIDTH, x * LINE_WIDTH + LINE_HEIGHT / 2, (y + 1) * LINE_WIDTH - LINE_HEIGHT / 2, x * LINE_WIDTH - LINE_HEIGHT / 2, (y + 1) * LINE_WIDTH - LINE_HEIGHT / 2, 0x30, 0x30, 0x30, 0xFF);
				}
			}
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
	SDL_Quit();
	assert(running == false);
	std::cout << "Did not segfault (this time)." << std::endl;
}

Move SquareApp::proposeMove() {
	Player curpla = currentPosition.CurrentPlayer();
	int cscore = currentPosition.score(curpla);
	Move mv;
	for (int x = 0; x < SQUARES_X; x++)
		for (int y = 0; y <= SQUARES_Y; y++) {
			mv = {Orientation::HORIZONAL, x, y,};
			if (currentPosition.moveResult(mv).score(curpla) > cscore)
				return mv;
		}
	for (int x = 0; x <= SQUARES_X; x++)
		for (int y = 0; y < SQUARES_Y; y++) {
			mv = {Orientation::VERTICAL, x, y};
			if (currentPosition.moveResult(mv).score(curpla) > cscore)
				return mv;
		}
	mv = {(horverDistribution(generator) ? Orientation::HORIZONAL : Orientation::VERTICAL), xDistribution(generator), yDistribution(generator)};
	return mv;
}
