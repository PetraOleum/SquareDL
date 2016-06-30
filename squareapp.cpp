#include "squareapp.h"
#include <iostream>
#include <cassert>
#include <ctime>
#include <algorithm>
#include "ai.h"

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
	window = SDL_CreateWindow("SDL test", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
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
	while (currentPosition.CurrentPlayer() == Player::TWO && currentPosition.score(Player::NONE) > 0) {
		Move mv = proposeMove();
		std::cout << (int)currentPosition.CurrentPlayer() << ": " << (int)mv.orientation << "; " << mv.x << "; " << mv.y << std::endl;
		currentPosition = currentPosition.moveResult(mv);
		std::cout << "Score: " << currentPosition.score(Player::ONE) << " / " << currentPosition.score(Player::TWO) << " (" << currentPosition.score(Player::NONE) << ")" << std::endl;
		std::cout << "Turn: " << currentPosition.turns() << std::endl;
	}
}

/// @brief Renderer
void SquareApp::OnRender() {
	SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xFF);
	SDL_RenderClear(renderer);
	for (int x = 0; x < SQUARES_X; x++)
		for (int y = 0; y < SQUARES_Y; y++) {
			SDL_Rect squareRect = {x * LINE_WIDTH + xOffset, y * LINE_WIDTH + yOffset, LINE_WIDTH, LINE_WIDTH};
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
				SDL_Rect lrect = {
					x * LINE_WIDTH + LINE_HEIGHT / 2 + xOffset, 
					y * LINE_WIDTH - LINE_HEIGHT / 2 + yOffset, 
					LINE_WIDTH - LINE_HEIGHT, 
					LINE_HEIGHT
				};
				if (currentPosition.horLineState(x, y)) {
					SDL_SetRenderDrawColor(renderer, 0xFF, 0x00, 0xFF, 0xFF);
					SDL_RenderFillRect(renderer, &lrect);
					filledTrigonRGBA(renderer, x * LINE_WIDTH + xOffset, 
							y * LINE_WIDTH + yOffset, 
							x * LINE_WIDTH + LINE_HEIGHT / 2 + xOffset, 
							y * LINE_WIDTH + LINE_HEIGHT / 2 + yOffset, 
							x * LINE_WIDTH + LINE_HEIGHT / 2 + xOffset, 
							y * LINE_WIDTH - LINE_HEIGHT / 2 + yOffset, 
							0xFF, 0x00, 0xFF, 0xFF);

					filledTrigonRGBA(renderer, (x + 1) * LINE_WIDTH + xOffset, 
							y * LINE_WIDTH + yOffset, 
							(x + 1) * LINE_WIDTH - LINE_HEIGHT / 2 + xOffset, 
							y * LINE_WIDTH + LINE_HEIGHT / 2 + yOffset, 
							(x + 1) * LINE_WIDTH - LINE_HEIGHT / 2 + xOffset, 
							y * LINE_WIDTH - LINE_HEIGHT / 2 + yOffset, 
							0xFF, 0x00, 0xFF, 0xFF);
				}
				else {
					SDL_SetRenderDrawColor(renderer, 0x30, 0x30, 0x30, 0xFF);
					SDL_RenderFillRect(renderer, &lrect);
					filledTrigonRGBA(renderer, x * LINE_WIDTH + xOffset, 
							y * LINE_WIDTH + yOffset, 
							x * LINE_WIDTH + LINE_HEIGHT / 2 + xOffset, 
							y * LINE_WIDTH + LINE_HEIGHT / 2 + yOffset, 
							x * LINE_WIDTH + LINE_HEIGHT / 2 + xOffset, 
							y * LINE_WIDTH - LINE_HEIGHT / 2 + yOffset, 
							0x30, 0x30, 0x30, 0xFF);

					filledTrigonRGBA(renderer, (x + 1) * LINE_WIDTH + xOffset, 
							y * LINE_WIDTH + yOffset, 
							(x + 1) * LINE_WIDTH - LINE_HEIGHT / 2 + xOffset, 
							y * LINE_WIDTH + LINE_HEIGHT / 2 + yOffset, 
							(x + 1) * LINE_WIDTH - LINE_HEIGHT / 2 + xOffset, 
							y * LINE_WIDTH - LINE_HEIGHT / 2 + yOffset, 
							0x30, 0x30, 0x30, 0xFF);
				}
			}
	for (int x = 0; x <= SQUARES_X; x++)
		for (int y = 0; y < SQUARES_Y; y++) {
				SDL_Rect lrect = {
					x * LINE_WIDTH - LINE_HEIGHT / 2 + xOffset, 
					y * LINE_WIDTH + LINE_HEIGHT / 2 + yOffset, 
					LINE_HEIGHT, 
					LINE_WIDTH - LINE_HEIGHT
				};
				if (currentPosition.verLineState(x, y)) {
					SDL_SetRenderDrawColor(renderer, 0xFF, 0x00, 0xFF, 0xFF);
					SDL_RenderFillRect(renderer, &lrect);
					filledTrigonRGBA(renderer, x * LINE_WIDTH + xOffset, 
							y * LINE_WIDTH + yOffset, 
							x * LINE_WIDTH + LINE_HEIGHT / 2 + xOffset, 
							y * LINE_WIDTH + LINE_HEIGHT / 2 + yOffset, 
							x * LINE_WIDTH - LINE_HEIGHT / 2 + xOffset, 
							y * LINE_WIDTH + LINE_HEIGHT / 2 + yOffset, 
							0xFF, 0x00, 0xFF, 0xFF);

					filledTrigonRGBA(renderer, x * LINE_WIDTH + xOffset, 
							(y + 1) * LINE_WIDTH + yOffset, 
							x * LINE_WIDTH + LINE_HEIGHT / 2 + xOffset, 
							(y + 1) * LINE_WIDTH - LINE_HEIGHT / 2 + yOffset, 
							x * LINE_WIDTH - LINE_HEIGHT / 2 + xOffset, 
							(y + 1) * LINE_WIDTH - LINE_HEIGHT / 2 + yOffset, 
							0xFF, 0x00, 0xFF, 0xFF);
				}
				else {
					SDL_SetRenderDrawColor(renderer, 0x30, 0x30, 0x30, 0xFF);
					SDL_RenderFillRect(renderer, &lrect);
					filledTrigonRGBA(renderer, x * LINE_WIDTH + xOffset, 
							y * LINE_WIDTH + yOffset, 
							x * LINE_WIDTH + LINE_HEIGHT / 2 + xOffset, 
							y * LINE_WIDTH + LINE_HEIGHT / 2 + yOffset, 
							x * LINE_WIDTH - LINE_HEIGHT / 2 + xOffset, 
							y * LINE_WIDTH + LINE_HEIGHT / 2 + yOffset, 
							0x30, 0x30, 0x30, 0xFF);

					filledTrigonRGBA(renderer, x * LINE_WIDTH + xOffset, 
							(y + 1) * LINE_WIDTH + yOffset, 
							x * LINE_WIDTH + LINE_HEIGHT / 2 + xOffset, 
							(y + 1) * LINE_WIDTH - LINE_HEIGHT / 2 + yOffset, 
							x * LINE_WIDTH - LINE_HEIGHT / 2 + xOffset, 
							(y + 1) * LINE_WIDTH - LINE_HEIGHT / 2 + yOffset, 
							0x30, 0x30, 0x30, 0xFF);
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
		case SDL_KEYDOWN:
			KeyPress(Event->key.keysym);
			break;
		case SDL_MOUSEBUTTONDOWN:
			mouseDown = true;
			mdsx = Event->button.x;
			mdsy = Event->button.y;
			break;
		case SDL_MOUSEMOTION:
			if (mouseDown) {
				mouseDragging = true;
				xOffset += (Event->motion.x - mdsx);
				yOffset += (Event->motion.y - mdsy);
				mdsx = Event->motion.x;
				mdsy = Event->motion.y;
			}
			else {
				mouseDragging = false;
			}
			break;
		case SDL_MOUSEBUTTONUP:
			mouseDown = false;
			if (currentPosition.CurrentPlayer() == Player::ONE && mouseDragging == false) {
				Move mv = humanMove(Event);
				std::cout << "Move: " << mv.x << "; " << mv.y << std::endl;
				currentPosition = currentPosition.moveResult(mv);
			}
			mouseDragging = false;
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
		case SDLK_r:
			currentPosition = Board();
			break;
		case SDLK_F11:
			if (fullscreen)
				SDL_SetWindowFullscreen(window, 0);
			else
				SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN_DESKTOP);
			fullscreen = !fullscreen;
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
//	Player curpla = currentPosition.CurrentPlayer();
//	int cscore = currentPosition.score(curpla);
//	Move mv;
//	for (int x = 0; x < SQUARES_X; x++)
//		for (int y = 0; y <= SQUARES_Y; y++) {
//			mv = {Orientation::HORIZONAL, x, y,};
//			if (currentPosition.moveResult(mv).score(curpla) > cscore)
//				return mv;
//		}
//	for (int x = 0; x <= SQUARES_X; x++)
//		for (int y = 0; y < SQUARES_Y; y++) {
//			mv = {Orientation::VERTICAL, x, y};
//			if (currentPosition.moveResult(mv).score(curpla) > cscore)
//				return mv;
//		}
//	mv = {(horverDistribution(generator) ? Orientation::HORIZONAL : Orientation::VERTICAL), xDistribution(generator), yDistribution(generator)};
//	return mv;
	return calculateBestMove(currentPosition, 1).move;
}

