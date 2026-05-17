#pragma once
#include "Organism.h"

class Plant : public Organism
{
	public:
		Plant() = default;
		Plant* reproduce(Position position);
};

