#ifndef _MAZE_H_
#define _MAZE_H_

#include <fstream>
#include <string>
#include <vector>
#include <iostream>
#include <Windows.h>
#include <random>
#include <time.h>

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
	int rows;
	int columns;
	int numberOfChrom = 10;
	int numberOfMoves;
	int stepNumber = 0;
	int runNumber = 0;
	int penalty = 0;
	bool firstRun = true;

	std::string mazeInput;
	std::string mazeTemp;

	std::string chromosome[140];
	std::string offspring[140];
	std::string chromTemp;

	std::string movement[140][200];
	
	std::vector < std::vector <int> > maze;
	std::vector < std::vector <int> > chromArr;
	std::vector < std::vector <int> > positionsX;
	std::vector < std::vector <int> > positionsY;

	glm::vec2 playerPos;
	glm::vec2 finishPoint;
	glm::vec2 startingPoint;

	float totalFitness;
	std::vector <float> percentage;
	std::vector <int> pickOfFortune;
	//std::vector <float> fitness;
	float fitness[10] = { 0 };
	//double percentage[10] = { 0 };
	

public:
	Maze();
	~Maze();

	void MazeInit(std::string mazeFile, glm::vec2 _windowSize);
	void Simulation(SDL_Renderer *_renderer, std::string _chromFile);
	void Draw(SDL_Renderer* _renderer, Sprite* _open, Sprite* _wall, Sprite* _start, Sprite* _end);
	void Move();
	void GenerateChromosomes();
	int RNG();
	void FitnessCalculation(int _runNum);
	void Evolution();
};

#endif