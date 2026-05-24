#pragma once
#include "Animal.h"

class Wolf : public Animal
{
public:
	Wolf() : Organism(8, 5, 20, 16, 'W') { setIsCarnivore(true); }
};


