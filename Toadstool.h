#pragma once
#include "Plant.h"

class Toadstool : public Plant
{
public:
	Toadstool() : Organism(0, 0, 12, 4, 'T') {}

	IDirect3DTexture9* getTexture() override;

	std::string toString() override;
};

