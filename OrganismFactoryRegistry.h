#pragma once
#include "OrganismFactory.h"

class OrganismFactoryRegistry
{
public:
	static std::unique_ptr<OrganismFactory> getFactory(char sign);
};

