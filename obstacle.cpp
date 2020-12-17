#include <iostream>
#include "obstacle.h"
using namespace std;

Obstacle::Obstacle() {
	x = 0;
	y = 0;
	con = 0;
}

void Obstacle::setCon(int value) {
	con = value;
}

void Obstacle::setX(int value) {
	x = value;
}

void Obstacle::setY(int value) {
	y = value;
}

int Obstacle::getCon() { return con; }

int Obstacle::getX() { return x; }

int Obstacle::getY() { return y; }

void Obstacle::Ydown() { y--; }

void Obstacle::Yup() { y++; }