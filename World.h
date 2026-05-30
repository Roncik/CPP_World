#pragma once
#include "Organism.h"

class World
{
private:
	int _worldX;
	int _worldY;
	int _turn{ 0 };
	
	char separator{ '.' };

	std::vector<std::unique_ptr<Organism>> organisms;
public:
	World(int worldX, int worldY);
	World() : World(6, 6) {};

	bool getOrganismFromPosition(int x, int y, size_t* index = nullptr);
	bool isPositionOnWorld(int x, int y);
	bool isPositionFree(Position position);

	int getWorldX();
	void setWorldX(int worldX);
	int getWorldY();
	void setWorldY(int worldY);

	int getTurn();
	
	void addOrganism(std::unique_ptr<Organism>& organism);
	const std::vector<std::unique_ptr<Organism>>& getOrganisms();

	void removeOrganism(size_t index);
	std::vector<Position> getVectorOfPositionsAround(Position position, bool onlyFreePositions = false);
	void makeTurn();

	void handleMove(size_t& orgIndex, bool isAnimal, bool isCarnivore);
	
	std::string toString();

	void Randomize(int worldX, int worldY, size_t numOfOrganisms, const std::vector<char>& possibleOrganismsSigns);

	void clear();

	//SERIALIZATION
	std::string serialize();
	void writeWorld(std::string fileName);
	void readWorld(std::string fileName);
};

