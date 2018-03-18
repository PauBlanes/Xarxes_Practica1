#pragma once
#include "Card.h"
#include <vector>
#include <SFML\Graphics.hpp>
#include <SFML\Network.hpp>


class Table {
	
public:
	Table();
	void DrawTable(RenderWindow* window);
	Card centerCard;
	Card prova;
	vector<Card>myCards;
	vector<Card>otherCards;
	vector<Vector2i> emptyCardSlots;
	void FillCards(Packet newCards);
	void UpdateStack(Packet newCard);
};