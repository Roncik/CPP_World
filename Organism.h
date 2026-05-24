#pragma once
#include "Position.h"

class Organism
{
private:
	Position position;

	static_assert(false, "jak zaimplementowac ta historie??");
	std::vector<std::pair<int, int>> family_history{}; // historia to lista par; tur urodzin(1) i śmierci(2) przodków

protected:
	// Parametry organizmu - ustawia je organizm indywidualnie
	int power;
	int initiative;
	int liveLength;
	int powerToReproduce;
	char sign;
	bool isAnimal{ false };

	//power = 3
	//powerToReproduce = 6
	//liveLength = 3

public:
	Organism(int power, int initiative, int liveLength, int powerToReproduce, char sign, Position position = Position(0, 0)) : power{ power }, 
		initiative{ initiative }, liveLength{ liveLength }, powerToReproduce{ powerToReproduce }, sign{ sign }, position{ position }
	{ 
		// jesli domyslne powerToReproduce - power jest wiekszy niz liveLength to organizm nigdy nie bedzie mogl sie reprodukowac
		assert(powerToReproduce - power <= liveLength && "niepoprawne parametry organizmu");
	}
	Organism(char sign) : Organism(0, 0, 0, 0, sign) {} //constructor delegation
	Organism() : Organism('O') {} //constructor delegation
	virtual ~Organism() = default;

	//W jakim celu definiować konstruktory i destruktory skoro nie zarządzamy ręcznie żadnymi zasobami(gotowe kontenery robią to za nas)?
	//W tym przypadku ma zastosowanie zasada zera(Rule of zero) ponieważ nie zarządzamy ręcznie żadnymi zasobami na stercie.

	Position getPosition();
	void setPosition(Position position);

	int getPower();
	void setPower(int power);

	int getInitiative();
	void setInitiative(int initiative);

	int getLiveLength();
	void setLiveLength(int liveLength);

	int getPowerToReproduce();
	void setPowerToReproduce(int powerToReproduce);
	
	char getSign();
	void setSign(char spec);

	bool getIsAnimal();
	void setIsAnimal(bool isAnimal);

	std::vector<std::pair<int, int>> getHistory();
	void setHistory(std::vector<std::pair<int, int>> newHistory);

	void move(int dx, int dy);

	virtual std::string toString();
};