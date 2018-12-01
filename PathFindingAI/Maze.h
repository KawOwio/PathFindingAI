#ifndef _MAZE_H_
#define _MAZE_H_

#include <fstream>
#include <string>
#include <vector>
#include <iostream>

#include "Sprite.h"

#define SDL_MAIN_HANDLED
#include <SDL.h>
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

	std::string chromosome[20];
	std::string chromTemp;

	std::vector < std::vector <int> > maze;
	std::vector < std::vector <int> > positionsX;
	std::vector < std::vector <int> > positionsY;

	int rows;
	int columns;

public:
	Maze();
	~Maze();

	void MazeInit(std::string mazeFile, glm::vec2 _windowSize);
	void Simulation(SDL_Renderer *_renderer, std::string _chromFile);

};

#endif