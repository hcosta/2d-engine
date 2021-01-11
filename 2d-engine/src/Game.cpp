#include "Game.h"
#include <iostream>
#include <SDL.h>
#include <SDL_image.h>

Game::Game() {
	std::cout << "Game constructor called\n";
}

Game::~Game() {
	std::cout << "Game destructor called\n";
}

void Game::Initialize() {
	if (SDL_Init(SDL_INIT_EVERYTHING)!=0) {
		std::cerr << "Error initializing SDL" << std::endl;
		return;
	}

	if (isFullscreen) {
		SDL_DisplayMode displayMode;
		SDL_GetCurrentDisplayMode(0, &displayMode);
		if (!isFakeFullscreen) {
			windowWidth = displayMode.w;
			windowHeight = displayMode.h;
			windowMode = SDL_WINDOW_BORDERLESS;
		}
	}

	window = SDL_CreateWindow(
		NULL, 
		SDL_WINDOWPOS_CENTERED, 
		SDL_WINDOWPOS_CENTERED,
		windowWidth,
		windowHeight,
		windowMode
	);

	if (!window) { // window == NULL
		std::cerr << "Error creating SDL window" << std::endl;
		return;
	}

	renderer = SDL_CreateRenderer(window, -1, 0); // -1 => default

	if (!renderer) { // window == NULL
		std::cerr << "Error creating SDL renderer" << std::endl;
		return;
	}

	if (isFullscreen) {
		SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN);
	}

	isRunning = true;
}

void Game::Run() {
	while(isRunning) {
		ProcessInput();
		Update();
		Render();
	}
}

void Game::Setup()
{
}

void Game::ProcessInput() {
	SDL_Event sdlEvent;
	while (SDL_PollEvent(&sdlEvent)) {
		switch (sdlEvent.type) {
			case SDL_QUIT:
				isRunning = false;
				break;
			case SDL_KEYDOWN:
				if (sdlEvent.key.keysym.sym == SDLK_ESCAPE) {
					isRunning = false;
				};
				break;
		}
	}
}

void Game::Update()
{
}

void Game::Render()
{
	SDL_SetRenderDrawColor(renderer, 21, 21, 21, 255);
	SDL_RenderClear(renderer);

	// Draw a primitive
	//SDL_Rect player = {10, 10, 20, 20};
	//SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	//SDL_RenderFillRect(renderer, &player);

	// Draw a PNG texture
	SDL_Surface *surface= IMG_Load("./assets/images/tank-tiger-right.png");
	SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
	SDL_FreeSurface(surface);

	// Copy the full texture (or a portion) to destination renderer window
	SDL_Rect dstRect = {10, 10, 32, 32};
	SDL_RenderCopy(renderer, texture, NULL, &dstRect);
	SDL_DestroyTexture(texture);

	SDL_RenderPresent(renderer);
}

void Game::Destroy()
{
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}
