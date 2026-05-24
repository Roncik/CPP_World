#pragma once
#include "Sheep.h"
#include "AnimalFactory.h"

class SheepFactory : public AnimalFactory
{
public:
	std::unique_ptr<Organism> create() override;
};

