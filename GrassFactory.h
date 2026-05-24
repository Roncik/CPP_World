#pragma once
#include "Grass.h"
#include "PlantFactory.h"

class GrassFactory : public PlantFactory
{
public:
	std::unique_ptr<Organism> create() override;
};
