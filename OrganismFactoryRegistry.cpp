#include "pch.h"
#include "OrganismFactoryRegistry.h"
#include "PlantFactory.h"
#include "AnimalFactory.h"

std::unique_ptr<OrganismFactory> OrganismFactoryRegistry::getFactory(char sign)
{
    switch (sign)
    {
        case 'O': return std::make_unique<OrganismFactory>();
        case 'P': return std::make_unique<PlantFactory>();
        case 'A': return std::make_unique<AnimalFactory>();
        default: throw std::logic_error("Factory not found"); return nullptr;
    }
}
