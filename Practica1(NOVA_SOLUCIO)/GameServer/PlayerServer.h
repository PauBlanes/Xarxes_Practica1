#pragma once
#include <iostream>
#include "Card.h"
#include <vector>
#include <SFML\Network.hpp>

using namespace std;

struct PlayerServer {
	string name;
	int cardsInHand;
	vector<Card> myCards;
	TcpSocket* sock;
};