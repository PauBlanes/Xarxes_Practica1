#pragma once
#include "Card.h"
#include <vector>
#include <SFML\Graphics.hpp>

class Table {
	
public:
	Table();
	void DrawTable(RenderWindow* window);
	Card centerCard;
	Card prova;
	vector<Card>myCards;
	vector<Card>otherCards;
};