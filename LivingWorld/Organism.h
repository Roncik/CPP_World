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
	//W jakim celu definiować konstruktory i destruktory skoro nie zarządzamy ręcznie żadnymi zasobami(gotowe kontenery robią to za nas)?
	//W tym przypadku ma zastosowanie zasada zera(Rule of zero) ponieważ nie zarządzamy ręcznie żadnymi zasobami na stercie.

	int getPower();
	void setPower(int power);
	Position getPosition();
	void setPosition(Position position);
	char getSign();
	void setSign(char spec);
	std::vector<std::pair<int, int>> getHistory();
	void setHistory(std::vector<std::pair<int, int>> newHistory);

	virtual std::string toString();

	virtual void move(int dx, int dy);

};