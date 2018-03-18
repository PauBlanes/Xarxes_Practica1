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

	
		
	
}
void Table::DrawTable(RenderWindow* window) {
	
	window->draw(centerCard.GetSprite());
	
	//pintar les dels altres	
	for (int i = 0; i < 15; i++) {
		window->draw(otherCards[i].GetSprite());
	}
	//pintar les meves
	for (int i = 0; i < 4; i++) {
		window->draw(myCards[i].GetSprite());
	}
	
}