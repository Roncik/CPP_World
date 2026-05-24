#include "pch.h"
#include "DandelionFactory.h"

std::unique_ptr<Organism> DandelionFactory::create()
{
    return std::make_unique<Dandelion>();
}
