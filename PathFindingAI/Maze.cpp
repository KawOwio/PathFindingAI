#include "Maze.h"

//Change value of dimensions for a bigger resolution
#define dimensions 16

Maze::Maze()
{
	
}
Maze::~Maze()
{

}

//Shared Functions
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

	//Initialise positions for each maze "square"
	glm::vec2 middlePoint((_windowSize.x / 2), (_windowSize.y / 2));
	glm::vec2 temp((columns / 2), (rows / 2));
	glm::vec2 startPoint((middlePoint.x - (temp.x * dimensions)), middlePoint.y - (temp.y * dimensions));

	//Initialising 2 2D vectors storing the positions of each "square"
	for (int c = 0; c < columns; c++)
	{
		for (int r = 0; r < rows; r++)
		{
			positionsX[r][c] = startPoint.x + c * dimensions;
		}
	}	
	for (int r = 0; r < rows; r++)
	{
		for (int c = 0; c < columns; c++)
		{
			positionsY[r][c] = startPoint.y + r * dimensions;
		}
	}

	//Length of chromosomes for GA (not the best solution)
	if (rows * columns > 50)
	{
		numberOfMoves = rows * columns;
	}
	else
	{
		numberOfMoves = rows * columns / 2;
	}

	//Generates chromosomes
	chromArr.resize(numberOfChrom, std::vector<int>((numberOfMoves * 2), 0));
	GenerateChromosomes();

	//Resizes the rest of vectors
	percentage.resize(numberOfChrom);
	pickOfFortune.resize(numberOfChrom);
	fitness.resize(numberOfChrom);
}
void Maze::Simulation(SDL_Renderer *_renderer, std::string _chromFile, char _c)
{
	int generation = 1;
	bool quit = false;
	bool simulation = true;
	bool start = true;
	SDL_Event e;
	Sprite* openSpace = new Sprite(_renderer, "../Assets/Sprites/0_openSpace.bmp");
	Sprite* wall = new Sprite(_renderer, "../Assets/Sprites/1_wall.bmp");
	Sprite* startPoint = new Sprite(_renderer, "../Assets/Sprites/2_startingPoint.bmp");
	Sprite* endPoint = new Sprite(_renderer, "../Assets/Sprites/3_endPoint.bmp");
	
	//Preparation
	if (_c == '1')
	{
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
		//Splits chromosomes into steps
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

		//Start the clock to measure how long did it make for algorithm to complete a maze
		int start_s = clock();

		while(simulation == true)
		{
			//Cleares the screen and sets the background colour
			SDL_SetRenderDrawColor(_renderer, 125, 125, 125, 255);
			SDL_RenderClear(_renderer);

			//Draws the maze (resets player for every new run)
			if (start == true)
			{
				Draw(_renderer, openSpace, wall, startPoint, endPoint);
				start = false;
				SDL_RenderPresent(_renderer);
				//Sleep(20);
			}

			//Genetic algorithm
			if (_c == '1')
			{
				//Moves the character and outputs the maze to the screen
				MoveGA();
				Draw(_renderer, openSpace, wall, startPoint, endPoint);

				//Checks if the character reached the end point
				if (maze[finishPoint.x][finishPoint.y] == 2)
				{
					Draw(_renderer, openSpace, wall, startPoint, endPoint);
					std::cout << "Finished" << std::endl;
					//Stops the clock
					int stop_s = clock();
					//Calculates time taken for program to run
					std::cout << "Time: " << (stop_s - start_s) / double(CLOCKS_PER_SEC) << " seconds\n\n";
					simulation = false;
					SDL_RenderPresent(_renderer);
					break;
				}

				//Output to the screen
				SDL_RenderPresent(_renderer);
				//Sleep(10);

				//Steps represent each 2 bits in chromosomes
				stepNumber++;

				//Moves to the next chromosome
				if (stepNumber >= (numberOfMoves))
				{
					//Calculate fitnes
					FitnessCalculation(runNumber);

					//Resets characters position
					maze[playerPos.x][playerPos.y] = 0;
					maze[startingPoint.x][startingPoint.y] = 2;
					playerPos.x = startingPoint.x;
					playerPos.y = startingPoint.y;
					start = true;


					stepNumber = 0;
					runNumber++;

					//Moves to the next generation
					if (runNumber >= numberOfChrom)
					{
						for (int i = 0; i < numberOfChrom; i++)
						{
							penalty[i] = 0;
						}
						Evolution();
						std::cout << ++generation << std::endl;
						//Splits chromosomes into steps
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

						runNumber = 0;
					}
				}
			}

			//A* Algorithm
			if (_c == '2')
			{
				//Starting node to the open list 
				nodeCurrent.position = startingPoint;
				CalculateCost(nodeCurrent);
				openNodes.push_back(nodeCurrent);
				
				//while the open list is not empty
				while (openNodes.size() > 0)
				{
					double lowestCost = 100000.0;
					//nodeLast = nodeCurrent;
					SDL_RenderPresent(_renderer);
					//current node is a node with the lowest cost
					int choice = 0;
					for (int i = 0; i < openNodes.size(); i++)
					{
						if (openNodes[i].cost <= lowestCost)
						{
							lowestCost = openNodes[i].cost;
							choice = i;
						}
						nodeCurrent = openNodes[choice];
					}
					Sleep(100);

					//Uncomment the next line (and line 264) to clear the travelled path
					//maze[nodeLast.position.x][nodeLast.position.y] = 0;
					maze[nodeCurrent.position.x][nodeCurrent.position.y] = 2;
					Draw(_renderer, openSpace, wall, startPoint, endPoint);
					SDL_RenderPresent(_renderer);

					//If finished
					if (nodeCurrent.position == finishPoint)
					{
						std::cout << "Finish" << std::endl;
						//Stops the clock
						int stop_s = clock();
						//Calculates time taken for program to run
						std::cout << "Time: " << (stop_s - start_s) / double(CLOCKS_PER_SEC) << " seconds\n\n";
						simulation = false;
						SDL_RenderPresent(_renderer);
						break;
					}
					else
					{
						nodeCurrent.visited = true;
						//Move current node to the closed list
						closedNodes.push_back(nodeCurrent);
						openNodes.clear();
						//Search for new nodes (adjecent ones)
						SearchAdjacent();
					}

					//If the character gets stuck then it goes through closed list
					//and checks for adjacent nodes until it finds another possible move
					int r = 0;
					while (openNodes.size() == 0 && nodeCurrent.position != finishPoint)
					{
						//nodeLast = nodeCurrent;
						
						//optimization so it checks for 10 possible moves at the time
						//and pick one with the lowest cost
						//(gets a slower towards the end but still completes 200x200 maze quicker with optimisation)
						openNodes.clear();
						for (int i = 0; i < closedNodes.size(); i++)
						{
							nodeCurrent = closedNodes[closedNodes.size() - 1 - i];
							SearchAdjacent();

							if (openNodes.size() >= 10)
							{
								i = closedNodes.size();
							}
						}

						Sleep(100);

						//Uncomment these for removing the "travel path" from the maze 
						//(and lines: 263, 279, 312)
						/*maze[nodeLast.position.x][nodeLast.position.y] = 0;
						maze[nodeCurrent.position.x][nodeCurrent.position.y] = 2;
						Draw(_renderer, openSpace, wall, startPoint, endPoint);
						SDL_RenderPresent(_renderer);*/
						r++;
					}
				}
				simulation = false;
				//Output to the screen
				SDL_RenderPresent(_renderer);
				Sleep(10);	
			}	
		}
	}
}
void Maze::Draw(SDL_Renderer* _renderer, Sprite* _open, Sprite* _wall, Sprite* _start, Sprite* _end)
{
	//Goes through every single row and column in a maze to output it to the screen
	for (int r = 0; r < rows; r++)
	{
		for (int c = 0; c < columns; c++)
		{
			//wall
			if (maze[r][c] == 1)
			{
				_wall->Draw(_renderer, positionsX[r][c], positionsY[r][c], dimensions);
			}
			//start point
			else if (maze[r][c] == 2)
			{
				_start->Draw(_renderer, positionsX[r][c], positionsY[r][c], dimensions);
				playerPos.x = r;
				playerPos.y = c;
				if (firstRun == true)
				{
					startingPoint.x = r;
					startingPoint.y = c;
					firstRun = false;
				}
			}
			//end point
			else if (maze[r][c] == 3)
			{
				_end->Draw(_renderer, positionsX[r][c], positionsY[r][c], dimensions);
				finishPoint.x = r;
				finishPoint.y = c;
			}
			//open space
			else
			{
				_open->Draw(_renderer, positionsX[r][c], positionsY[r][c], dimensions);
			}
		}
	}
}

