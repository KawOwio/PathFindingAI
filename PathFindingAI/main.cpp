#include <Windows.h>
#include <cmath>
#include <time.h>
#include <random>

#include "Maze.h"
#include "Sprite.h"

#define SDL_MAIN_HANDLED
#include <SDL.h>
#include <glm.hpp>

int RNG(); //Generates a random number
void Simulation(SDL_Renderer* _renderer);

int main(int argc, char* args[])
{
	glm::vec2 windowSize(1000, 1000);

	//Initialisation of SDL2 for graphical representation
	SDL_Window* window = nullptr;
	SDL_Surface* surface = nullptr;
	SDL_Renderer* renderer = nullptr;
	SDL_Texture* texture = nullptr;

	if (SDL_Init(SDL_INIT_EVERYTHING) == -1)
	{
		std::cout << "Error initialising SDL" << SDL_GetError() << "\n\n\n";
	}

	window = SDL_CreateWindow("Pathfinding AI", 460, 40, windowSize.x, windowSize.y, 0);
	renderer = SDL_CreateRenderer(window, -1, 0);

	Maze mazeClass;

	//Starts the clock
	int start_s = clock();

	//Seed for random number generator 
	srand(time(NULL));

	//Initialisation of the maze
	mazeClass.MazeInit("../Assets/Maps/maze2.txt", windowSize);

	//20 chromosomes(y) each is 10 numbers long(x)
	int chromosomes[10][20] = { 0 };
	//Generates Y chromosomes with X binary numbers in each one
	for (int y = 0; y < 20; y++)
	{
		std::cout << "Chromosome " << y + 1 << ": ";
		for (int x = 0; x < 10; x++)
		{
			chromosomes[x][y] = RNG();
			std::cout << chromosomes[x][y];
		}
		std::cout << std::endl;
	}

	//Stops the clock
	int stop_s = clock();
	//Calculates time taken for program to run
	std::cout << "Time: " << (stop_s - start_s) / double(CLOCKS_PER_SEC) << " seconds\n\n";

	//Freeing memory
	window = nullptr;
	surface = nullptr;
	renderer = nullptr;
	texture = nullptr;

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_DestroyTexture(texture);

	SDL_Quit();

	std::cout << "\n\n\n";
	system("PAUSE");
	return 0;
}

void Simulation(SDL_Renderer* _renderer)
{
	bool quit = false;
	SDL_Event e;
	Sprite* openSpace = new Sprite(_renderer, "../Assets/Sprites/0_openSpace.bmp");
	Sprite* wall = new Sprite(_renderer, "../Assets/Sprites/1_wall.bmp");
	Sprite* startingPoint = new Sprite(_renderer, "../Assets/Sprites/2_startingPoint.bmp");
	Sprite* endPoint = new Sprite(_renderer, "../Assets/Sprites/3_endPoint.bmp");

	while (!quit)
	{
		//Quit simulation
		while (SDL_PollEvent(&e))
		{
			if (e.type == SDL_QUIT)
			{
				quit = true;
			}
		}

		SDL_RenderClear(_renderer);

		openSpace->Draw(_renderer);
		wall->Draw(_renderer);
		startingPoint->Draw(_renderer);
		endPoint->Draw(_renderer);

		SDL_RenderPresent(_renderer);
	}
}

int RNG()
{
	int r = rand() % 2;;
	return r;
}