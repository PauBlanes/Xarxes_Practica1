#include "GameEngine.h"

PeerLogic::PeerLogic(){}

void PeerLogic::Start() {
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
	/*while (!EveryoneHasName()) {
		
	}*/
}
bool PeerLogic::EveryoneHasName() {
	for (int i = 0; i < others.size(); i++) {
		if (others[i].name == "")
			return false;
	}
	return true;
}