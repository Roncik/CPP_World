#include "pch.h"
#include "World.h"
#include "OrganismFactoryRegistry.h"
#include "Animal.h"

bool World::getOrganismFromPosition(int x, int y, size_t* index)
{	
	for (size_t i = 0; i < organisms.size(); ++i)
	{
		auto& org = organisms[i];
		if (org->getPosition().getX() == x && org->getPosition().getY() == y)
		{
			if (index)
				*index = i;
			return true;
		}
	}
	return false;
}

bool World::isPositionOnWorld(int x, int y)
{
	return (x >= 0 && y >= 0 && x < getWorldX() && y < getWorldY());
}

bool World::isPositionFree(Position position) {
	return !this->getOrganismFromPosition(position.getX(), position.getY());
}

std::vector<Position> World::getVectorOfPositionsAround(Position position, bool onlyFreePositions)
{	
	int pos_x = position.getX(), pos_y = position.getY();
	std::vector<Position> result;
	for(int x = -1; x < 2; ++x)
		for (int y = -1; y < 2; ++y)
			if ((x != 0 || y != 0) && isPositionOnWorld(pos_x + x, pos_y + y)) 
			{
				result.push_back(Position(pos_x + x, pos_y + y));
			}

	if (onlyFreePositions)
	{
		auto iter = std::remove_if(result.begin(), result.end(), [this](Position pos) { return !isPositionFree(pos); });
		result.erase(iter, result.end());
	}

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

void World::addOrganism(std::unique_ptr<Organism>& organism)
{
	this->organisms.push_back(std::move(organism));
}

void World::removeOrganism(size_t index)
{
	this->organisms.erase(this->organisms.begin() + index);
}

void World::makeTurn()
{
	srand(static_cast<unsigned int>(time(0)));

	//sortowanie organizmów przez initiative malejąco, aby uzyskać odpowiednią kolejność w turach
	std::sort(organisms.begin(), organisms.end(), [](auto& first, auto& second) -> bool { return second->getInitiative() < first->getInitiative(); });

	//Remove organisms with no liveLength left loop
	for (size_t i = 0; i < organisms.size(); ++i)
	{
		auto& currentOrganism = organisms[i];

		if (!currentOrganism->getLiveLength())
		{
			removeOrganism(i);
			--i;
		}
	}

	//Increment power and decrement liveLength loop
	std::for_each(organisms.begin(), organisms.end(), [](auto& organism)
		{ 
			organism->setPower(organism->getPower() + 1);
			organism->setLiveLength(organism->getLiveLength() - 1);
		});

	//Move loop
	for (size_t i = 0; i < organisms.size(); ++i)
	{
		auto& org = organisms[i];
		
		bool isAnimal = org->getIsAnimal();
		bool isCarnivore{ false };
		if (isAnimal)
			isCarnivore = dynamic_cast<Animal*>(org.get())->getIsCarnivore(); //tutaj jest niebezpieczny downcast, mimo ze wiem ze obiekt jest Animal

		handleMove(i, isAnimal, isCarnivore);
	}

	//power loop(reproduction)
	for (auto& org : organisms)
	{
		if (org->getPower() >= org->getPowerToReproduce())
		{
			org->setPower(org->getPower() - org->getPowerToReproduce()); // zmiejsz power o zuzyte na reprodukcje
			
			
			std::vector<Position> possiblePositions = getVectorOfPositionsAround(org->getPosition(), true);
			size_t numberOfPositions = possiblePositions.size();
			if (numberOfPositions > 0)
			{
				int randomIndex = rand() % numberOfPositions;

				std::unique_ptr<Organism> newOrganism = OrganismFactoryRegistry::getFactory(org->getSign())->create();
				newOrganism->setPosition(possiblePositions[randomIndex]);

				addOrganism(newOrganism);
			}
		}
	}

	++turn;
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
			data = this->organisms[i]->getPower();
			my_file.write((char*)&data, sizeof(int));
			data = this->organisms[i]->getPosition().getX();
			my_file.write((char*)&data, sizeof(int));
			data = this->organisms[i]->getPosition().getY();
			my_file.write((char*)&data, sizeof(int));
			char s_data = this->organisms[i]->getSign();
			my_file.write((char*)&s_data, sizeof(char));
		}
		my_file.close();
	}
}

