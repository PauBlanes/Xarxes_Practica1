#pragma once
#include <SFML\Graphics.hpp>
#include <SFML\Network.hpp>
#include <string>
#include <cstring>
#include <iostream>
#include <list>
#include <thread>
#include <mutex>
#include "PlayerServer.h"
#include "Deck.h"

using namespace std;
using namespace sf;

#define NUM_OTHER_PLAYERS 1

struct PeerInfo {
	string IP;
	unsigned int port;
	string name;
};

class PeerLogic {
private:
	vector<PlayerServer>others;
	PlayerServer me;

	/*vector<Card> stack; //daqui anirem repartint als jugadors mentre en quedin
	Card topCard; //la carta de la pila del mig
	int turnIndex;	
	Deck deck;

	Clock clock;
	Time turnDuration;
	vector<thread> some_threads;
	mutex myMutex;

	bool gameEnded;*/

public:
	PeerLogic();
	void Start();
	void ComunicateNames();
	void ReceiveManager();
	void SendManager();
	bool EveryoneHasName();
};