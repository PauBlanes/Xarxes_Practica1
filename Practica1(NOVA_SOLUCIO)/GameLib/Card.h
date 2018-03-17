#pragma once
#include <SFML\Graphics.hpp>
#include <string>
#include <iostream>

using namespace sf;
using namespace std;

enum CardColor { red, blue, green, black };

class Card { 
private:
	int number; 
	CardColor color;
	Vector2f position;
	Sprite sprite;
	Texture texture;

public:
	Card (int num, CardColor c, Vector2f pos, bool turned);	

};


	//AIXO SERA UNA FUNCIO QUE TE EL SERVER
	/*bool IsCardValid(Card other) {
		if (cardColor == other.cardColor && number == other.number) // si es el mismo numero y color es valido
			return true;
		if (cardColor != other.cardColor && (number == (other.number + 1) || number == (other.number - 1))) //si no es mismo color pero hace escalera es valido.
			return true;
		else
			return false;
	}*/

