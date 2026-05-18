#include "../pch.h"
#include "Animal.h"

Animal* Animal::reproduce(Position position)
{
	static_assert(false, "I should implement a factory for organisms instead of this");
	Animal* newAnimal = new Animal();
	newAnimal->setPosition(position);
	return newAnimal;
}