void World::readWorld(std::string fileName)
{
	assert(false && "need to fix this function");
	
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
			
			/*Organism org(power, pos);
			org.setSign(species);
			new_organisms.push_back(org);*/
		}
		//this->organisms = new_organisms;
		my_file.close();
	}
}

void World::handleMove(size_t& orgIndex, bool isAnimal, bool isCarnivore)
{
	if (!isAnimal)
		return;

	auto& org = organisms[orgIndex];

	/*
	- jesli animal i carnivore to moze zabic pobliski animal i wejsc na jego miejsce(power zwieksza sie o power zabitego animal)
	- jesli animal i herbivore to moze zjesc pobliski plant i wejsc na jego miejsce(power zwieksza sie o power zjedzonego plant)
	- jesli plant to nie przemieszcza sie
	- jesli animal nie ma gdzie sie przemiescic - umiera
	*/

	auto availablePositions = getVectorOfPositionsAround(org->getPosition());

	if (!availablePositions.size())
	{
		removeOrganism(orgIndex);
		--orgIndex;
		return;
	}

	Position& chosenPosition = availablePositions[rand() % availablePositions.size()];
	size_t nearbyOrganismId;

	if (!getOrganismFromPosition(chosenPosition.getX(), chosenPosition.getY(), &nearbyOrganismId))
	{
		org->setPosition(chosenPosition);
		return;
	}

	auto& nearbyOrganism = organisms[nearbyOrganismId];
	bool isNearbyOrganismAnimal = nearbyOrganism->getIsAnimal();

	if (!isNearbyOrganismAnimal)
	{
		// step over nearby plant and kill it
		if (!isCarnivore)
			org->setPower(org->getPower() + nearbyOrganism->getPower());

		removeOrganism(nearbyOrganismId);
		if (nearbyOrganismId < orgIndex) // jesli usuwany organizm mial indeks nizszy niz obecny, trzeba naprawic indeks
			--orgIndex;

		org->setPosition(chosenPosition);
		return;
	}

	// fight nearby animal
	if (org->getInitiative() > nearbyOrganism->getInitiative())
	{
		//scenario 1 - current animal has higher initiative than the attacked one - current animal wins
		if (isCarnivore)
			org->setPower(org->getPower() + nearbyOrganism->getPower());
		removeOrganism(nearbyOrganismId);
		if (nearbyOrganismId < orgIndex) // jesli usuwany organizm mial indeks nizszy niz obecny, trzeba naprawic indeks
			--orgIndex;
		org->setPosition(chosenPosition);
		return;
	}
	else if (org->getInitiative() == nearbyOrganism->getInitiative())
	{
		//scenario 2 - current animal has equal initiative to the attacked one - the result is randomized
		bool result = rand() % 2; // if result is true the current animal wins
		if (result)
		{
			if (isCarnivore)
				org->setPower(org->getPower() + nearbyOrganism->getPower());
			removeOrganism(nearbyOrganismId);
			if (nearbyOrganismId < orgIndex) // jesli usuwany organizm mial indeks nizszy niz obecny, trzeba naprawic indeks
				--orgIndex;
			org->setPosition(chosenPosition);
			return;
		}
		else
		{
			// if the nearby animal won, it should only get the power if it was a carnivore(herbivores can't eat other animals)
			bool isNearbyOrganismCarnivore = dynamic_cast<Animal*>(nearbyOrganism.get())->getIsCarnivore();
			if (isNearbyOrganismCarnivore)
				nearbyOrganism->setPower(nearbyOrganism->getPower() + org->getPower());
			removeOrganism(orgIndex);
			--orgIndex;
			return;
		}
	}
	else
	{
		throw std::logic_error("unhandled case");
	}
}

std::string World::toString()
{
	std::string result = "\nturn: " + std::to_string(getTurn()) + "\n";

	for (int wY = 0; wY < getWorldY(); ++wY) {
		for (int wX = 0; wX < getWorldX(); ++wX) {
			char sign{ 0 };
			size_t id{};
			if (getOrganismFromPosition(wX, wY, &id))
				sign = organisms[id]->getSign();
			if (sign) 
				result += sign;
			else
				result += separator;
		};
		result += "\n";
	}
	return result;
}

