#include "pch.h"
#include "Grass.h"
#include "ImGUIManager.h"

IDirect3DTexture9* Grass::getTexture()
{
    return ImGUIManager::textures::Grass;
}
