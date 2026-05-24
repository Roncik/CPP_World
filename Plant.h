#pragma once
#include "Organism.h"

// tutaj dziedziczenie zrobilem virtual zeby kazda klasa dziedziczaca po Plant byla odpowiedzialna za konstruowanie czesci Organism
// tzn. mogla wywolac konstruktor Organism w swoim konstruktorze
class Plant : public virtual Organism
{
	public:
		virtual ~Plant() = default;
	
		Plant() : Organism('P') {}
};

