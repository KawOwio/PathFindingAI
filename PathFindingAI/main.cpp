#include <cmath>
#include <time.h>

#include "Maze.h"

int RNG(); //Generates a random number

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

	//Stops the clock
	int stop_s = clock();
	//Calculates time taken for program to run
	std::cout << "Time: " << (stop_s - start_s) / double(CLOCKS_PER_SEC) << " seconds\n\n";

	mazeClass.Simulation(renderer, "../Assets/Chromosomes.txt");

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

int RNG()
{
	int r = rand() % 2;;
	return r;
}