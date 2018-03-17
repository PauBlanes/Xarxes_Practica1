#include <SFML\Graphics.hpp>
#include <SFML\Network.hpp>
#include <string>
#include <cstring>
#include <iostream>
#include <list>
#include "ServerLogic.h"

using namespace std;
using namespace sf;


int main()
{	
	ServerLogic serverLogic;
	serverLogic.ServerManager(1);

	return 0;
}
