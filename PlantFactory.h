#pragma once
#include "OrganismFactory.h"
#include "Plant.h"

class PlantFactory : public OrganismFactory
{
public:
	virtual ~PlantFactory() = default;

	std::unique_ptr<Organism> create() override = 0;
};

