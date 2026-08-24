#include "pch.h"
#include "World.h"
#include "OrganismFactoryRegistry.h"
#include "Animal.h"
#include "Plant.h"
#include "Grass.h"

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
	return this->_worldX;
}

void World::setWorldX(int newWorldX)
{
	this->_worldX = newWorldX;
}

int World::getWorldY()
{
	return this->_worldY;
}

void World::setWorldY(int newWorldY)
{
	this->_worldY = newWorldY;
}

int World::getTurn()
{
	return this->_turn;
}

void World::addOrganism(std::unique_ptr<Organism>& organism)
{
	this->organisms.push_back(std::move(organism));
}

const std::vector<std::unique_ptr<Organism>>& World::getOrganisms()
{
	return this->organisms;
}

void World::removeOrganism(size_t index)
{
	this->organisms[index]->logDeathTurn(getTurn());
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
	for (size_t i = 0; i < organisms.size(); ++i)
	{
		auto& org = organisms[i];
		
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
				newOrganism->initHistory(organisms[i].get(), getTurn() + 1); // tutaj numerTury + 1 bo licznik jest incrementowany na końcu makeTurn
				newOrganism->initFamily(organisms[i].get());

				addOrganism(newOrganism);
			}
		}
	}

	++_turn;
}

std::string World::serialize()
{
	std::stringstream ss{};
	// tutaj trzeba uzyc write zeby zawsze zapisac odpowiednia ta sama ilosc bajtow - np worldX = 6 zapisze sie tylko jeden bajt przy <<
	ss.write((char*)&this->_worldX, sizeof(this->_worldX));
	ss.write((char*)&this->_worldY, sizeof(this->_worldY));
	ss.write((char*)&this->_turn, sizeof(this->_turn));

	auto organisms_size = this->organisms.size();
	ss.write((char*)&organisms_size, sizeof(organisms_size));

	return ss.str();
}

void World::writeWorld(std::string fileName)
{
	std::fstream my_file;
	my_file.open(fileName, std::ios::out | std::ios::binary);
	if (!my_file.is_open())
		return;

	std::string data = this->serialize(); // world serializes itself
	std::for_each(organisms.begin(), organisms.end(), [&](auto& cur)
		{
			auto& org = *cur.get();
			data += org.serialize(); // all organisms serialize themselves
		});

	my_file.write(data.c_str(), data.size());
	my_file.close();
}

void World::readWorld(std::string fileName)
{
	std::fstream my_file;
	my_file.open(fileName, std::ios::in | std::ios::binary);
	if (!my_file.is_open())
	{
		throw std::runtime_error("couldn't open file for import");
	}

	this->clear();

	/*
	struktura serializacji:
	worldX
	worldY
	turn
	organisms_size
	***sign
	***position
	***selfRecord
	***familyHistory
	***power
	***initiative
	***liveLength
	***powerToReproduce
	***isAnimal
	***isCarnivore
	***id
	*/

	// worldX
	my_file.read((char*)&_worldX, sizeof(_worldX));

	// worldY
	my_file.read((char*)&_worldY, sizeof(_worldY));

	// turn
	my_file.read((char*)&_turn, sizeof(_turn));

	// organisms
	unsigned int max_index{ 0 };
	size_t organisms_size;
	my_file.read((char*)&organisms_size, sizeof(organisms_size));

	auto orgInstance = Grass();
	for (size_t i = 0; i < organisms_size; ++i)
	{
		// sign
		decltype(orgInstance.getSign()) sign; // trik zeby auto sign zawsze byl tym samym typem co w Organism
		my_file.read((char*)&sign, sizeof(sign));
		std::unique_ptr<Organism> newOrganism = OrganismFactoryRegistry::getFactory(sign)->create();
		newOrganism->setSign(sign);

		// position
		decltype(orgInstance.getPosition()) position;
		my_file.read((char*)&position, sizeof(position));
		newOrganism->setPosition(position);

		// selfRecord
		auto selfRecord = *orgInstance.getSelfRecord().get();
		my_file.read((char*)&selfRecord, sizeof(selfRecord));
		newOrganism->setSelfRecord(selfRecord);

		// familyHistory
		decltype(this->organisms.size()) size{};
		my_file.read((char*)&size, sizeof(size));
		newOrganism->clearHistory();
		while (size--)
		{
			auto record = *orgInstance.getSelfRecord().get();
			my_file.read((char*)&record, sizeof(record));
			newOrganism->addFamilyRecord(record);
		}

		// power
		decltype(orgInstance.getPower()) power;
		my_file.read((char*)&power, sizeof(power));
		newOrganism->setPower(power);

		// initiative
		decltype(orgInstance.getInitiative()) initiative;
		my_file.read((char*)&initiative, sizeof(initiative));
		newOrganism->setInitiative(initiative);

		// liveLength
		decltype(orgInstance.getLiveLength()) liveLength;
		my_file.read((char*)&liveLength, sizeof(liveLength));
		newOrganism->setLiveLength(liveLength);

		// powerToReproduce
		decltype(orgInstance.getPowerToReproduce()) powerToReproduce;
		my_file.read((char*)&powerToReproduce, sizeof(powerToReproduce));
		newOrganism->setPowerToReproduce(powerToReproduce);

		// isAnimal
		decltype(orgInstance.getIsAnimal()) isAnimal;
		my_file.read((char*)&isAnimal, sizeof(isAnimal));
		newOrganism->setIsAnimal(isAnimal);

		// isCarnivore
		if (isAnimal)
		{
			Animal* animal = dynamic_cast<Animal*>(newOrganism.get());
			decltype(animal->getIsCarnivore()) isCarnivore;
			my_file.read((char*)&isCarnivore, sizeof(isCarnivore));
			animal->setIsCarnivore(isCarnivore);
		}
		else
		{
			// i tak musimy odczytac bool zeby przesunac 'kursor odczytu' dalej
			decltype(orgInstance.getIsAnimal()) dummy;
			my_file.read((char*)&dummy, sizeof(dummy));
		}

		//id
		decltype(orgInstance.getId()) id;
		my_file.read((char*)&id, sizeof(id));
		newOrganism->setId(id);
		if (id > max_index)
			max_index = id;
		else
		{
			// i tak musimy odczytac bool zeby przesunac 'kursor odczytu' dalej
			auto dummy = Organism().getIsAnimal();
			my_file.read((char*)&dummy, sizeof(dummy));
		}

		addOrganism(newOrganism);
	}

	//set global index counter
	Organism::setGlobalIdCounter(max_index + 1);


	my_file.close();
	
}

