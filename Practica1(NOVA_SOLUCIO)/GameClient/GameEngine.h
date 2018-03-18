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
	void ReceiveAndManage(TcpSocket* sock);
	void receiveText(sf::TcpSocket* sock, std::vector<std::string>* aMensajes);
	
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

	std::vector<std::string> aMensajes;

	Table gameTable;
};

