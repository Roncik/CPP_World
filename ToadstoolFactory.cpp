#include "pch.h"
#include "ToadstoolFactory.h"
#include "Toadstool.h"

std::unique_ptr<Organism> ToadstoolFactory::create()
{
    return std::make_unique<Toadstool>();
}
