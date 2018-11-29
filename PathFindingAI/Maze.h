#ifndef _MAZE_H_
#define _MAZE_H_

#include <fstream>
#include <string>
#include <vector>
#include <iostream>

#include <glm.hpp>

//0 = open space
//1 = wall
//2 = starting point
//3 = end point

class Maze
{
private:
	std::string mazeInput;
	std::string mazeTemp;
	std::vector < std::vector <int> > maze;
	std::vector < std::vector <int> > positionsX;
	std::vector < std::vector <int> > positionsY;

public:
	Maze();
	~Maze();

	void MazeInit(std::string mazeFile, glm::vec2 _windowSize);
	void MazePositions();

};

#endif