//Genetic Algorithm
void Maze::MoveGA()
{
	//up
	if (movement[runNumber][stepNumber] == "00")
	{
		if (playerPos.x != 0)
		{
			if (maze[playerPos.x - 1][playerPos.y] != 1)
			{
				maze[playerPos.x][playerPos.y] = 0;
				maze[playerPos.x - 1][playerPos.y] = 2;
			}
			else
			{
				penalty[runNumber] += 10;
				//std::cout << "Collision with a wall!" << std::endl;
			}
		}
		else
		{
			penalty[runNumber] += 10;
			//std::cout << "Collision with a border!" << std::endl;
		}
	}
	//right
	else if (movement[runNumber][stepNumber] == "01")
	{
		if (playerPos.y != columns - 1)
		{
			if (maze[playerPos.x][playerPos.y + 1] != 1)
			{

				maze[playerPos.x][playerPos.y] = 0;
				maze[playerPos.x][playerPos.y + 1] = 2;
			}
			else
			{
				penalty[runNumber] += 10;
				//std::cout << "Collision with a wall!" << std::endl;
			}
		}
		else
		{
			penalty[runNumber] += 10;
			//std::cout << "Collision with a border!" << std::endl;
		}
	}
	//down
	else if (movement[runNumber][stepNumber] == "10")
	{
		if (playerPos.x != rows - 1)
		{
			if (maze[playerPos.x + 1][playerPos.y] != 1)
			{
				maze[playerPos.x][playerPos.y] = 0;
				maze[playerPos.x + 1][playerPos.y] = 2;
			}
			else
			{
				penalty[runNumber] += 10;
				//std::cout << "Collision with a border!" << std::endl;
			}
		}
		else
		{
			penalty[runNumber] += 10;
			//std::cout << "Collision with a border!" << std::endl;
		}
	}
	//left
	else if (movement[runNumber][stepNumber] == "11")
	{
		if (playerPos.y != 0)
		{
			if (maze[playerPos.x][playerPos.y - 1] != 1)
			{
				maze[playerPos.x][playerPos.y] = 0;
				maze[playerPos.x][playerPos.y - 1] = 2;
			}
			else
			{
				penalty[runNumber] += 10;
				//std::cout << "Collision with a border!" << std::endl;
			}
		}
		else
		{
			penalty[runNumber] += 10;
			//std::cout << "Collision with a border!" << std::endl;
		}
	}
	else
	{
		std::cout << "No movement(bug)" << std::endl;
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

	//Absolute value
	if (Dx < 0)
	{
		Dx *= -1;
	}
	if (Dy < 0)
	{
		Dy *= -1;
	}

	fitness[_runNum] = 1 / (Dx + Dy + penalty[runNumber]);
	//std::cout << "fitness " << _runNum << ": " << fitness[_runNum] << std::endl;
}
void Maze::Evolution()
{
	totalFitness = 0;
	float random;

	//total fitness calculation
	for (int i = 0; i < numberOfChrom; i++)
	{
		totalFitness += fitness[i];
	}

	//Wheel of fortune
	float totalPercentage = 0.0f;
	int r = 0;
	bool assigned = false;

	for (int n = 0; n < numberOfChrom; n++)
	{
		assigned = false;
		totalPercentage = 0;
		r = 0;
		random = rand() % 100;

		while (assigned == false)
		{
			totalPercentage += percentage[r];
			if (random <= totalPercentage)
			{
				totalPercentage - percentage[r];
				pickOfFortune[n] = r;
				assigned = true;
				//std::cout << "Pick " << n + 1 << ": " << pickOfFortune[n] + 1 << std::endl;
			}
			else
			{
				r++; 
			}
		}
	}

	//Crossover operation (at random point)
	int crossoverRate = 7;
	for (int i = 0; i < numberOfChrom; i += 2)
	{
		int crossover = rand() % 11;
		r = rand() % numberOfMoves / 2;

		if (crossover <= crossoverRate)
		{
			//Perform crossover
			offspring[i] = chromosome[pickOfFortune[i]].substr(0, r)
				+ chromosome[pickOfFortune[i+1]].substr(r, (numberOfMoves * 2));
			offspring[i+1] = chromosome[pickOfFortune[i+1]].substr(0, r)
				+ chromosome[pickOfFortune[i]].substr(r, (numberOfMoves * 2));
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
	int mutationRate = 2;
	for (int c = 0; c < numberOfChrom; c++)
	{
		for (int i = 0; i < (numberOfMoves * 2); i++)
		{
			int mutation = rand() % 10;

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

//A*
void Maze::CalculateCost(Coordinates &_node)
{
	double deltaX = finishPoint.x - _node.position.x;
	double deltaY = finishPoint.y - _node.position.y;

	double c = sqrt(pow(deltaX, 2) + pow(deltaY, 2));
	if (c < 0)
	{
		c *= -1;
	}
	_node.cost = c;
}
void Maze::SearchAdjacent()
{
	for (int i = 0; i < 4; i++)
	{
		Coordinates adjacent;
		adjacent.position = glm::vec2(nodeCurrent.position.x + dx[i], nodeCurrent.position.y + dy[i]); 
		if (CheckNodes(adjacent, openNodes) == false)	//If the adjacent is in the open list 
		{
			if (CheckNodes(adjacent, closedNodes) == false)	//If the adjacent is in the closed list 
			{
				if (adjacent.position.x >= 0 && adjacent.position.x < rows
					&& adjacent.position.y >= 0 && adjacent.position.y < columns
					&& maze[adjacent.position.x][adjacent.position.y] != 1)	//If the adjacent isn't out of bounds and isn't a wall
				{
					//Assigns adjacent nodes to an open list (if all criterias are met) 
					CalculateCost(adjacent);
					openNodes.push_back(adjacent);	
				}
			}
		}
	}
}
bool Maze::CheckNodes(Coordinates _adj, std::vector <Coordinates> _list)
{
	if (_list.size() == 0)
	{
		return false;
	}
	for (int l = 0; l < _list.size(); l++)
	{
		if (_adj.position == _list[l].position)
		{
			return true;
		}
		else if (_adj.position != _list[l].position)
		{
			if (_list[l].visited != true)
			{
				return false;
			}
		}
	}
	return false;
}