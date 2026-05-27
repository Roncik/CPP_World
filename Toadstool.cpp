#include "pch.h"
#include "Toadstool.h"
#include "ImGUIManager.h"

IDirect3DTexture9* Toadstool::getTexture()
{
    return ImGUIManager::textures::Toadstool;
}
