#pragma once
// #pragma once serves the same purpose as include guards, but with several advantages: 
// less code, avoidance of name clashes, and sometimes improvement in compilation speed
#include <SDL.h>
#include "../ECS/ECS.h"
#include "../AssetStore/AssetStore.h"
#include "../EventBus/EventBus.h" 

const int FPS = 60;
const int MILLISECS_PER_FRAME = 1000 / FPS;

class Game {
private:
	bool isRunning = false;
	bool isDebug;
	double millisecsPreviousFrame = 0;

	bool isFullscreen = false;
	bool isFakeFullscreen = false; // 800x600 escalados
	int windowMode = SDL_WINDOW_RESIZABLE;

	std::unique_ptr<Registry> registry;  // Registry* registry;
	std::unique_ptr<AssetStore> assetStore;
	std::unique_ptr<EventBus> eventBus;

	SDL_Window *window = NULL;
	SDL_Renderer *renderer = NULL;
public:
	Game(); //Constructor
	~Game(); //Destructor

	void Initialize();
	void Run();
	void LoadLevel(int level);
	void Setup();
	void ProcessInput();
	void Update();
	void Render();
	void Destroy();

	int windowWidth = 800;
	int windowHeight = 600;
};
