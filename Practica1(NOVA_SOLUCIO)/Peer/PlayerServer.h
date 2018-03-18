#pragma once
#include <iostream>
#include "Card.h"
#include <vector>
#include <SFML\Network.hpp>

using namespace std;

struct PlayerServer {
	string name;	
	vector<Card> myCards;
	TcpSocket* sock;
	bool myTurn;
};