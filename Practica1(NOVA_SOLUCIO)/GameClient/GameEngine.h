#pragma once
#include <SFML\Graphics.hpp>
#include <SFML\Network.hpp>
#include <string>
#include <cstring>
#include <iostream>
#include "pokerImg.h"
#include "PlayerClient.h"
#include "Table.h"

struct position
{
	int x, y;
};
class GameEngine
{
public:
	GameEngine();
	~GameEngine();
	void start();
	sf::Socket::Status VSend(sf::TcpSocket* sock, string msg, string command);
	sf::Socket::Status VSend(sf::TcpSocket* sock, Packet toSend);
	void ReceiveAndManage(TcpSocket* sock);
	
	
private:
	pokerImg pocker;
	vector <position> playerPockerPos;
	vector <Sprite> playerPocker;

	vector <position> otherPlayerPockerPos;
	vector <Sprite> otherPlayerPocker;
	
	
	PlayerClient me;
	vector<PlayerClient> others;
	int otherPlayers;

	bool canStart;
	string winner;

	std::vector<std::string> aMensajes;

	Table gameTable;
};

