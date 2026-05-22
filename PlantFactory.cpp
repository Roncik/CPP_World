#include "pch.h"
#include "PlantFactory.h"

std::unique_ptr<Organism> PlantFactory::create()
{
    return std::make_unique<Plant>();
}
