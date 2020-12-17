#pragma once
class User {
private:
	int x;
	int heart;
public:
	User();
	void setX(int value);
	int getX();
	void Xup();
	void Xdown();
	void initHeart();
	int getHeart();
	void heartDown();
};