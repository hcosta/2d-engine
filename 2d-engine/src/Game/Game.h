#pragma once
#include <SDL.h>
// #pragma once serves the same purpose as include guards, but with several advantages: 
// less code, avoidance of name clashes, and sometimes improvement in compilation speed

const int FPS = 60;
const int MILLISECS_PER_FRAME = 1000 / FPS;

class Game {
private:
	bool isRunning = false;
	bool isFullscreen = false;
	bool isFakeFullscreen = false; // 800x600 escalados
	int windowMode = SDL_WINDOW_RESIZABLE;
	double millisecsPreviousFrame = 0;

	SDL_Window *window = NULL;
	SDL_Renderer *renderer = NULL;
public:
	Game(); //Constructor
	~Game(); //Destructor

	void Initialize();
	void Run();
	void Setup();
	void ProcessInput();
	void Update();
	void Render();
	void Destroy();

	int windowWidth = 800;
	int windowHeight = 600;
};
