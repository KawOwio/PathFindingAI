#pragma once

#include <fstream>
#include <string>

//0 = open space
//1 = wall
//2 = starting point
//3 = end point

class Maze
{
private:
	int m_mazeX;
	int m_mazeY;
	std::string maze;

public:
	Maze();
	~Maze();

	std::string MazeInnit(std::string mazeFile);

	void SetX(int _x);
	void SetY(int _y);

	int GetX();
	int GetY();
};