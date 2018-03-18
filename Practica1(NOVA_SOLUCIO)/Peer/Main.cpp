#include <SFML\Graphics.hpp>
#include <SFML\Network.hpp>
#include <string>
#include <cstring>
#include <iostream>
#include <list>
#include "GameEngine.h"

using namespace std;
using namespace sf;



#define OTHER_PLAYERS 1
void SendAllPlayers(string msg);
void ReceiveFromAll(std::vector<std::string>* aMensajes);
list <TcpSocket*> peers;

int main()
{
	//ESTABLIR CONNEXIÓ
	PeerLogic gameLogic;
	gameLogic.Start();

	//ENVIAR-REBRE NOMS

	//Posem a non-blocking tots els peers
	/*for (list<TcpSocket*>::iterator it = peers.begin(); it != peers.end(); ++it)
	{
		TcpSocket* clientRef = *it;
		clientRef->setBlocking(false);
		
	}

	//AQUI JA EL CHAT PROPIAMENT
	std::vector<std::string> aMensajes;

	sf::Vector2i screenDimensions(800, 600);

	sf::RenderWindow window;
	window.create(sf::VideoMode(screenDimensions.x, screenDimensions.y), "Chat");

	sf::Font font;
	if (!font.loadFromFile("courbd.ttf"))
	{
		std::cout << "Can't load the font file" << std::endl;
	}

	sf::String mensaje = " >";

	sf::Text chattingText(mensaje, font, 14);
	chattingText.setFillColor(sf::Color(0, 160, 0));
	chattingText.setStyle(sf::Text::Bold);


	sf::Text text(mensaje, font, 14);
	text.setFillColor(sf::Color(0, 160, 0));
	text.setStyle(sf::Text::Bold);
	text.setPosition(0, 560);

	sf::RectangleShape separator(sf::Vector2f(800, 5));
	separator.setFillColor(sf::Color(200, 200, 200, 255));
	separator.setPosition(0, 550);

	while (window.isOpen())
	{
		sf::Event evento;
		while (window.pollEvent(evento))
		{
			switch (evento.type)
			{
			case sf::Event::Closed:
				window.close();
				break;
			case sf::Event::KeyPressed:
				if (evento.key.code == sf::Keyboard::Escape)
					window.close();
				else if (evento.key.code == sf::Keyboard::Return)
				{
					aMensajes.push_back(mensaje);
					if (aMensajes.size() > 25)
					{
						aMensajes.erase(aMensajes.begin(), aMensajes.begin() + 1);
					}
					SendAllPlayers(mensaje);
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
		window.draw(separator);

		ReceiveFromAll(&aMensajes);

		for (size_t i = 0; i < aMensajes.size(); i++)
		{
			std::string chatting = aMensajes[i];
			chattingText.setPosition(sf::Vector2f(0, 20 * i));
			chattingText.setString(chatting);
			window.draw(chattingText);
		}
		std::string mensaje_ = mensaje + "_";
		text.setString(mensaje_);
		window.draw(text);


		window.display();
		window.clear();
	}*/
	system("pause");
	return 0;	
}

void SendAllPlayers(string msg) {
	for (list<TcpSocket*>::iterator it = peers.begin(); it != peers.end(); ++it)
	{
		TcpSocket& clientRef = **it;
		
		sf::Socket::Status status;
		string toSend = msg;
		size_t bytesSend;

		do
		{
			status = clientRef.send(toSend.c_str(), toSend.length() + 1, bytesSend);
			if (status == sf::Socket::Partial) {
				toSend = toSend.substr(bytesSend + 1, toSend.length() - bytesSend);
			}
		} while (status == sf::Socket::Partial);		
	}
}

void ReceiveFromAll(std::vector<std::string>* aMensajes) {
	
	for (list<TcpSocket*>::iterator it = peers.begin(); it != peers.end(); ++it)
	{
		TcpSocket& clientRef = **it;
		size_t received;
		sf::Socket::Status status;
		string tmp;
		char buffer[500];

		status = clientRef.receive(buffer, sizeof(buffer), received);

		switch (status)
		{
		case sf::Socket::Done:
			buffer[received] = '\0';
			tmp = buffer;
			aMensajes->push_back(tmp);
			break;

		case sf::Socket::Partial:
			break;
		case sf::Socket::Disconnected:
			tmp = "Disconnected";
			aMensajes->push_back(tmp);
			exit(0);
			break;
		case sf::Socket::Error:
			tmp = "Error to receive";
			aMensajes->push_back(tmp);
			break;
		default:
			break;
		}
	}

}