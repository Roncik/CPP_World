#include "pch.h"
#include "GrassFactory.h"

std::unique_ptr<Organism> GrassFactory::create()
{
    return std::make_unique<Grass>();
}
