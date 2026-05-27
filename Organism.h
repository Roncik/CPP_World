#pragma once
#include "Position.h"
//#include "ImGUIManager.h"

class Organism
{
private:
	Position position;

protected:
	std::shared_ptr<std::pair<int, int>> selfRecord;
	std::vector<std::shared_ptr<std::pair<int, int>>> familyHistory{}; // historia to lista par; tur urodzin(1) i śmierci(2) przodków

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

		initHistory();
	}
	Organism(char sign) : Organism(0, 0, 0, 0, sign) {} //constructor delegation
	Organism() : Organism('O') {} //constructor delegation
	virtual ~Organism() = default;

	//W jakim celu definiować konstruktory i destruktory skoro nie zarządzamy ręcznie żadnymi zasobami(gotowe kontenery robią to za nas)?
	//W tym przypadku ma zastosowanie zasada zera(Rule of zero) ponieważ nie zarządzamy ręcznie żadnymi zasobami na stercie.

	Position getPosition();
	void setPosition(Position position);

	std::pair<int, int> getSelfRecord();
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
	std::vector<std::shared_ptr<std::pair<int, int>>> getHistory();
	void setHistory(std::vector<std::shared_ptr<std::pair<int, int>>> newHistory);
	void logDeathTurn(int turnNum);

	virtual std::string toString();

	virtual std::string serialize();

	virtual IDirect3DTexture9* getTexture();
};


//#pragma once
//#include "Position.h"
//
//class Organism
//{
//private:
//	Position position;
//
//protected:
//	std::shared_ptr<std::pair<int, int>> selfRecord;
//	std::vector<std::shared_ptr<std::pair<int, int>>> familyHistory{}; // historia to lista par; tur urodzin(1) i śmierci(2) przodków
//
//	// Parametry organizmu - ustawia je organizm indywidualnie
//	int power;
//	int initiative;
//	int liveLength;
//	int powerToReproduce;
//	char sign;
//	bool isAnimal{ false };
//
//	//power = 3
//	//powerToReproduce = 6
//	//liveLength = 3
//
//public:
//	Organism(int power, int initiative, int liveLength, int powerToReproduce, char sign, Position position = Position(0, 0)) : power{ power },
//		initiative{ initiative }, liveLength{ liveLength }, powerToReproduce{ powerToReproduce }, sign{ sign }, position{ position }
//	{
//		// jesli domyslne powerToReproduce - power jest wiekszy niz liveLength to organizm nigdy nie bedzie mogl sie reprodukowac
//		assert(powerToReproduce - power <= liveLength && "niepoprawne parametry organizmu");
//
//		initHistory();
//	}
//	Organism(char sign) : Organism(0, 0, 0, 0, sign) {} //constructor delegation
//	Organism() : Organism('O') {} //constructor delegation
//	virtual ~Organism() = default;
//
//	//W jakim celu definiować konstruktory i destruktory skoro nie zarządzamy ręcznie żadnymi zasobami(gotowe kontenery robią to za nas)?
//	//W tym przypadku ma zastosowanie zasada zera(Rule of zero) ponieważ nie zarządzamy ręcznie żadnymi zasobami na stercie.
//
//	Position getPosition();
//	void setPosition(Position position);
//
//	std::pair<int, int> getSelfRecord();
//	void setSelfRecord(std::pair<int, int> selfRecord);
//
//	void addFamilyRecord(const std::pair<int, int>& record);
//
//	int getPower();
//	void setPower(int power);
//
//	int getInitiative();
//	void setInitiative(int initiative);
//
//	int getLiveLength();
//	void setLiveLength(int liveLength);
//
//	int getPowerToReproduce();
//	void setPowerToReproduce(int powerToReproduce);
//
//	char getSign();
//	void setSign(char spec);
//
//	bool getIsAnimal();
//	void setIsAnimal(bool isAnimal);
//
//	void initHistory(Organism* parent = nullptr, int turnNum = 0);
//	std::vector<std::shared_ptr<std::pair<int, int>>> getHistory();
//	void setHistory(std::vector<std::shared_ptr<std::pair<int, int>>> newHistory);
//	void logDeathTurn(int turnNum);
//
//	virtual std::string toString();
//
//	virtual std::string serialize();
//};