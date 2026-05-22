#pragma once
#include "Organism.h"

class OrganismFactory
{
public:
	virtual ~OrganismFactory() = default;
	virtual std::unique_ptr<Organism> create();
};

