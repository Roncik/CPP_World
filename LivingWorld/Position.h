#pragma once

class Position {
private:
	int x;
	int y;

public:
	Position(int x, int y);
	Position() : x(0), y(0) {}

	int getX();
	void setX(int x);
	int getY();
	void setY(int y);

	std::string toString();

	double distance(Position position);
	void move(int dx, int dy);
};
