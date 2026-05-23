#pragma once
#include "Organism.h"

class Plant : public Organism
{
	public:
		virtual ~Plant() = default;
		
		//Plant() : Organism(sign = 'P') {} //nie wiedzialem nawet ze mozna robic jak designated initialization
		Plant() : Organism(sign = 'P') {}
};

