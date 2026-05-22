#include "pch.h"
#include "OrganismFactory.h"

std::unique_ptr<Organism> OrganismFactory::create()
{
    return std::make_unique<Organism>();
}
