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
		maze = line;
	}
	file.close();


	return maze;
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