#include "pch.h"
#include "Animal.h"

bool Animal::getIsCarnivore()
{
	return this->_isCarnivore;
}

void Animal::setIsCarnivore(bool newIsCarnivore)
{
	this->_isCarnivore = newIsCarnivore;
}

std::string Animal::serialize()
{
	std::stringstream ss{};
	// tutaj trzeba uzyc write zeby zawsze zapisac odpowiednia ta sama ilosc bajtow - np worldX = 6 zapisze sie tylko jeden bajt przy <<

	//znak na poczatku zeby wiadomo bylo od razu jaki organizm trzeba stworzyc
	//sign
	ss.write((char*)&this->_sign, sizeof(this->_sign));

	//position
	Position pos = getPosition();
	ss.write((char*)&pos, sizeof(pos));

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
	ss.write((char*)&this->_isCarnivore, sizeof(bool));

	return ss.str();
}

