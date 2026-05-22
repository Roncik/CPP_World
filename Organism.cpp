#include "pch.h"
#include "Organism.h"

int Organism::getPower()
{
	return this->power;
}

void Organism::setPower(int power)
{
	this->power = power;
}

int Organism::getInitiative()
{
	return this->initiative;
}

void Organism::setInitiative(int initiative)
{
	this->initiative = initiative;
}

int Organism::getLiveLength()
{
	return this->liveLength;
}

void Organism::setLiveLength(int liveLength)
{
	this->liveLength = liveLength;
}

int Organism::getPowerToReproduce()
{
	return this->powerToReproduce;
}

void Organism::setPowerToReproduce(int powerToReproduce)
{
	this->powerToReproduce = powerToReproduce;
}

Position Organism::getPosition()
{
	return this->position;
}

void Organism::setPosition(Position position)
{
	this->position = position;
}

std::string Organism::toString()
{
	return "{ species: " + std::to_string(this->getSign()) + 
		", power: " + std::to_string(getPower()) +
			", position: " + getPosition().toString() + "}";
}

void Organism::move(int dx, int dy)
{
	position.move(dx, dy);
}

char Organism::getSign()
{
	return this->sign;
}

void Organism::setSign(char spec)
{
	this->sign = spec;
}

std::vector<std::pair<int, int>> Organism::getHistory()
{
	return this->family_history;
}

void Organism::setHistory(std::vector<std::pair<int, int>> newHistory)
{
	this->family_history = newHistory;
}
