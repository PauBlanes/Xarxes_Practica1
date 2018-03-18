#include "Table.h"

Table::Table() {
	//creem les cartes dels oponents
	vector<Vector2i>opponentPositions;
	opponentPositions.push_back({ 280,100 });
	opponentPositions.push_back({ 330,100 });
	opponentPositions.push_back({ 380,100 });
	opponentPositions.push_back({ 430,100 });
	opponentPositions.push_back({ 480,100 });

	opponentPositions.push_back({ 700,180 });
	opponentPositions.push_back({ 700,230 });
	opponentPositions.push_back({ 700,280 });
	opponentPositions.push_back({ 700,330 });
	opponentPositions.push_back({ 700,380 });

	opponentPositions.push_back({ 170,180 });
	opponentPositions.push_back({ 170,230 });
	opponentPositions.push_back({ 170,280 });
	opponentPositions.push_back({ 170,330 });
	opponentPositions.push_back({ 170,380 });
	
	
	for (int i = 0; i < 15; i++) {		
		Card newCard(opponentPositions[i]);
		if (i > 4)
			newCard.SetRotation(90);
		otherCards.push_back(newCard);
	}

	//Omplo array de posicions buides per les meves cartes
	emptyCardSlots.push_back({ 330,450 });
	emptyCardSlots.push_back({ 380,450 });
	emptyCardSlots.push_back({ 430,450 });
	emptyCardSlots.push_back({ 480,450 });
	emptyCardSlots.push_back({ 280,450 });

	Card temp(Vector2i(380, 280));
	centerCard = temp;
}
void Table::DrawTable(RenderWindow* window) {
	
	window->draw(centerCard.GetSprite());
	
	//pintar les dels altres	
	for (int i = 0; i < otherCards.size(); i++) {
		window->draw(otherCards[i].GetSprite());
	}
	//pintar les meves
	for (int i = 0; i < myCards.size(); i++) {
		window->draw(myCards[i].GetSprite());
	}
	
}

void Table::FillCards(Packet newCards) {
	int numCards;
	newCards >> numCards;	
	for (int i = 0; i < numCards; i++) {
		int cardNum; int cardColor;
		newCards >> cardNum >> cardColor;		
		Card temp(cardNum, (CardColor)(cardColor+1), emptyCardSlots[0]);
		myCards.push_back(temp);
		emptyCardSlots.erase(emptyCardSlots.begin());
	}
}
void Table::UpdateStack(Packet newCard) {
	int cardNum; int cardColor;
	newCard >> cardNum >> cardColor;	
	centerCard.SetCard(cardNum, (CardColor)(cardColor+1));
}