#pragma once
#include "Dandelion.h"
#include "PlantFactory.h"

class DandelionFactory : public PlantFactory
{
public:
	std::unique_ptr<Organism> create() override;
};

