#pragma once
#include "Organism.h"

// tutaj dziedziczenie zrobilem virtual zeby kazda klasa dziedziczaca po Animal byla odpowiedzialna za konstruowanie czesci Organism
// tzn. mogla wywolac konstruktor Organism w swoim konstruktorze
class Animal : public virtual Organism
{
protected:
	bool isCarnivore{ false };
public:
	virtual ~Animal() = default;
	
	Animal() : Organism('A') { setIsAnimal(true); } 

	bool getIsCarnivore();
	void setIsCarnivore(bool isCarnivore);

	std::string serialize();
};

