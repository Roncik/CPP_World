#include "pch.h"
#include "Position.h"
#include "Organism.h"
#include "Plant.h"
#include "Animal.h"
#include "World.h"
#include "Grass.h"
#include "Sheep.h"
#include "Dandelion.h"
#include "Wolf.h"
#include "Toadstool.h"

using org = std::unique_ptr<Organism>;

int main()
{
	
	World world;

	org grass = std::make_unique<Grass>();
	grass->setPosition(Position(0, 0));

	org sheep = std::make_unique<Sheep>();
	sheep->setPosition(Position(1, 1));

	org dandelion = std::make_unique<Dandelion>();
	dandelion->setPosition(Position(2, 2));

	org wolf = std::make_unique<Wolf>();
	wolf->setPosition(Position(3, 3));

	org toadstool = std::make_unique<Toadstool>();
	toadstool->setPosition(Position(4, 4));

	//world.addOrganism(grass);
	//world.addOrganism(sheep);
	world.addOrganism(dandelion);
	//world.addOrganism(wolf);
	//world.addOrganism(toadstool);

	world.writeWorld("world.bin");
	// Tura 0
	std::cout << world.toString() << '\n';

	// Tura 1
	world.makeTurn();
	std::cout << world.toString() << '\n';

	// Tura 2
	world.makeTurn();
	std::cout << world.toString() << '\n';

	// Tura 3
	world.makeTurn();
	std::cout << world.toString() << '\n';

	// Tura 4
	world.makeTurn();
	std::cout << world.toString() << '\n';

	// Tura 5
	world.makeTurn();
	std::cout << world.toString() << '\n';

	// Tura 6
	world.makeTurn();
	std::cout << world.toString() << '\n';

	// Tura 7
	world.makeTurn();
	std::cout << world.toString() << '\n';

	world.organisms.back()->serialize();
	
	
	
	//// Position 
	//Position p1;
	//Position p2{ 1, 1 };
	//Position p3{ -3, -5 };

	////cout << p1.toString() << endl;
	////cout << p2.toString() << endl;
	////cout << p3.toString() << endl;
	////cout << p1.distance(p2) << endl;
	////p2.move(4, 6);
	////cout << p2.toString() << endl;

	//// Organism
	////Organism org1;
	////Organism org2{ 10, p2 };

	////cout << org1.toString() << endl;
	////cout << org2.toString() << endl;
	////org1.move(2, 3);
	////cout << org1.toString() << endl;

	//// Plant & Animal
	//Plant plant{ 3, p3 };
	//Animal animal{ 5, p2 };
	//Plant plant2;
	//Animal animal2;

	//std::cout << plant.toString() << std::endl;
	//std::cout << animal.toString() << std::endl;
	//std::cout << plant2.toString() << std::endl;
	//std::cout << animal2.toString() << std::endl;
	//plant.move(3, 4);
	//std::cout << plant.toString() << std::endl;
	//animal.move(1, 2);
	//std::cout << animal.toString() << std::endl;
	//
	//// World test
	//World world;
	//Position posP1{ 4, 5 };
	//Plant plantW1{ 3, posP1 };
	//Position posP2{ 5, 4 };
	//Plant plantW2{ 3, posP2 };

	//Position posW2{ 3, 2 };
	//Animal animalW1{ 6, posW2 };
	//Position posW3{ 2, 3 };
	//Animal animalW2{ 6, posW3 };

	//world.addOrganism(&plantW1);
	//world.addOrganism(&plantW2);
	//world.addOrganism(&animalW1);
	//world.addOrganism(&animalW2);

	//auto positions = world.getVectorOfFreePositionsAround(Position(5, 5));

	//for(auto pos: positions)
	//	std::cout << pos.toString() << std::endl;

	//// Tura 0
	//std::cout << world.toString() << std::endl;

	//// Tura 1
	//world.makeTurn();
	//std::cout << world.toString() << std::endl;

	//// Tura 2
	//world.makeTurn();
	//std::cout << world.toString() << std::endl;

	//world.writeWorld("world.bin");

	//// Tura 3
	//world.makeTurn();
	//std::cout << world.toString() << std::endl;

	//// powrot do Tury 2
	//world.readWorld("world.bin");
	//std::cout << world.toString() << std::endl;

	return 0;
}
