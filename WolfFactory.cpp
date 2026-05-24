#include "pch.h"
#include "WolfFactory.h"

std::unique_ptr<Organism> WolfFactory::create()
{
    return std::make_unique<Wolf>();
}
