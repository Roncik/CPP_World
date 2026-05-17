#include "../pch.h"
#include "World.h"

char World::getOrganismFromPosition(int x, int y)
{	
	for (Organism org : organisms)
		if (org.getPosition().getX() == x && org.getPosition().getY() == y)
			return org.getSign();
	return 0;
}

bool World::isPositionOnWorld(int x, int y)
{
	return (x >= 0 && y >= 0 && x < getWorldX() && y < getWorldY());
}

bool World::isPositionFree(Position position) {
	return this->getOrganismFromPosition(position.getX(), position.getY());
}

std::vector<Position> World::getVectorOfFreePositionsAround(Position position)
{	
	int pos_x = position.getX(), pos_y = position.getY();
	std::vector<Position> result;
	for(int x = -1; x < 2; ++x)
		for (int y = -1; y < 2; ++y)
			if ((x != 0 || y != 0) && 
				isPositionOnWorld(pos_x + x, pos_y + y)) {
				result.push_back(Position(pos_x + x, pos_y + y));
			}
	auto iter = std::remove_if(result.begin(), result.end(),
		[this](Position pos) {return !isPositionFree(pos); });
	result.erase(iter, result.end());

	return result;
}

World::World(int worldX, int worldY)
{
	setWorldX(worldX);
	setWorldY(worldY);
}

int World::getWorldX()
{
	return this->worldX;
}

void World::setWorldX(int worldX)
{
	this->worldX = worldX;
}

int World::getWorldY()
{
	return this->worldY;
}

void World::setWorldY(int worldY)
{
	this->worldY = worldY;
}

int World::getTurn()
{
	return this->turn;
}

void World::addOrganism(Organism* organism)
{
	this->organisms.push_back(*organism);
}

void World::makeTurn()
{
	std::vector<Position> newPositions;
	size_t numberOfNewPositions;
	int randomIndex;

	srand(static_cast<unsigned int>(time(0)));
	for (auto& org : organisms) {
		newPositions = getVectorOfFreePositionsAround(org.getPosition());
		numberOfNewPositions = newPositions.size();
		if (numberOfNewPositions > 0) {
			randomIndex = rand() % numberOfNewPositions;
			org.setPosition(newPositions[randomIndex]);
		}
	}
	turn++;
}

void World::writeWorld(std::string fileName)
{
	std::fstream my_file;
	my_file.open(fileName, std::ios::out | std::ios::binary);
	if (my_file.is_open()) {
		my_file.write((char*)&this->worldX, sizeof(int));
		my_file.write((char*)&this->worldY, sizeof(int));
		my_file.write((char*)&this->turn, sizeof(int));
		size_t orgs_size = this->organisms.size();
		my_file.write((char*)&orgs_size, sizeof(int));
		for (size_t i = 0; i < orgs_size; i++) {
			int data;
			data = this->organisms[i].getPower();
			my_file.write((char*)&data, sizeof(int));
			data = this->organisms[i].getPosition().getX();
			my_file.write((char*)&data, sizeof(int));
			data = this->organisms[i].getPosition().getY();
			my_file.write((char*)&data, sizeof(int));
			char s_data = this->organisms[i].getSign();
			my_file.write((char*)&s_data, sizeof(char));
		}
		my_file.close();
	}
}

void World::readWorld(std::string fileName)
{
	std::fstream my_file;
	my_file.open(fileName, std::ios::in | std::ios::binary);
	if (my_file.is_open()) {
		int result;
		my_file.read((char*)&result, sizeof(int));
		this->worldX = (int)result;
		my_file.read((char*)&result, sizeof(int));
		this->worldY = (int)result;
		my_file.read((char*)&result, sizeof(int));
		this->turn = (int)result;
		my_file.read((char*)&result, sizeof(int));
		int orgs_size = (int)result;
		std::vector<Organism> new_organisms;
		for (int i = 0; i < orgs_size; i++) {
			int power;
			my_file.read((char*)&result, sizeof(int));
			power = (int)result;

			int pos_x;
			my_file.read((char*)&result, sizeof(int));
			pos_x = (int)result;
			int pos_y;
			my_file.read((char*)&result, sizeof(int));
			pos_y = (int)result;
			Position pos{ pos_x, pos_y };
			
			int s_size;
			my_file.read((char*)&result, sizeof(int));
			s_size = (int)result;

			std::string species;
			species.resize(s_size);
			my_file.read((char*)&species[0], s_size);
			
			Organism org(power, pos);
			org.setSign(species);
			new_organisms.push_back(org);
		}
		this->organisms = new_organisms;
		my_file.close();
	}
}

std::string World::toString()
{
	std::string result = "\nturn: " + std::to_string(getTurn()) + "\n";
	std::string spec;

	for (int wY = 0; wY < getWorldY(); ++wY) {
		for (int wX = 0; wX < getWorldX(); ++wX) {
			spec = getOrganismFromPosition(wX, wY);
			if (spec != "")
				result += spec;
			else
				result += separator;
		};
		result += "\n";
	}
	return result;
}

