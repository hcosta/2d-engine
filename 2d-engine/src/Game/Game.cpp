#include "Game.h"
#include <SDL.h>
#include <SDL_image.h>
#include <glm/glm.hpp>
#include <spdlog/spdlog.h>
#include <iostream>

Game::Game() {
	isRunning = false;
	spdlog::info("Game constructor called");
}

Game::~Game() {
	spdlog::info("Game destructor called");
}

void Game::Initialize() {
	if (SDL_Init(SDL_INIT_EVERYTHING)!=0) {
		spdlog::error("Error initializing SDL");
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
		spdlog::error("Error creating SDL window");
		return;
	}

	renderer = SDL_CreateRenderer(window, -1, 0); // -1 => default

	if (!renderer) { // window == NULL
		spdlog::error("Error creating SDL renderer");
		return;
	}

	if (isFullscreen) {
		SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN);
	}

	isRunning = true;
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

glm::vec2 playerPosition;
glm::vec2 playerVelocity;

void Game::Setup(){
	playerPosition = glm::vec2(10.0, 20.0);
	playerVelocity = glm::vec2(100.0, 0.0);

	fpsPrevious = SDL_GetTicks();
}

void Game::Update(){
	// If we are too fast, waste time until reach MILLISECS_PER_FRAME
	int timeToWait = MILLISECS_PER_FRAME - (SDL_GetTicks() - millisecsPreviousFrame);
	if (timeToWait > 0 && timeToWait <= MILLISECS_PER_FRAME) {
		SDL_Delay(timeToWait);
	}

	// Difference in ticks since last frame in seconds
	double deltaTime = (SDL_GetTicks() - millisecsPreviousFrame) / 1000.0;

	// Store current frame time
	millisecsPreviousFrame = SDL_GetTicks();

	playerPosition.x += playerVelocity.x * deltaTime;
	playerPosition.y += playerVelocity.y * deltaTime;

	// FPS counter
	fpsCounter++;
	if (fpsPrevious < SDL_GetTicks() - 1000)
	{
		fpsPrevious = SDL_GetTicks();
		fpsCurrent = fpsCounter;
		fpsCounter = 0;
	}

}

void Game::Render(){
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
	SDL_Rect dstRect = { 
		static_cast<int>(playerPosition.x), 
		static_cast<int>(playerPosition.y),
		32, 
		32
	};

	SDL_RenderCopy(renderer, texture, NULL, &dstRect);
	SDL_DestroyTexture(texture);

	SDL_RenderPresent(renderer);
}

void Game::Run() {
	Setup();
	while (isRunning) {
		ProcessInput();
		Update();
		Render();
	}
}

void Game::Destroy(){
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}
