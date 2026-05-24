#pragma once
#include "PlantFactory.h"

class ToadstoolFactory : public PlantFactory
{
public:
	std::unique_ptr<Organism> create() override;
};

