#include "pch.h"
#include "Animal.h"

bool Animal::getIsCarnivore()
{
	return this->isCarnivore;
}

void Animal::setIsCarnivore(bool isCarnivore)
{
	this->isCarnivore = isCarnivore;
}
