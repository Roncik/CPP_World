#include "pch.h"
#include "Toadstool.h"
#include "ImGUIManager.h"

IDirect3DTexture9* Toadstool::getTexture()
{
    return ImGUIManager::textures::Toadstool;
}

std::string Toadstool::toString()
{
	using namespace std::string_literals;
	
	return	"{ species: " + "Toadstool"s +
		", power: " + std::to_string(getPower()) +
		", position: " + getPosition().toString() + "}";
}
