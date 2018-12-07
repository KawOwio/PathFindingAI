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

	//Output maze in console
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

	//CHANGE THIS FOR DEBUGGING
	//numberOfMoves = rows * columns;
	numberOfMoves = 20;

	//
	chromArr.resize(numberOfChrom, std::vector<int>((numberOfMoves * 2), 0));
	GenerateChromosomes();

}

void Maze::Simulation(SDL_Renderer *_renderer, std::string _chromFile)
{
	bool quit = false;
	bool simulation = true;
	bool start = true;
	SDL_Event e;
	Sprite* openSpace = new Sprite(_renderer, "../Assets/Sprites/0_openSpace.bmp");
	Sprite* wall = new Sprite(_renderer, "../Assets/Sprites/1_wall.bmp");
	Sprite* startPoint = new Sprite(_renderer, "../Assets/Sprites/2_startingPoint.bmp");
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

	for (int i = 0; i < numberOfChrom; i++)
	{
		chromosome[i] = chromTemp.substr(0, chromTemp.find(' '));
		chromTemp.erase(0, chromTemp.find(' ') + 1);
	}

	for (int c = 0; c < numberOfChrom; c++)
	{
		chromTemp = chromosome[c];
		std::cout << "Chromosome " << c + 1 << ": ";
		for (int i = 0; i < (numberOfMoves); i++)
		{
			movement[c][i] = chromTemp.substr(0, 2);
			chromTemp.erase(0, 2);
			std::cout << movement[c][i];
		}
		std::cout << std::endl;
	}


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

		while(simulation == true)
		{
			//Cleares the screen
			SDL_RenderClear(_renderer);

			//Draws the maze (resets player for every new chromosome)
			if (start == true)
			{
				Draw(_renderer, openSpace, wall, startPoint, endPoint);
				start = false;
				SDL_RenderPresent(_renderer);
			}

			//Moves the "player" and outputs the maze to the screen
			Move();
			Draw(_renderer, openSpace, wall, startPoint, endPoint);

			//Checks if "the player" reached the end point
			if (maze[finishPoint.x][finishPoint.y] == 2)
			{
				simulation = false;
				Draw(_renderer, openSpace, wall, startPoint, endPoint);
			}

			//Outputs on the screen
			SDL_RenderPresent(_renderer);
			Sleep(200);

			runNum++;
			if (runNum >= (numberOfMoves))
			{
				//Calculate fitnes
				FitnessCalculation(runNum);

				//Resets characters position
				std::cout << playerPos.x << "<-X\tY->" << playerPos.y << std::endl;
				maze[playerPos.x][playerPos.y] = 0;
				maze[startingPoint.x][startingPoint.y] = 2;
				playerPos.x = startingPoint.x;
				playerPos.y = startingPoint.y;
				std::cout << playerPos.x << "<-X\tY->" << playerPos.y << std::endl;
				start = true;

				std::cout << "***NEW RUN***" << std::endl;

				runNum = 0;
				generationNumber++;

				if (generationNumber > numberOfChrom)
				{
					Evolution();
					generationNumber = 0;
				}
			}
		}
	}
}

