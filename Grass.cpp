#include "pch.h"
#include "Grass.h"
#include "ImGUIManager.h"

IDirect3DTexture9* Grass::getTexture()
{
    return ImGUIManager::textures::Grass;
}

std::string Grass::toString()
{
	using namespace std::string_literals;

	return	"{ species: " + "Grass"s +
		", power: " + std::to_string(getPower()) +
		", position: " + getPosition().toString() + "}";
}
