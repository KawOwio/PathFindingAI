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
	int columns = atoi(mazeTemp.c_str());

	//Finds second number in a string of numbers (number of rows)
	mazeTemp = mazeInput.substr(0, mazeInput.find(' '));
	mazeInput.erase(0, mazeInput.find(' ') + 1);
	int rows = atoi(mazeTemp.c_str());

	//Crates a 2D array that represents a maze
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
	for (int r = 0; r < rows; r++)
	{
		for (int c = 0; c < columns; c++)
		{
			std::cout << maze[r].at(c);
		}
		std::cout << std::endl;
	}

	//Initialise positions for each maze "square"
	glm::vec2 middlePoint((_windowSize.x / 2), (_windowSize.y / 2));
	glm::vec2 temp((columns / 2), (rows / 2));
	glm::vec2 startPoint((middlePoint.x - (temp.x * 32)), middlePoint.y - (temp.y * 32));

	std::cout << startPoint.x << "\t" << startPoint.y << std::endl;

	for (int c = 0; c < columns; c++)
	{
		for (int r = 0; r < rows; r++)
		{
			positionsX[r][c] = startPoint.x + r * 32;
		}
	}	
	
	//FIX THIS//
	for (int r = 0; r < rows; r++)
	{
		for (int c = 0; c < columns; c++)
		{
			positionsY[r][c] = startPoint.y + r * 32;
		}
	}

	for (int r = 0; r < rows; r++)
	{
		for (int c = 0; c < columns; c++)
		{
			std::cout << positionsX[r][c];
		}
		std::cout << std::endl;
	}

	for (int r = 0; r < rows; r++)
	{
		for (int c = 0; c < columns; c++)
		{
			std::cout << positionsY[r][c];
		}
		std::cout << std::endl;
	}


}