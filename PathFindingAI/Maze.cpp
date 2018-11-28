#include "Maze.h"

//0 = open space
//1 = wall
//2 = starting point
//3 = end point

Maze::Maze()
{
	
}
Maze::~Maze()
{

}

std::string Maze::MazeInnit(std::string mazeFile)
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

	return mazeInput;
}

void Maze::SetX(int _x)
{
	m_mazeX = _x;
}
void Maze::SetY(int _y)
{
	m_mazeY = _y;
}

int Maze::GetX()
{
	return m_mazeX;
}
int Maze::GetY()
{
	return m_mazeY;
}