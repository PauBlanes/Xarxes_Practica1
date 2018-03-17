#pragma once
#include <SFML\Graphics.hpp>
#include <string>
#include <iostream>

using namespace sf;
using namespace std;

enum CardColor { red, blue, green, black };

class Card { 
private:	
	Vector2f position;
	Sprite sprite;
	Texture texture;

public:		
	Card (int num, CardColor c, Vector2f pos, bool turned);
	Card() {}
	int number;
	CardColor color;

};


	

