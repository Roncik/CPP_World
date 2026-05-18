#pragma once
#include "Position.h"

class Organism
{
private:
	Position position;
	std::vector<std::pair<int, int>> family_history{}; // historia to lista par; tur urodzin(1) i śmierci(2) przodków

protected:
	// Parametry organizmu - ustawia je organizm indywidualnie
	int power{ 0 };
	int initiative{ 0 };
	int liveLength{ 0 };
	int powerToReproduce{ 0 };
	char sign{ 'O' };

public:
	Organism() : position(0, 0) {};
	Organism(Organism* other);
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
	std::vector<std::pair<int, int>> getHistory();
	void setHistory(std::vector<std::pair<int, int>> newHistory);

	virtual std::string toString();

	void move(int dx, int dy);

	static_assert(false, "I should implement a factory for organisms instead of this");
	virtual Organism* reproduce(Position position);
};