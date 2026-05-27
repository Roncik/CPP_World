#pragma once
#include "Plant.h"

class Dandelion : public Plant
{
public:
	Dandelion() : Organism(0, 0, 6, 2, 'D') {}

	IDirect3DTexture9* getTexture() override;
};

