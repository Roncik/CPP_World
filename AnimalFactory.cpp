#include "pch.h"
#include "AnimalFactory.h"

std::unique_ptr<Organism> AnimalFactory::create()
{
    return std::make_unique<Animal>();
}
