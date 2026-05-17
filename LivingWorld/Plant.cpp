#include "../pch.h"
#include "Plant.h"

Plant* Plant::reproduce(Position position)
{
	Plant* newPlant = new Plant();
	newPlant->setPosition(position);
	return newPlant;
}