Move SquareApp::humanMove(SDL_Event* e) {
	std::cout << "Clicked" << std::endl;
	int dx = (e->button.x - xOffset) % LINE_WIDTH;
	int dy = (e->button.y - yOffset) % LINE_WIDTH;
//	if ((dy > LINE_HEIGHT / 2 && dy < LINE_WIDTH - LINE_HEIGHT / 2) || (dx > LINE_HEIGHT / 2 && dx < LINE_WIDTH - LINE_HEIGHT / 2))
//		return {Orientation::HORIZONAL, -1, -1};
	int dix = LINE_WIDTH - dx;
	int diy = LINE_WIDTH - dy;
	int min = std::min(std::min(std::min(dx, dy), dix), diy);
	int sx = (e->button.x - xOffset) / LINE_WIDTH;
	int sy = (e->button.y - yOffset) / LINE_WIDTH;
	std::cout << e->button.x << "; " << dx << "; " << dix << "; " << sx << std::endl;
	std::cout << min << std::endl;
	if (min == dx)
		return {Orientation::VERTICAL, sx, sy};
	else if (min == dy)
		return {Orientation::HORIZONAL, sx, sy};
	else if (min == dix)
		return {Orientation::VERTICAL, sx + 1, sy};
	else
		return {Orientation::HORIZONAL, sx, sy + 1};


}
