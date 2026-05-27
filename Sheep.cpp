#include "pch.h"
#include "Sheep.h"
#include "ImGUIManager.h"

IDirect3DTexture9* Sheep::getTexture()
{
    return ImGUIManager::textures::Sheep;
}
