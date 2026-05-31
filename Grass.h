#pragma once
#include "Plant.h"

class Grass : public Plant
{
public:
	// Plant dziedziczy po Organism w trybie virtual, to oznacza ze Grass jest odpowiedzialny za skonstruowanie czesci Organism
	// w efekcie Grass moze wywolac konstruktor Organism
	Grass() : Organism(0, 0, 6, 3, 'G') {}

	IDirect3DTexture9* getTexture() override;

	std::string toString() override;
};

