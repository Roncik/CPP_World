#pragma once
#include "Organism.h"

class Animal : public Organism
{
public:
	Animal() = default;
	Animal* reproduce(Position position);
};