void World::handleMove(size_t& orgIndex, bool isAnimal, bool isCarnivore)
{
	if (!isAnimal) // jesli roslina - nie rusza sie
		return;

	auto removeOrganismFixIndex = [&](size_t index) -> void
		{
			removeOrganism(index);
			if (index <= orgIndex) // jesli usuwany organizm mial indeks nizszy lub rowny obecnemu, trzeba naprawic indeks
				--orgIndex;
		};

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
		removeOrganismFixIndex(orgIndex);
		return;
	}

	Position& chosenPosition = availablePositions[rand() % availablePositions.size()];
	size_t nearbyOrganismId{ (0) };

	if (!getOrganismFromPosition(chosenPosition.getX(), chosenPosition.getY(), &nearbyOrganismId))
	{
		org->setPosition(chosenPosition);
		return;
	}

	assert(nearbyOrganismId != orgIndex);

	auto& nearbyOrganism = organisms[nearbyOrganismId];
	bool isNearbyOrganismAnimal = nearbyOrganism->getIsAnimal();

	if (!isNearbyOrganismAnimal)
	{
		if (nearbyOrganism->getSign() == 'T') // jesli plant to muchomor, animal i muchomor umiera
		{
			removeOrganismFixIndex(orgIndex);
			removeOrganismFixIndex(nearbyOrganismId);
			return;
		}

		// step over nearby plant and kill it
		if (!isCarnivore)
			org->setPower(org->getPower() + nearbyOrganism->getPower());

		removeOrganismFixIndex(nearbyOrganismId);
		org->setPosition(chosenPosition);
		return;
	}
	else if (org->isFamily(nearbyOrganism->getId()))
	{
		// Relatives don't attack each other
		
		return;
	}

	// fight nearby animal
	if (org->getInitiative() > nearbyOrganism->getInitiative())
	{
		//scenario 1 - current animal has higher initiative than the attacked one - current animal wins
		if (isCarnivore)
			org->setPower(org->getPower() + nearbyOrganism->getPower());

		removeOrganismFixIndex(nearbyOrganismId);

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

			org->setPosition(chosenPosition);
			removeOrganismFixIndex(nearbyOrganismId);

			org->setPosition(chosenPosition);
			return;
		}
		else
		{
			// if the nearby animal won, it should only get the power if it was a carnivore(herbivores can't eat other animals)
			bool isNearbyOrganismCarnivore = dynamic_cast<Animal*>(nearbyOrganism.get())->getIsCarnivore();
			if (isNearbyOrganismCarnivore)
				nearbyOrganism->setPower(nearbyOrganism->getPower() + org->getPower());
			
			removeOrganismFixIndex(orgIndex);

			return;
		}
	}
	else if (org->getInitiative() < nearbyOrganism->getInitiative())
	{
		//scenario 3 - current animal has smaller initiative to the attacked one - current animal dies
		
		bool isNearbyOrganismCarnivore = dynamic_cast<Animal*>(nearbyOrganism.get())->getIsCarnivore();
		if (isNearbyOrganismCarnivore)
			nearbyOrganism->setPower(nearbyOrganism->getPower() + org->getPower());

		removeOrganismFixIndex(orgIndex);
		return;
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

void World::Randomize(int worldX, int worldY, size_t numOfOrganisms, const std::vector<char>& possibleOrganismsSigns)
{
	if (!worldX || !worldY)
		return;
	
	if (numOfOrganisms > (worldX * worldY))
		throw std::logic_error("The chosen number of organisms couldn't fit in a world of this size");

	srand(static_cast<unsigned int>(time(0)));
	
	this->clear();

	setWorldX(worldX);
	setWorldY(worldY);

	for (size_t i = 0; i < numOfOrganisms; ++i)
	{
		const char& randomSign = possibleOrganismsSigns[rand() % possibleOrganismsSigns.size()];

		auto org = OrganismFactoryRegistry::getFactory(randomSign)->create();
		Position randPosition;
		do
		{
			randPosition = Position(rand() % worldX, rand() % worldY);
		} 
		while (this->getOrganismFromPosition(randPosition.getX(), randPosition.getY()));


		org->setPosition(randPosition);
		addOrganism(org);
	}
}

void World::clear()
{
	this->_worldX = 0;
	this->_worldY = 0;
	this->_turn = 0;
	this->organisms.clear();
	Organism::setGlobalIdCounter(0);
}
