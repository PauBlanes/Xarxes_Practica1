#include <SFML\Graphics.hpp>
#include <SFML\Network.hpp>
#include <string>
#include <cstring>
#include <iostream>
#include <list>
#include "GameEngine.h"

using namespace std;
using namespace sf;




int main()
{
	//ESTABLIR CONNEXIÓ
	PeerLogic gameLogic;
	gameLogic.Start();
	gameLogic.ComunicateNames();
	gameLogic.gameLoop();	
	
	return 0;	
}

