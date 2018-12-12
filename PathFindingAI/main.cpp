//**********************************************************//
//			 Pathfinding algorithms: A* and GA				//
//		   Created by Nikita Gribuska (s5065617)			//
//	GitHub link: https://github.com/s5065617/PathFindingAI	//
//**********************************************************//

#include "Maze.h"

int main(int argc, char* args[])
{
	//Simple text menu
	char choice = '0';
	bool choiceMade = false;
	while (choiceMade == false)
	{
		std::cout << "Choose the algorithm you want to use.";
		std::cout << "\n1. Genetic Algorithm\n2. A* Algorithm\n3. Quit the program";
		std::cout << "\nYour choice: ";
		std::cin >> choice;

		if (choice == '1' || choice == '2')
		{
			choiceMade = true;
			system("cls");
		}
		else if (choice == '3')
		{
			return 0;
		}
		else
		{
			system("cls");
			std::cout << "Wrong input!" << std::endl;
		}
	}

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

	window = SDL_CreateWindow("Pathfinding AI",
		SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		windowSize.x, windowSize.y, 0);

	renderer = SDL_CreateRenderer(window, -1, 0);

	Maze mazeClass;

	//Seed for random number generator 
	srand(time(NULL));

	//Initialisation of the maze and starting the simulation
	//Input your maze file name in there \/
	mazeClass.MazeInit("../Assets/Maps/hardmaze2.txt", windowSize);

	mazeClass.Simulation(renderer, "../Assets/Chromosomes.txt", choice);
	SDL_DestroyWindow(window);

	//Freeing memory
	window = nullptr;
	surface = nullptr;
	renderer = nullptr;
	texture = nullptr;

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_DestroyTexture(texture);

	system("pause");
	SDL_Quit();

	return 0;
}