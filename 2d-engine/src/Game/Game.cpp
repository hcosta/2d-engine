#include "Game.h"
#include <spdlog/spdlog.h>
#include "../ECS/ECS.h"
#include "../Components/TransformComponent.h"
#include "../Components/RigidBodyComponent.h"
#include <SDL.h>
#include <SDL_image.h>
#include <glm/glm.hpp>
#include <iostream>

Game::Game() {
	isRunning = false;
	registry = std::make_unique<Registry>();
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

void Game::Setup(){
	// Create one entity
	Entity tank = registry->CreateEntity();

	// Add some components to that entity
	tank.AddComponent<TransformComponent>(glm::vec2(10.0, 30.0), glm::vec2(1.0, 1.0), 0.0);
	tank.AddComponent<RigidBodyComponent>(glm::vec2(50.0, 0.0));

	// Remove a component from entity
	tank.RemoveComponent<TransformComponent>();
}

void Game::Update(){
	// If we are too fast, waste time until reach MILLISECS_PER_FRAME
	Uint32 timeToWait = static_cast<Uint32>(MILLISECS_PER_FRAME) - (SDL_GetTicks() - static_cast<Uint32>(millisecsPreviousFrame));
	if (timeToWait > 0 && timeToWait <= MILLISECS_PER_FRAME) {
		SDL_Delay(timeToWait);
	}

	// Difference in ticks since last frame in seconds
	double deltaTime = (SDL_GetTicks() - millisecsPreviousFrame) / 1000.0;

	// Store current frame time
	millisecsPreviousFrame = SDL_GetTicks();

	// MovementSystem.Update();
	// CollisionSystem.Update();
	// DamageSystem.Update();
}

void Game::Render(){
	SDL_SetRenderDrawColor(renderer, 21, 21, 21, 255);
	SDL_RenderClear(renderer);

	// TODO: Render game objects

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
