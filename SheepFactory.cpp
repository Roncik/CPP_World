#include "pch.h"
#include "SheepFactory.h"

std::unique_ptr<Organism> SheepFactory::create()
{
    return std::make_unique<Sheep>();
}
