#include "Card.h"


Card::Card(int num, CardColor c, Vector2f pos, bool turned) {
	number = num;
	color = c;
	position = pos;
	
	//Setegem el sprite
	string filename;
	if (turned == false) {
		filename = "img/" + to_string(number) + "-" + to_string(color) + ".jpg";				
	}
	else {
		filename = "img/back.jpg";				
	}	
	texture.loadFromFile(filename);
	sprite.setTexture(texture);
	sprite.setPosition(pos.x, pos.y);	
}