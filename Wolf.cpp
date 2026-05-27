#include "pch.h"
#include "Wolf.h"
#include "ImGUIManager.h"

IDirect3DTexture9* Wolf::getTexture()
{
    return ImGUIManager::textures::Wolf;
}
