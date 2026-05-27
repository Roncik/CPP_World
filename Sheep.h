#pragma once
#include "Animal.h"

class Sheep : public Animal
{
public:
	Sheep() : Organism(3, 3, 10, 6, 'S') {}

	IDirect3DTexture9* getTexture() override;
};
