#pragma once
#include "Organism.h"

class Animal : public Organism
{
protected:
	bool isCarnivore{ false };
public:
	virtual ~Animal() = default;
	
	Animal() : Organism(sign = 'A') { setIsAnimal(true); } //nie wiedzialem nawet ze mozna robic jak designated initialization

	bool getIsCarnivore();
	void setIsCarnivore(bool isCarnivore);
};

