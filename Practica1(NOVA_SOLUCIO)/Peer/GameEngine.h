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


using namespace std;
using namespace sf;

#define NUM_OTHER_PLAYERS 1

struct PeerInfo {
	string IP;
	unsigned int port;	
};


class PeerLogic {
private:
	vector<PlayerServer>others;
	PlayerServer me;

	vector<string> aMensajes;

	//Deck deck;
	//vector<Card> stack; //daqui anirem repartint als jugadors mentre en quedin
	//Card topCard; //la carta de la pila del mig	
	int turnIndex;	
	
	Clock clock;
	Time turnDuration;
	vector<thread> some_threads;
	mutex myMutex;

	bool gameEnded;
		

public:
	PeerLogic();
	void Start();
	void ComunicateNames();
	void ReceiveManager(PlayerServer* pS);
	void ReceiveFromAllManager();
	void gameLoop();
	void SendManager(string cmd);
	void SendManager(string cmd, string msg);
	void SendAllPlayers(Packet msg);
	bool EveryoneHasName();
	
};

