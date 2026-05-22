#pragma once
#include "Organism.h"

class Animal : public Organism
{
public:
	virtual ~Animal() = default;
	
	Animal() : Organism(sign = 'A') {} //nie wiedzialem nawet ze mozna robic jak designated initialization
};

