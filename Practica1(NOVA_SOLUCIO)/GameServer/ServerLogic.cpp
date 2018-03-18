#include "ServerLogic.h"

ServerLogic::ServerLogic(){
	//Agafem la primera carta de la pila per posar-la al mig
	topCard.SetCard(deck.stack[0].number, deck.stack[0].color);
	deck.stack.erase(deck.stack.begin());

	//Establim quan duren els torns
	turnDuration = seconds(3);

	running = true;
}
bool ServerLogic::IsCardValid(Card cardToTest) {
	if (topCard.color == cardToTest.color && topCard.number == cardToTest.number) // si es el mismo numero y color es valido
		return true;
	if (topCard.color != cardToTest.color && ( (cardToTest.number == (topCard.number + 1)) || (cardToTest.number == 2 && topCard.number == 11) ) ) //si es más alta y de color diferente
		return true;
	else
		return false;
}
void ServerLogic::ServerManager(int _maxPlayers)
{	
	// Create a socket to listen to new connections
	TcpListener listener;
	Socket::Status status = listener.listen(50000);
	if (status != Socket::Done)
	{
		cout << "Error al abrir listener\n";
		exit(0);
	}
	else
		cout << "Port ok" << endl;

	// Create a selector
	SocketSelector selector;
	// Add the listener to the selector
	selector.add(listener);

	//Define the number of players you want
	maxPlayers = _maxPlayers;

	// Endless loop that waits for new connections
	while (running)
	{
		// Make the selector wait for data on any socket
		if (selector.wait())
		{
			// Test the listener
			if (selector.isReady(listener))
			{
				// The listener is ready: there is a pending connection
				TcpSocket* client = new TcpSocket;
				if (listener.accept(*client) == Socket::Done)
				{
					// Add the new client to the clients list
					std::cout << "Llega el cliente con puerto: " << client->getRemotePort() << std::endl;
					PlayerServer newPlayer;
					newPlayer.sock = client;
					
					//Ens esperem a que ens enviï el seu nom
					players.push_back(newPlayer);
					
					// Add the new client to the selector so that we will be notified when he sends something
					selector.add(*client);				

				}
				else
				{
					// Error, we won't get a new connection, delete the socket
					cout << "Error al recoger conexion nueva\n";
					delete client;
				}
			}
			else
			{				
				//Ningú s'ha intentat connectar, per tant fem la comunicacio amb els actuals			
				for (int i = 0; i < players.size(); i++)
				{
					bool shouldErase = false;
					TcpSocket& client = *(players[i].sock);
					if (selector.isReady(client))
					{
						// The client has sent some data, we can receive i												
						string strRec;
						Packet packet;
						players[i].sock->receive(packet);
						if (status == Socket::Done) //Aquesta funcio ja comprovara si estan tots
						{
							ComunicationManager(packet, &players[i]);
							
						}
						else if (status == Socket::Disconnected)
						{
							selector.remove(client);
							shouldErase = true;
							cout << "Elimino el socket con puerto : " << client.getRemotePort() << " que se ha desconectado" << endl;
							SendAllPlayers("A client disconnected", &client);
						}
						else
						{
							if (players.size() == maxPlayers) //pq sino ens imprimeix error i simplement es que encara no volem rebre missatges
								cout << "Error al recibir de " << client.getRemotePort() << endl;
						}
					}
					//Si volem borrar elements de la llista hem de controlar que no ens sortim fora amb l'iterador
					if (shouldErase)
						players.erase(players.begin()+i);					
				}

			}
		}
	}

}
void ServerLogic::SendAllPlayers(string msg, TcpSocket* clientToExclude) {
	
	for (int i = 0; i < players.size(); i++) {
		TcpSocket& clientRef = *(players[i].sock);
		Packet toSend; toSend << msg;
		if (&clientRef != clientToExclude)
			clientRef.send(toSend);
	}
}
void ServerLogic::SendAllPlayers(Packet msg, TcpSocket* clientToExclude) {

	for (int i = 0; i < players.size(); i++) {
		TcpSocket& clientRef = *(players[i].sock);		
		if (&clientRef != clientToExclude)
			clientRef.send(msg);
	}
}
void ServerLogic::ComunicationManager(Packet receivedPacket, PlayerServer* pS) {

	string comand;
	receivedPacket >> comand;	

	if (players.size() == maxPlayers && EveryoneHasName()) { //si no estan tots nomes deixemr ebre info del usuari
		
		if (comand == "MSG") { //si ens envien missatge enviem a  tothom, també al que ha enviat
			
			string msg;
			receivedPacket >> msg;
			cout << "He recibido '" << msg << "' del puerto " << pS->sock->getRemotePort() << endl;
			//possibles comprovacions de insults i etc.

			Packet msgPacket; 
			msgPacket << "MSG";
			msgPacket << msg;			
			SendAllPlayers(msgPacket, NULL);			
		}				
		else if (comand == "CHECKCARD" && pS->myTurn) {
			int cardNum; int cardColor;
			receivedPacket >> cardNum >> cardColor;
			Card card2Check; card2Check.SetCard(cardNum, (CardColor)(cardColor + 1));
			if (IsCardValid(card2Check)) { //si la carta es valida actualitzem el stack per a tothom
				topCard = card2Check;
				SendCommand("UPDATESTACK", NULL);
			}
		}		
	}
	else {
		if (comand == "USERINFO") {
			
			//Ens guardem el nom
			string n;
			receivedPacket >> n;
			pS->name = n;
			
			//Avisem a tots els clients que hi ha una nova connexio
			Packet newConection;
			string c = "PLAYERNAME";
			newConection << c << pS->name;
			SendAllPlayers(newConection, pS->sock);

			//Si tots els jugadors ja estan conectats avisem
			if (players.size() == maxPlayers && EveryoneHasName()) {
				cout << "Todos los jugadores estan conectados!" << endl;

				//Enviem les cartes a cada jugador i ells ja entenen que han de començar
				for (int i = 0; i < players.size(); i++) {
					SendCommand("FILLCARDS", &players[i]);
				}
				SendCommand("UPDATESTACK", NULL); //despres de omplir la ma dels jugadors enviem la primera carta
				
				//Enviem al primer jugador que es el seu torn i comencem la rutina de torns
				clock.restart();
				players[turnIndex].myTurn = true;
				SendCommand("START_TURN", &players[turnIndex]);				
				turnIndex++;
				this->CreateThreads();

				//Aixo ja es pq pintin a la consola
				SendAllPlayers("All players are connected", NULL);
			}
			else {
				SendAllPlayers("Waiting for players...", NULL);
			}
		}
	}
	
	
}
bool ServerLogic::EveryoneHasName() {
	for (int i = 0; i < players.size(); i++) {
		if (players[i].name == "")
			return false;
	}
	return true;
}
void ServerLogic::SendCommand(string cmd, PlayerServer* pS) {
	
	lock_guard<mutex>guard(myMutex);
	
	Packet packToSend;
	packToSend << cmd;

	if (cmd == "FILLCARDS") {		
		int howMany = 5 - pS->myCards.size();
		packToSend << howMany;		
		for (int i = 0; i < howMany; i++) {
			Card newCard = deck.stack[0];			
			packToSend << newCard.number << (int)newCard.color;
			deck.stack.erase(deck.stack.begin());
		}
		pS->sock->send(packToSend);
	}
	if (cmd == "UPDATESTACK") {		
		packToSend << topCard.number << (int)topCard.color;
		SendAllPlayers(packToSend, NULL);
	}
	if (cmd == "START_TURN") {
		packToSend << pS->name;
		SendAllPlayers(packToSend, NULL);
	}
}
void ServerLogic::TurnTimer() {
	while (running) {
		Time elapsed = clock.getElapsedTime();		
		if (elapsed >= turnDuration) {			
			clock.restart();
			players[turnIndex].myTurn = false;
			turnIndex = (turnIndex + 1) % maxPlayers;
			players[turnIndex].myTurn = true;
			SendCommand("START_TURN", &players[turnIndex]);
		}
	}
	
}
void ServerLogic::CreateThreads() {
	some_threads.push_back(thread(&ServerLogic::TurnTimer, this));
	for (auto& t : some_threads) t.join();
}