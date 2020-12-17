#include "user.h"
using namespace std;

User::User() {
	heart = 3;
}

void User::setX(int value) {
	x = value;
}

int User::getX() { return x; }

void User::Xup() { x++; }

void User::Xdown() { x--; }

void User::initHeart() { heart = 3; }

int User::getHeart() { return heart; }

void User::heartDown() { heart--; }