#pragma once
class Obstacle {
private:
	int x;
	int y;
	int con;

public:
	Obstacle();
	void setCon(int value);
	void setX(int value);
	void setY(int value);
	int getCon();
	int getX();
	int getY();
	void Ydown();
	void Yup();
};