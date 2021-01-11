#pragma once

// #pragma once serves the same purpose as include guards, but with several advantages: 
// less code, avoidance of name clashes, and sometimes improvement in compilation speed

class Game
{
	private:
		// TODO
	public:
		Game(); //Constructor
		~Game(); //Destructor
		void Initialize();
		void Run();
		void ProcessInput();
		void Update();
		void Render();
		void Destroy();
};
