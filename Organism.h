#pragma once
#include "Position.h"
//#include "ImGUIManager.h"

class Organism
{
private:
	Position _position;

protected:
	std::shared_ptr<std::pair<int, int>> _selfRecord;

	std::vector<std::shared_ptr<std::pair<int, int>>> _familyHistory{}; // historia to lista par; tur urodzin(1) i śmierci(2) przodków
	std::shared_ptr<std::vector<Organism*>> _family;

	// Parametry organizmu - ustawia je organizm indywidualnie
	int _power;
	int _initiative;
	int _liveLength;
	int _powerToReproduce;
	char _sign;
	bool _isAnimal{ false };
public:
	Organism(int power, int initiative, int liveLength, int powerToReproduce, char sign, Position position = Position(0, 0)) : _power{ power }, 
		_initiative{ initiative }, _liveLength{ liveLength }, _powerToReproduce{ powerToReproduce }, _sign{ sign }, _position{ position }
	{ 
		// jesli domyslne powerToReproduce - power jest wiekszy niz liveLength to organizm nigdy nie bedzie mogl sie reprodukowac
		assert(powerToReproduce - power <= liveLength && "niepoprawne parametry organizmu");

		initHistory();
		initFamily();
	}
	Organism(char sign) : Organism(0, 0, 0, 0, sign) {} //constructor delegation
	Organism() : Organism('O') {} //constructor delegation
	virtual ~Organism() = default;

	//W jakim celu definiować konstruktory i destruktory skoro nie zarządzamy ręcznie żadnymi zasobami(gotowe kontenery robią to za nas)?
	//W tym przypadku ma zastosowanie zasada zera(Rule of zero) ponieważ nie zarządzamy ręcznie żadnymi zasobami na stercie.

	Position getPosition();
	void setPosition(Position position);

	std::shared_ptr<std::pair<int, int>> getSelfRecord();
	void setSelfRecord(std::pair<int, int> selfRecord);

	void addFamilyRecord(const std::pair<int, int>& record);

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

	void initHistory(Organism* parent = nullptr, int turnNum = 0);
	void clearHistory();
	std::vector<std::shared_ptr<std::pair<int, int>>> getHistory();
	void setHistory(std::vector<std::shared_ptr<std::pair<int, int>>> newHistory);
	void logDeathTurn(int turnNum);

	void initFamily(Organism* parent = nullptr);
	bool isFamily(Organism* other);

	std::string printHistory();

	virtual std::string toString();

	virtual std::string serialize();

	virtual IDirect3DTexture9* getTexture();
};