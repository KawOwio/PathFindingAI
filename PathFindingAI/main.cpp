#include <Windows.h>
#include <cmath>
#include <time.h>
#include <random>

#include "Maze.h"

int RNG(); //Generates a random number

int main()
{
	Maze mazeClass;

	//Starts the clock
	int start_s = clock();

	//Seed for random number generator 
	srand(time(NULL));
	int randomNumber;

	//Initialisation of the maze
	mazeClass.MazeInnit("../Assets/Maps/maze3.txt");

	//20 chromosomes(y) each is 10 numbers long(x)
	int chromosomes[10][20] = { 0 };
	//Generates Y chromosomes with X binary numbers in each one
	for (int y = 0; y < 20; y++)
	{
		//std::cout << "Chromosome " << y + 1 << ": ";
		for (int x = 0; x < 10; x++)
		{
			chromosomes[x][y] = RNG();
			//std::cout << chromosomes[x][y];
		}
		std::cout << std::endl;
	}

	//Stops the clock
	int stop_s = clock();
	//Calculates time taken for program to run
	std::cout << "Time: " << (stop_s - start_s) / double(CLOCKS_PER_SEC) << " seconds\n\n";

	std::cout << "\n\n\n";
	system("PAUSE");
	return 0;
}

int RNG()
{
	int r = rand() % 2;;
	return r;
}