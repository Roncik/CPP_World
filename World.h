#pragma once

#include <vector>
#include <ctime>
#include "Organism.h"

class World
{
private:
	int worldX;
	int worldY;
	int turn{ 0 };
	
	char separator{ '.' };

	bool getOrganismFromPosition(int x, int y, size_t* index = nullptr);
	bool isPositionOnWorld(int x, int y);
	bool isPositionFree(Position position);

public:
	std::vector<std::unique_ptr<Organism>> organisms;
	World(int worldX, int worldY);
	World() : World(6, 6) {};

	int getWorldX();
	void setWorldX(int worldX);
	int getWorldY();
	void setWorldY(int worldY);

	int getTurn();
	
	void addOrganism(std::unique_ptr<Organism>& organism);

	void removeOrganism(size_t index);
	std::vector<Position> getVectorOfPositionsAround(Position position, bool onlyFreePositions = false);
	void makeTurn();

	void handleMove(size_t& orgIndex, bool isAnimal, bool isCarnivore);
	
	std::string toString();

	//SERIALIZATION
	std::string serialize();
	void writeWorld(std::string fileName);
	void readWorld(std::string fileName);
};

