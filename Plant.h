#pragma once
#include "Organism.h"

class Plant : public Organism
{
	public:
		virtual ~Plant() = default;
		
		//Plant() : Organism(sign = 'P') {} //nie wiedzialem nawet ze mozna robic jak designated initialization
		Plant() : Organism(power = 3, initiative = 1, liveLength = 3, powerToReproduce = 6, sign = 'P') {}
};

