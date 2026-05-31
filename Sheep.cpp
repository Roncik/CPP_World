#include "pch.h"
#include "Sheep.h"
#include "ImGUIManager.h"

IDirect3DTexture9* Sheep::getTexture()
{
    return ImGUIManager::textures::Sheep;
}

std::string Sheep::toString()
{
	using namespace std::string_literals;
	
	return	"{ species: " + "Sheep"s +
		", power: " + std::to_string(getPower()) +
		", position: " + getPosition().toString() + "}";
}
