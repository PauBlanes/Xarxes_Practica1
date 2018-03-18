#include "GameEngine.h"


using namespace std;
using namespace sf;


GameEngine::GameEngine()
{	
	turnDuration = seconds(10);
}

GameEngine::~GameEngine()
{
}

void GameEngine::start() {

	cout << "Client" << endl;
	sf::IpAddress ip = sf::IpAddress::getLocalAddress();
	sf::TcpSocket socket;
	
	sf::Vector2i screenDimensions(800, 600);

	sf::RenderWindow gameWindow;
	sf::RenderWindow chatWindow;

	sf::Font font;	
			

	if (!font.loadFromFile("courbd.ttf"))
	{
		std::cout << "Can't load the font file" << std::endl;
	}

	sf::String mensaje = ">";

	sf::Text chattingText(mensaje, font, 14);
	chattingText.setFillColor(sf::Color(0, 160, 0));
	chattingText.setStyle(sf::Text::Bold);

	string turnIndicator = "IT'S YOUR TURN";
	Text turnIndicatorText(turnIndicator, font, 24);
	turnIndicatorText.setFillColor(sf::Color(0, 160, 0));
	turnIndicatorText.setStyle(sf::Text::Bold);
	turnIndicatorText.setPosition(Vector2f(300, 50));
	turnIndicatorText.setString(turnIndicator);

	Text turnTimerText(to_string(turnTimer), font, 24);
	turnTimerText.setFillColor(sf::Color(255, 255, 255));
	turnTimerText.setPosition(Vector2f(50, 50));
	turnTimerText.setString(to_string(turnTimer));
	
	Text winText(winner, font, 48);
	winText.setFillColor(sf::Color(255, 0, 0));
	winText.setStyle(sf::Text::Bold);
	winText.setPosition(Vector2f(100, 200));
	

	sf::Text text(mensaje, font, 14);


	text.setFillColor(sf::Color(0, 160, 0));
	text.setStyle(sf::Text::Bold);
	text.setPosition(0, 560);

	sf::RectangleShape separator(sf::Vector2f(800, 5));
	
	separator.setFillColor(sf::Color(200, 200, 200, 255));
	separator.setPosition(0, 550);
	
	Socket::Status status = socket.connect(ip, 50000);
	socket.setBlocking(false);


	if (status != Socket::Done) {
		cout << "Error de connexion al servidor\n";
		//exit(0);
	}
	else if (status == Socket::Done) {
		
		//Enviem el nostre nom
		cout << "Conected, Chose your name..." << endl;
		cin >> me.name;
		VSend(&socket, me.name, "USERINFO");
		
		//Anem rebent info dels altres fins que el server ens digui que podem començar			
		while (!canStart)
		{			
			ReceiveAndManage(&socket);
		}
	
		string intro = "Connected to: Servidor";
		aMensajes.push_back(intro);		

		gameWindow.create(sf::VideoMode(screenDimensions.x, screenDimensions.y), me.name + ":Game");
		chatWindow.create(sf::VideoMode(screenDimensions.x, screenDimensions.y), me.name + ":Chat");
	}
	while (gameWindow.isOpen())
	{		
		sf::Event evento;		
		if (me.myTurn) {
			//indiquem al jugador que es el seu torn				
			gameWindow.draw(turnIndicatorText);
			turnTimerText.setString(to_string(turnTimer));
			gameWindow.draw(turnTimerText);
		}		

		//GAMEPLAY
		while (gameWindow.pollEvent(evento))
		{
			if (me.myTurn) { //si es el nostre torn detectem eventos
				switch (evento.type)
				{
				case::sf::Event::MouseButtonPressed:
					for each (Card c in gameTable.myCards)
					{
						if (c.isClicked(&gameWindow)) {
							Packet temp;							
							temp << "CHECKCARD" << c.number << (int)c.color;
							VSend(&socket, temp);						
						}
					}
					break;
				case sf::Event::Closed:
					gameWindow.close();
					chatWindow.close();
					break;
				case::Event::KeyPressed:
					if (evento.key.code == Keyboard::P) {
						Packet temp; temp << "PASS";
						VSend(&socket, temp);						
					}
				}				
			}			
		}		
		
		//Pintem la les cartes de la taula
		gameTable.DrawTable(&gameWindow);

		if (winner != "") {
			winText.setString(winner);
			gameWindow.draw(winText);
		}

		gameWindow.display();
		gameWindow.clear();

		//CHAT
		if (aMensajes.size() >= 25) aMensajes.clear();
		
		while (chatWindow.pollEvent(evento))
		{
			switch (evento.type)
			{
			case sf::Event::Closed:
				chatWindow.close();
				break;
			case sf::Event::KeyPressed:
				if (evento.key.code == sf::Keyboard::Escape)
					chatWindow.close();
				else if (evento.key.code == sf::Keyboard::Return)
				{

					//	SEND
					//Enviem al server					
					if (VSend(&socket, mensaje, "MSG") != Socket::Status::Done) {
						aMensajes.push_back("there is no connection");
					}

					if (aMensajes.size() > 25)
					{
						aMensajes.erase(aMensajes.begin(), aMensajes.begin() + 1);
					}
					mensaje = ">";
				}
				break;
			case sf::Event::TextEntered:
				if (evento.text.unicode >= 32 && evento.text.unicode <= 126)
					mensaje += (char)evento.text.unicode;
				else if (evento.text.unicode == 8 && mensaje.getSize() > 0)
					mensaje.erase(mensaje.getSize() - 1, mensaje.getSize());
				break;
			}
		}
		chatWindow.draw(separator);		
		ReceiveAndManage(&socket);

		for (size_t i = 0; i < aMensajes.size(); i++)
		{
			std::string chatting = aMensajes[i];
			chattingText.setPosition(sf::Vector2f(0, 20 * i));
			chattingText.setString(chatting);
			chatWindow.draw(chattingText);
		}
		std::string mensaje_ = mensaje + "_";
		text.setString(mensaje_);
		chatWindow.draw(text);

		chatWindow.display();
		chatWindow.clear();
	}
	
	
	socket.disconnect();
}

