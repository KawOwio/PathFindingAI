#ifndef _MAZE_H_
#define _MAZE_H_

#include <fstream>
#include <string>
#include <vector>
#include <iostream>
#include <Windows.h>
#include <random>

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

	std::string movement[20][5];

	std::vector < std::vector <int> > maze;
	std::vector < std::vector <int> > chromArr;
	std::vector < std::vector <int> > positionsX;
	std::vector < std::vector <int> > positionsY;

	glm::vec2 playerPos;
	glm::vec2 finishPoint;

	int rows;
	int columns;
	int runNum = 0;
	int chromNum = 0;

public:
	Maze();
	~Maze();

	void MazeInit(std::string mazeFile, glm::vec2 _windowSize);
	void Simulation(SDL_Renderer *_renderer, std::string _chromFile);
	void Draw(SDL_Renderer* _renderer, Sprite* _open, Sprite* _wall, Sprite* _start, Sprite* _end);
	void Move();
	void GenerateChromosomes();
	int RNG();
};

#endif