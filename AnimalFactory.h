#pragma once
#include "OrganismFactory.h"
#include "Animal.h"

class AnimalFactory : public OrganismFactory
{
public:
	virtual ~AnimalFactory() = default;
	
	std::unique_ptr<Organism> create() override = 0;
};

