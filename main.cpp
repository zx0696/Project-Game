#include <iostream>
#include "game.h"
using namespace std;

int main() {
	//game.run()����
	//system(" mode  con lines=30   cols=90 ");
	Game* game = new Game();
	game->run();
}
