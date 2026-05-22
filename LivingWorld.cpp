#include "pch.h"
#include "Position.h"
#include "Organism.h"
#include "Plant.h"
#include "Animal.h"
#include "World.h"

int main()
{
	World world;
	Plant plant1;
	plant1.setInitiative(5);
	Animal animal1;
	animal1.setInitiative(10);

	Plant plant2;
	plant2.setInitiative(6);
	Animal animal2;
	animal2.setInitiative(2);

	Plant plant3;
	plant3.setInitiative(4);
	Animal animal3;
	animal3.setInitiative(8);

	Plant plant4;
	plant4.setInitiative(1);


	Animal animal4;
	animal4.setInitiative(3);
	animal4.setPowerToReproduce(4);
	animal4.setPower(4);
	animal4.setLiveLength(7);


	world.addOrganism(&plant1);
	world.addOrganism(&animal1);
	world.addOrganism(&plant2);
	world.addOrganism(&animal2);
	world.addOrganism(&plant3);
	world.addOrganism(&animal3);
	world.addOrganism(&plant4);
	world.addOrganism(&animal4);

	world.makeTurn();
	
	
	
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
