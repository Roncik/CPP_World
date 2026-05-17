#include "../pch.h"
#include "Animal.h"

Animal* Animal::reproduce(Position position)
{
	Animal* newAnimal = new Animal();
	newAnimal->setPosition(position);
	return newAnimal;
}