Socket::Status GameEngine::VSend(sf::TcpSocket* sock, string msg, string command) {

	sf::Socket::Status status;
	string toSend;
	size_t bytesSend;
	Packet packet2Send;
	
	if (command == "")
		packet2Send << toSend;
	else {
		if (command == "MSG") {
			toSend = me.name + msg;
		}		
		else
			toSend = msg;
		packet2Send << command << toSend;
	}
		

	do
	{		
		status = sock->send(packet2Send);		
	} while (status == sf::Socket::Partial);
	return status;
}
Socket::Status GameEngine::VSend(sf::TcpSocket* sock, Packet toSend) {
	
	sf::Socket::Status status;
	do
	{
		status = sock->send(toSend);
	} while (status == sf::Socket::Partial);

	return status;
}

void GameEngine::ReceiveAndManage(TcpSocket* sock) {
	
	sf::Socket::Status status;
	Packet receivedPacket;
	status = sock->receive(receivedPacket);
	string comand;

	switch (status)
	{
	case sf::Socket::Done:
		receivedPacket >> comand;

		if (comand == "PLAYERNAME") {
			string newName;
			receivedPacket >> newName;
			PlayerClient newPlayer;
			newPlayer.name = newName;
			others.push_back(newPlayer);
		}		
		else if (comand == "FILLCARDS") {
			if (!canStart)
				canStart = true;
			gameTable.FillCards(receivedPacket);
		}		
		else if (comand == "UPDATESTACK") {			
			gameTable.UpdateStack(receivedPacket);
		}
		else if (comand == "WIN") {
			string winName;
			receivedPacket >> winName;
			winner = "THE WINNER IS : " + winName;
			cout << winner << endl;
			me.myTurn = false;					
		}
		else if (comand == "START_TURN") {
			string who;
			receivedPacket >> who;
			//MIREM DE QUI ES EL TORN
			if (who == me.name) {
				me.myTurn = true;
				clock.restart();
				this->CreateThreads(); //activem el contador		
				cout << "It's your turn" << endl;
			}
			else if (me.myTurn == true) {
				me.myTurn = false;				
				cout << "end of turn" << endl;
			}
		}
		else if (comand == "ENEMYCARDS") {}
		else if (comand == "MSG") {
			string msg;
			receivedPacket >> msg;
			aMensajes.push_back(msg);
		}
		break;
	case sf::Socket::NotReady:
		break;
	case sf::Socket::Partial:
		break;
	case sf::Socket::Disconnected:
		cout << "SERVER DISCONNECTION" << endl;
		break;
	case sf::Socket::Error:
		cout << "RECEIVE ERROR" << endl;
		break;
	default:
		break;
	}
	
	
}
void GameEngine::TurnTimer() {
	while (me.myTurn) {
		Time elapsed = clock.getElapsedTime();

		turnTimer = turnDuration.asSeconds() - elapsed.asSeconds();

		if (elapsed >= turnDuration) {
			me.myTurn = false;	
			//el propio server ya nos quitara el control no necesitamos hacer más aqui
		}
	}
}
void GameEngine::CreateThreads() {
	some_threads.push_back(thread(&GameEngine::TurnTimer, this));

}