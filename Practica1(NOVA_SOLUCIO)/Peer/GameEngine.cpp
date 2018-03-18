#include "GameEngine.h"

PeerLogic::PeerLogic(){}

void PeerLogic::Start() {
	//Demanem el nom primer
	cout << "Escribe tu nick..." << endl;
	cin >> me.name;	
	
	//ESTABLIR CONNEXIÓ
	TcpSocket sock;
	IpAddress ip = IpAddress::getLocalAddress();
	sock.connect(ip, 5000);

	Packet packet;
	sock.receive(packet);
	unsigned short myPort = sock.getLocalPort();
	sock.disconnect();


	int currentPeers; string newIp; unsigned int newPort;
	packet >> currentPeers;

	cout << "COMENZANDO CONEXION CON LOS " << currentPeers << " PEERS ACTUALES..." << endl;
	for (int i = 0; i < currentPeers; i++) {
		packet >> newIp >> newPort;

		TcpSocket *newPeer = new TcpSocket;
		Socket::Status s = newPeer->connect(newIp, newPort); //teoricament el newPort esta b pero si el poso aqui no va
		if (s == Socket::Done) {
			PlayerServer newPlayer;
			newPlayer.sock = newPeer;

			//Ens esperem a que ens enviï el seu nom
			others.push_back(newPlayer);
			//peers.push_back(newPeer);
			cout << "conectado al peer con ip : " << newIp << " en su puerto : " << newPort << endl;
		}
		else {
			cout << "Error al conectar con peer con ip : " << newIp << " en su puerto " << newPort << endl;
			delete newPeer;
		}
	}
	//mentre no estiguem conectats a tots els altres peers fem aquesta rutina
	if (currentPeers < NUM_OTHER_PLAYERS ) {
		TcpListener listener;
		Socket::Status listStatus = listener.listen(myPort);
		if (listStatus != Socket::Done)
			cout << "port error" << endl;
		else
			cout << "port " << myPort << " ok" << endl;

		for (int i = 0; i < NUM_OTHER_PLAYERS - currentPeers; i++) {
			cout << "ESPERANDO NUEVOS JUGADORES..." << endl;
			TcpSocket *newPeer = new TcpSocket;
			Socket::Status s = listener.accept(*newPeer);
			if (s == Socket::Done) {
				PlayerServer newPlayer;
				newPlayer.sock = newPeer;
				others.push_back(newPlayer);
				//peers.push_back(newPeer);
				cout << "conectado al peer con ip : " << newPeer->getRemoteAddress() << endl;
			}
			else {
				cout << "error al aceptar nuevo peer" << endl;
				delete newPeer;
			}

		}
		listener.close();
	}

	cout << "TODOS LOS PEERS SE HAN CONECTADO CORRECTAMENTE!" << endl;	
}
void PeerLogic::ComunicateNames() {
	
	//Posem a non blocking
	for (int i = 0; i < others.size(); i++)
	{
		TcpSocket* clientRef = others[i].sock;
		clientRef->setBlocking(false);

	}
	SendManager("USERINFO"); //Enviem el nostre nom a tots i ens quedem esperant a rebre els dels altres
	while (!EveryoneHasName()) {
		for (int i = 0; i < others.size(); i++) {
			ReceiveManager(&others[i]);
		}
	}
	cout << "I know all the names" << endl;
}
bool PeerLogic::EveryoneHasName() {
	for (int i = 0; i < others.size(); i++) {
		if (others[i].name == "")
			return false;
	}
	return true;
}

void PeerLogic::SendManager(string cmd) {
	
	Packet pack2Send;
	pack2Send << cmd;
	
	if (cmd == "USERINFO") {
		pack2Send << me.name;
		SendAllPlayers(pack2Send);
	}
}
void PeerLogic::SendManager(string cmd, string msg) {

	Packet pack2Send;
	pack2Send << cmd;

	if (cmd == "MSG") {
		string finalMsg = me.name + msg;		
		pack2Send << finalMsg;
		SendAllPlayers(pack2Send);
	}
}
void PeerLogic::SendAllPlayers(Packet msg) {

	for (int i = 0; i < others.size(); i++) {
		TcpSocket& clientRef = *(others[i].sock);
		Socket::Status s;
		do
			s = clientRef.send(msg);
		while (s == Socket::Status::Partial);
	}
}
void PeerLogic::ReceiveManager(PlayerServer* pS) {
	
	Packet receivedPacket;
	pS->sock->receive(receivedPacket);

	string cmd;
	receivedPacket >> cmd;

	if (cmd == "USERINFO") {
		string n;
		receivedPacket >> n;
		pS->name = n;
	}
	else if (cmd == "MSG") {
		string receivedMsg;		
		receivedPacket >> receivedMsg;		
		aMensajes.push_back(receivedMsg);
	}
}
void PeerLogic::ReceiveFromAllManager() {
	for (int i = 0; i < others.size(); i++) {
		ReceiveManager(&others[i]);
	}
}
void PeerLogic::gameLoop() {
	
	sf::Vector2i screenDimensions(800, 600);

	sf::RenderWindow window;
	window.create(sf::VideoMode(screenDimensions.x, screenDimensions.y), me.name + ":Chat");

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
					aMensajes.push_back(me.name + mensaje);
					if (aMensajes.size() > 25)
					{
						aMensajes.erase(aMensajes.begin(), aMensajes.begin() + 1);
					}
					SendManager("MSG", mensaje);
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

		ReceiveFromAllManager();

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
	}
}