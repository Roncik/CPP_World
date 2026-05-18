#include "../pch.h"
#include "Plant.h"

Plant* Plant::reproduce(Position position)
{
	static_assert(false, "I should implement a factory for organisms instead of this");
	Plant* newPlant = new Plant();
	newPlant->setPosition(position);
	return newPlant;
}
