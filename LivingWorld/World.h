#pragma once

#include <vector>
#include <ctime>
#include "Organism.h"

class World
{
private:
	int worldX;
	int worldY;
	int turn = 0;
	std::vector<Organism> organisms;
	char separator = '.';

	char getOrganismFromPosition(int x, int y);
	bool isPositionOnWorld(int x, int y);
	bool isPositionFree(Position position);

public:
	World(int worldX, int worldY);
	World() : World(6, 6) {};

	int getWorldX();
	void setWorldX(int worldX);
	int getWorldY();
	void setWorldY(int worldY);

	int getTurn();
	
	void addOrganism(Organism *organism);
	void removeOrganism(size_t index);
	std::vector<Position> getVectorOfFreePositionsAround(Position position);
	void makeTurn();

	void writeWorld(std::string fileName);
	void readWorld(std::string fileName);
	
	std::string toString();

};

