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

using namespace std;
using namespace sf;

class ServerLogic {
private:
	vector<Card> stack; //daqui anirem repartint als jugadors mentre en quedin
	Card topCard; //la carta de la pila del mig
	list<TcpSocket*> clients;
	vector<PlayerServer>players;
	int maxPlayers;
	Deck deck;
public:
	ServerLogic();
	bool IsCardValid(Card cardToTest);
	void ServerManager(int _maxPlayers);
	void SendAllPlayers(string msg, TcpSocket* clientToExclude);
	void SendAllPlayers(Packet msg, TcpSocket* clientToExclude);
	void ComunicationManager(Packet receivedMsg, PlayerServer* pS);
	void SendCommand(string cmd, PlayerServer* pS);
	bool EveryoneHasName();
};