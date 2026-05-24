#pragma once
#include "Wolf.h"
#include "AnimalFactory.h"

class WolfFactory : public AnimalFactory
{
public:
	std::unique_ptr<Organism> create() override;
};

