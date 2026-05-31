#include "pch.h"
#include "Dandelion.h"
#include "ImGUIManager.h"

IDirect3DTexture9* Dandelion::getTexture()
{
    return ImGUIManager::textures::Dandelion;
}

std::string Dandelion::toString()
{
	using namespace std::string_literals;
	
	return	"{ species: " + "Dandelion"s +
			", power: " + std::to_string(getPower()) +
			", position: " + getPosition().toString() + "}";
}
