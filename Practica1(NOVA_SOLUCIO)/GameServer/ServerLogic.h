#pragma once
#include <SFML\Graphics.hpp>
#include <SFML\Network.hpp>
#include <string>
#include <cstring>
#include <iostream>
#include <list>
#include <vector>
#include "Card.h"
#include "PlayerServer.h"
#include "PlayerClient.h"
#include "Deck.h"
#include <thread>
#include <mutex>

using namespace std;
using namespace sf;

class ServerLogic {
private:
	bool running;
	vector<Card> stack; //daqui anirem repartint als jugadors mentre en quedin
	Card topCard; //la carta de la pila del mig
	list<TcpSocket*> clients;
	vector<PlayerServer>players;
	int turnIndex;
	int maxPlayers;
	Deck deck;

	Clock clock;
	Time turnDuration;
	vector<thread> some_threads;
	mutex myMutex;

	bool gameEnded;

public:
	ServerLogic();
	bool IsCardValid(Card cardToTest, PlayerServer* pS);
	void ServerManager(int _maxPlayers);
	void SendAllPlayers(string msg, TcpSocket* clientToExclude);
	void SendAllPlayers(Packet msg, TcpSocket* clientToExclude);
	void ComunicationManager(Packet receivedMsg, PlayerServer* pS);
	void SendCommand(string cmd, PlayerServer* pS);
	void PassTurn();
	void TurnTimer();
	void CreateThreads();
	bool EveryoneHasName();
};