void Maze::Draw(SDL_Renderer* _renderer, Sprite* _open, Sprite* _wall, Sprite* _start, Sprite* _end)
{
	bool firstRun = true;

	for (int r = 0; r < rows; r++)
	{
		for (int c = 0; c < columns; c++)
		{
			if (maze[r][c] == 1)
			{
				_wall->Draw(_renderer, positionsX[r][c], positionsY[r][c]);
			}
			else if (maze[r][c] == 2)
			{
				_start->Draw(_renderer, positionsX[r][c], positionsY[r][c]);
				playerPos.x = r;
				playerPos.y = c;
				if (firstRun == true)
				{
					startingPoint.x = r;
					startingPoint.y = c;
					firstRun = false;
				}
			}
			else if (maze[r][c] == 3)
			{
				_end->Draw(_renderer, positionsX[r][c], positionsY[r][c]);
				finishPoint.x = r;
				finishPoint.y = c;
			}
			else
			{
				_open->Draw(_renderer, positionsX[r][c], positionsY[r][c]);
			}
		}
	}
}
void Maze::Move()
{
	//Directions:
	/*
	00 - Up
	01 - Right
	10 - Down
	11 - Left
	*/

	if (movement[generationNumber][runNum] == "00")
	{
		//up
		if (playerPos.x != 0)
		{
			if (maze[playerPos.x - 1][playerPos.y] != 1)
			{
				maze[playerPos.x][playerPos.y] = 0;
				maze[playerPos.x - 1][playerPos.y] = 2;
			}
			else
			{
				std::cout << "Collision with a wall!" << std::endl;
			}
		}
		else
		{
			std::cout << "Collision with a border!" << std::endl;
		}
	}
	else if (movement[generationNumber][runNum] == "01")
	{
		//right
		if (playerPos.y != columns - 1)
		{
			if (maze[playerPos.x][playerPos.y + 1] != 1)
			{
				maze[playerPos.x][playerPos.y] = 0;
				maze[playerPos.x][playerPos.y + 1] = 2;
			}
			else
			{
				std::cout << "Collision with a wall!" << std::endl;
			}
		}
		else
		{
			std::cout << "Collision with a border!" << std::endl;
		}
	}
	else if (movement[generationNumber][runNum] == "10")
	{
		//down
		if (playerPos.x != rows - 1)
		{
			if (maze[playerPos.x + 1][playerPos.y] != 1)
			{
				maze[playerPos.x][playerPos.y] = 0;
				maze[playerPos.x + 1][playerPos.y] = 2;
			}
			else
			{
				std::cout << "Collision with a border!" << std::endl;
			}
		}
		else
		{
			std::cout << "Collision with a border!" << std::endl;
		}
	}
	else if (movement[generationNumber][runNum] == "11")
	{
		//left
		if (playerPos.y != 0)
		{
			if (maze[playerPos.x][playerPos.y - 1] != 1)
			{
				maze[playerPos.x][playerPos.y] = 0;
				maze[playerPos.x][playerPos.y - 1] = 2;
			}
			else
			{
				std::cout << "Collision with a border!" << std::endl;
			}
		}
		else
		{
			std::cout << "Collision with a border!" << std::endl;
		}
	}
	else
	{
		std::cout << "No movement" << std::endl;
	}
}
void Maze::GenerateChromosomes()
{
	//Make a text file for chromosomes
	//(can see generated chromosomes after the programmed finished working)
	std::ofstream chromOutput("../Assets/Chromosomes.txt");

	//Generates Y chromosomes with X binary numbers in each one
	//And outputs it to a .txt file
	for (int x = 0; x < numberOfChrom; x++)
	{
		for (int y = 0; y < (numberOfMoves * 2); y++)
		{
			chromArr[x][y] = RNG();
			chromOutput << chromArr[x][y];
		}
		chromOutput << " ";
	}
	chromOutput.close();
}
int Maze::RNG()
{
	int r = rand() % 2;;
	return r;
}
void Maze::FitnessCalculation(int _runNum)
{
	float Dx = finishPoint.x - playerPos.x;
	float Dy = finishPoint.y - playerPos.y;

	fitness[_runNum] = 1 / (Dx + Dy + 1);
}
void Maze::Evolution() 	  //WIP
{
	float totalFitness;
	percentage.resize(numberOfChrom);
	pickOfFortune.resize(numberOfChrom);
	float random;

	for (int i = 0; i < 4; i++)
	{
		totalFitness = +fitness[i];
	}

	for (int i = 0; i < 4; i++)
	{
		percentage[i] = fitness[i] / totalFitness * 100;
	}

	//Wheel of fortune
	for (int i = 0; i < numberOfChrom; i++)
	{
		random = rand() % 101;

		if (random >= 0 && random <= fitness[0])
		{
			//select 1st
			pickOfFortune[i] = 1;
		}
		else if (random > fitness[0] && random <= (fitness[0] + fitness[1]))
		{
			//2nd
			pickOfFortune[i] = 2;
		}
		else if (random > (fitness[0] + fitness[1]) && random <= (100 - fitness[3]))
		{
			//3rd
			pickOfFortune[i] = 3;
		}
		else if (random > (100 - fitness[3]) && random <= fitness[3])
		{
			//4th
			pickOfFortune[i] = 4;
		}
	}

	//Crossover operation
	float crossoverRate = 0.7f;

	for (int i = 0; i < numberOfChrom; i += 2)
	{
		//int crossover = rand() % 2;
		int crossover = 0.9;

		if (crossover <= crossoverRate)
		{
			//Perform crossover
			offspring[i] = chromosome[pickOfFortune[i]].substr(0, (numberOfMoves))
				+ chromosome[pickOfFortune[i]].substr((numberOfMoves), (numberOfMoves * 2));
			offspring[i+1] = chromosome[pickOfFortune[i+1]].substr(0, (numberOfMoves))
				+ chromosome[pickOfFortune[i+1]].substr((numberOfMoves), (numberOfMoves * 2));
			
			std::cout << offspring[i] << std::endl;
			std::cout << offspring[i+1] << std::endl;
		}
		else
		{
			//Do not perform crossover
			offspring[i] = chromosome[i];
			offspring[i+1] = chromosome[i+1];
		}
	}

	//Killing parents population, replacing them with the offsprings
	for (int i = 0; i < numberOfChrom; i++)
	{
		chromosome[i] = offspring[i];
	}

	//Mutation
	int mutationRate = 1;

	for (int c = 0; c < numberOfChrom; c++)
	{
		for (int i = 0; i < (numberOfMoves * 2); i++)
		{
			int mutation = rand() % 100;

			if (mutation <= mutationRate)
			{
				if (chromosome[c].at(i) == '0')
				{
					chromosome[c].at(i) = '1';
				}
				else if (chromosome[c].at(i) == '1')
				{
					chromosome[c].at(i) = '0';
				}
				else
				{
					std::cout << "Error with mutation!" << std::endl;
				}
			}
		}
	}
}