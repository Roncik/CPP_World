#include "pch.h"
#include "Wolf.h"
#include "ImGUIManager.h"

IDirect3DTexture9* Wolf::getTexture()
{
    return ImGUIManager::textures::Wolf;
}

std::string Wolf::toString()
{
	using namespace std::string_literals;
	
	return	"{ species: " + "Wolf"s +
		", power: " + std::to_string(getPower()) +
		", position: " + getPosition().toString() + "}";
}
