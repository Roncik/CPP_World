#include "pch.h"
#include "Organism.h"

int Organism::getPower()
{
	return this->_power;
}

void Organism::setPower(int newPower)
{
	this->_power = newPower;
}

int Organism::getInitiative()
{
	return this->_initiative;
}

void Organism::setInitiative(int newInitiative)
{
	this->_initiative = newInitiative;
}

int Organism::getLiveLength()
{
	return this->_liveLength;
}

void Organism::setLiveLength(int newLiveLength)
{
	this->_liveLength = newLiveLength;
}

int Organism::getPowerToReproduce()
{
	return this->_powerToReproduce;
}

void Organism::setPowerToReproduce(int newPowerToReproduce)
{
	this->_powerToReproduce = newPowerToReproduce;
}

Position Organism::getPosition()
{
	return this->_position;
}

void Organism::setPosition(Position newPosition)
{
	this->_position = newPosition;
}

std::shared_ptr<std::pair<int, int>> Organism::getSelfRecord()
{
	return this->_selfRecord;
}

void Organism::setSelfRecord(std::pair<int, int> newSelfRecord)
{
	this->_selfRecord = std::make_shared<std::pair<int, int>>(newSelfRecord);
}

void Organism::addFamilyRecord(const std::pair<int, int>& record)
{
	_familyHistory.push_back(std::make_shared<std::pair<int, int>>(record));
}

std::string Organism::toString()
{
	return "{ species: " + std::to_string(this->getSign()) + 
		", power: " + std::to_string(getPower()) +
			", position: " + getPosition().toString() + "}";
}

char Organism::getSign()
{
	return this->_sign;
}

void Organism::setSign(char spec)
{
	this->_sign = spec;
}

bool Organism::getIsAnimal()
{
	return this->_isAnimal;
}

void Organism::setIsAnimal(bool newIsAnimal)
{
	this->_isAnimal = newIsAnimal;
}

void Organism::initHistory(Organism* parent, int turnNum)
{
	_selfRecord = std::make_shared<std::pair<int, int>>();
	_selfRecord->first = turnNum;
	_selfRecord->second = -1; // -1 = organizm jeszcze zyje

	if (parent)
		_familyHistory = parent->_familyHistory;

	_familyHistory.push_back(_selfRecord);
}

void Organism::clearHistory()
{
	this->_familyHistory.clear();
}

std::vector<std::shared_ptr<std::pair<int, int>>> Organism::getHistory()
{
	return this->_familyHistory;
}

void Organism::setHistory(std::vector<std::shared_ptr<std::pair<int, int>>> newHistory)
{
	this->_familyHistory = newHistory;
}

void Organism::logDeathTurn(int turnNum)
{
	_selfRecord->second = turnNum;
}

void Organism::initFamily(Organism* parent)
{
	if (parent)
		this->_family = parent->_family;
	else
		this->_family = std::make_shared<std::vector<Organism*>>();
	this->_family->push_back(this);
}

bool Organism::isFamily(Organism* other)
{
	return std::find(this->_family->begin(), this->_family->end(), other) != this->_family->end();
}

std::string Organism::printHistory()
{
	std::stringstream ss;

	ss << "History: \n";
	for (size_t i = 0; i < _familyHistory.size(); ++i)
	{
		auto& entry = _familyHistory[i];
		ss << '(' << entry->first << ", " << entry->second << ')' << ' ' << (i == (_familyHistory.size() - 1) ? "Current" : "Relative") << '\n';
	}

	return ss.str();
}

std::string Organism::serialize()
{
	std::stringstream ss{};
	// tutaj trzeba uzyc write zeby zawsze zapisac odpowiednia ta sama ilosc bajtow - np worldX = 6 zapisze sie tylko jeden bajt przy <<

	//znak na poczatku zeby wiadomo bylo od razu jaki organizm trzeba stworzyc
	//sign
	ss.write((char*)&this->_sign, sizeof(this->_sign));

	//position
	ss.write((char*)&this->_position, sizeof(this->_position));

	//selfRecord
	auto& selfrec = *_selfRecord.get();
	ss.write((char*)&selfrec, sizeof(selfrec));

	//familyHistory
	size_t size = _familyHistory.size();
	ss.write((char*)&size, sizeof(size));
	std::for_each(_familyHistory.begin(), _familyHistory.end(), [&](auto& cur)
		{
			auto& rec = *cur.get();
			ss.write((char*)&rec, sizeof(rec));
		});

	//power
	ss.write((char*)&this->_power, sizeof(this->_power));

	//initiative
	ss.write((char*)&this->_initiative, sizeof(this->_initiative));

	//liveLength
	ss.write((char*)&this->_liveLength, sizeof(this->_liveLength));

	//powerToReproduce
	ss.write((char*)&this->_powerToReproduce, sizeof(this->_powerToReproduce));

	//isAnimal
	ss.write((char*)&this->_isAnimal, sizeof(this->_isAnimal));

	//isCarnivore
	bool isCarnivore{ false };
	ss.write((char*)&isCarnivore, sizeof(isCarnivore));

	return ss.str();
}

IDirect3DTexture9* Organism::getTexture()
{
	return nullptr;
}
