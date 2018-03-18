#pragma once
#include <iostream>
#include <SFML\Network.hpp>

using namespace std;

struct PlayerClient {
	string name;
	int numCards = 5;
	bool myTurn;
};


