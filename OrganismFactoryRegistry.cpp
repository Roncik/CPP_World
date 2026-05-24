#include "pch.h"
#include "OrganismFactoryRegistry.h"
#include "PlantFactory.h"
#include "AnimalFactory.h"
#include "GrassFactory.h"
#include "SheepFactory.h"
#include "DandelionFactory.h"
#include "WolfFactory.h"
#include "ToadstoolFactory.h"

std::unique_ptr<OrganismFactory> OrganismFactoryRegistry::getFactory(char sign)
{
    switch (sign)
    {
        case 'O': return std::make_unique<OrganismFactory>(); //Organism
        case 'P': return std::make_unique<PlantFactory>(); //Plant
        case 'A': return std::make_unique<AnimalFactory>(); //Animal

        case 'G': return std::make_unique<GrassFactory>(); //Grass

        case 'S': return std::make_unique<SheepFactory>(); //Sheep

        case 'D': return std::make_unique<DandelionFactory>(); //Dandelion

        case 'W': return std::make_unique<WolfFactory>(); //Wolf

        case 'T': return std::make_unique<ToadstoolFactory>(); //Toadstool

        default: throw std::logic_error("Factory not found"); return nullptr;
    }
}
