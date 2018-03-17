#include "Card.h"


Card::Card(int num, CardColor c, Vector2f pos, bool turned) {
	number = num;
	color = c;
	position = pos;
	
	//Setegem el sprite
	if (turned == false) {
		string filename = "img/" + to_string(number) + "-" + to_string(color) + ".jpg";
		texture.loadFromFile(filename);
		sprite.setTexture(texture);
	}
	else {
		string filename = "img/back.jpg";
		texture.loadFromFile(filename);
		sprite.setTexture(texture);
	}
	
}