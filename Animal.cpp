#include "pch.h"
#include "Animal.h"

bool Animal::getIsCarnivore()
{
	return this->isCarnivore;
}

void Animal::setIsCarnivore(bool isCarnivore)
{
	this->isCarnivore = isCarnivore;
}

std::string Animal::serialize()
{
	std::stringstream ss;
	// tutaj trzeba uzyc write zeby zawsze zapisac odpowiednia ta sama ilosc bajtow - np worldX = 6 zapisze sie tylko jeden bajt przy <<

	//znak na poczatku zeby wiadomo bylo od razu jaki organizm trzeba stworzyc
	//sign
	ss.write((char*)&this->sign, sizeof(this->sign));

	//position
	Position pos = getPosition();
	ss.write((char*)&pos, sizeof(pos));

	//selfRecord
	auto& selfrec = *selfRecord.get();
	ss.write((char*)&selfrec, sizeof(selfrec));

	//familyHistory
	size_t size;
	ss.write((char*)&size, sizeof(size));
	std::for_each(familyHistory.begin(), familyHistory.end(), [&](auto& cur)
		{
			auto& rec = *cur.get();
			ss.write((char*)&rec, sizeof(rec));
		});

	//power
	ss.write((char*)&this->power, sizeof(this->power));

	//initiative
	ss.write((char*)&this->initiative, sizeof(this->initiative));

	//liveLength
	ss.write((char*)&this->liveLength, sizeof(this->liveLength));

	//powerToReproduce
	ss.write((char*)&this->powerToReproduce, sizeof(this->powerToReproduce));

	//isAnimal
	ss.write((char*)&this->isAnimal, sizeof(this->isAnimal));

	//isCarnivore
	ss.write((char*)&this->isCarnivore, sizeof(bool));

	return ss.str();
}