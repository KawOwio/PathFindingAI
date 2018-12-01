#include "Maze.h"

Maze::Maze()
{
	
}
Maze::~Maze()
{

}

void Maze::MazeInit(std::string mazeFile, glm::vec2 _windowSize)
{
	std::ifstream file(mazeFile);

	//Check if file was open
	if (!file.is_open())
	{
		throw std::exception();
	}
	else
	{
		std::string line;
		std::getline(file, line);
		mazeInput = line;
	}
	file.close();

	//Finds first number in a string of numbers (number of columns)
	mazeTemp = mazeInput.substr(0, mazeInput.find(' '));
	mazeInput.erase(0, mazeInput.find(' ') + 1);
	columns = atoi(mazeTemp.c_str());

	//Finds second number in a string of numbers (number of rows)
	mazeTemp = mazeInput.substr(0, mazeInput.find(' '));
	mazeInput.erase(0, mazeInput.find(' ') + 1);
	rows = atoi(mazeTemp.c_str());

	//Crates a 2D vector that represents a maze
	maze.resize(rows, std::vector<int>(columns, 0));
	positionsX.resize(rows, std::vector<int>(columns, 0));
	positionsY.resize(rows, std::vector<int>(columns, 0));


	//Generate maze from a text file
	for (int r = 0; r < rows; r++)
	{
		for (int c = 0; c < columns; c++)
		{
			mazeTemp = mazeInput.substr(0, mazeInput.find(' '));
			mazeInput.erase(0, mazeInput.find(' ') + 1);
			maze[r][c] = atoi(mazeTemp.c_str());
		}
	}

	//Output maze
	/*for (int r = 0; r < rows; r++)
	{
		for (int c = 0; c < columns; c++)
		{
			std::cout << maze[r].at(c);
		}
		std::cout << std::endl;
	}*/

	//Initialise positions for each maze "square"
	glm::vec2 middlePoint((_windowSize.x / 2), (_windowSize.y / 2));
	glm::vec2 temp((columns / 2), (rows / 2));
	glm::vec2 startPoint((middlePoint.x - (temp.x * 64)), middlePoint.y - (temp.y * 64));

	//Initialising 2 2D vectors storing the positions of each "square"
	for (int c = 0; c < columns; c++)
	{
		for (int r = 0; r < rows; r++)
		{
			positionsX[r][c] = startPoint.x + c * 64;
		}
	}	
	for (int r = 0; r < rows; r++)
	{
		for (int c = 0; c < columns; c++)
		{
			positionsY[r][c] = startPoint.y + r * 64;
		}
	}
}

void Maze::Simulation(SDL_Renderer *_renderer, std::string _chromFile)
{
	bool quit = false;
	SDL_Event e;
	Sprite* openSpace = new Sprite(_renderer, "../Assets/Sprites/0_openSpace.bmp");
	Sprite* wall = new Sprite(_renderer, "../Assets/Sprites/1_wall.bmp");
	Sprite* startingPoint = new Sprite(_renderer, "../Assets/Sprites/2_startingPoint.bmp");
	Sprite* endPoint = new Sprite(_renderer, "../Assets/Sprites/3_endPoint.bmp");
	
	//Convert a text file of chromosomes to a string
	std::ifstream file(_chromFile);
	//Check if file was open
	if (!file.is_open())
	{
		throw std::exception();
	}
	else
	{
		std::string line;
		std::getline(file, line);
		chromTemp = line;
	}
	file.close();

	//FINISH CHROMOSOMES
	chromosome[0] = chromTemp.substr(0, chromTemp.find(' '));
	chromTemp.erase(0, chromTemp.find(' ') + 1);
	std::cout << chromosome[0] << std::endl;

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

		//Cleares the screen
		SDL_RenderClear(_renderer);

		//Draws the maze
		for (int r = 0; r < rows; r++)
		{
 			for (int c = 0; c < columns; c++)
			{
				if (maze[r][c] == 1)
				{
					wall->Draw(_renderer, positionsX[r][c], positionsY[r][c]);
				}
				else if (maze[r][c] == 2)
				{
					startingPoint->Draw(_renderer, positionsX[r][c], positionsY[r][c]);
				}
				else if (maze[r][c] == 3)
				{
					endPoint->Draw(_renderer, positionsX[r][c], positionsY[r][c]);
				}
				else
				{
					openSpace->Draw(_renderer, positionsX[r][c], positionsY[r][c]);
				}
			}
		}

		/*Directions:
		00 - Up
		01 - Right
		10 - Down
		11 - Left
		*/



		//Outputs on the screen
		SDL_RenderPresent(_renderer);
	}
}