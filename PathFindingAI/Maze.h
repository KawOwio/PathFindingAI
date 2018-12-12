#ifndef _MAZE_H_
#define _MAZE_H_

#include <fstream>
#include <vector>
#include <iostream>
#include <Windows.h>
#include <random>
#include <time.h>

#include "Sprite.h"

#define SDL_MAIN_HANDLED
#include <SDL.h>
#include <glm.hpp>

struct Coordinates
{
	glm::vec2 position;
	double cost = 0.0;
	bool visited = false;
	Coordinates *pointerToParent;
};

class Maze
{
private:
	//Shared
	int rows;
	int columns;

	bool firstRun = true;
	int algorithm = 1;	//2 == A* / 1 == GA

	std::string mazeInput;
	std::string mazeTemp;

	std::vector < std::vector <int> > maze;
	std::vector < std::vector <int> > positionsX;
	std::vector < std::vector <int> > positionsY;

	glm::vec2 playerPos;
	glm::vec2 finishPoint;
	glm::vec2 startingPoint;

	//GA
	int numberOfChrom = 10;
	int numberOfMoves;
	int stepNumber = 0;
	int runNumber = 0;
	int penalty[140] = { 0 };
	int totalPen = 0;
	float totalFitness;

	std::string chromosome[140];
	std::string offspring[140];
	std::string chromTemp;
	std::string movement[140][200];

	std::vector < std::vector <int> > chromArr;
	std::vector <float> fitness;
	std::vector <float> percentage;
	std::vector <int> pickOfFortune;

	//A*
	Coordinates nodeCurrent;
	Coordinates nodeLast;
	Coordinates nodeGoal;

	int dx[8] = { 1, 1, 0, -1, -1, -1, 0, 1 };
	int dy[8] = { 0, 1, 1, 1, 0, -1, -1, -1 };

	std::vector < Coordinates > closedNodes;
	std::vector < Coordinates > openNodes;

public:
	Maze();
	~Maze();

	//Shared
	void MazeInit(std::string mazeFile, glm::vec2 _windowSize);
	void Simulation(SDL_Renderer *_renderer, std::string _chromFile);
	void Draw(SDL_Renderer* _renderer, Sprite* _open, Sprite* _wall, Sprite* _start, Sprite* _end);

	//GA
	void MoveGA();
	void GenerateChromosomes();
	int RNG();
	void FitnessCalculation(int _runNum);
	void Evolution();

	//A*
	void CalculateCost(Coordinates &_node);
	void SearchAdjacent();
	bool CheckNodes(Coordinates _adj, std::vector <Coordinates> _list);
	void MoveAStar();

};
#endif