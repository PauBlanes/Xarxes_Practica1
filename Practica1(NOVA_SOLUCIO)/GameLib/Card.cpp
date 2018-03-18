#include "Card.h"


Card::Card(int num, CardColor c, Vector2i pos) {
	number = num;
	color = c;
	position = pos;
	
	//Setegem el sprite
	filename;	
	filename = "img/" + to_string(number) + "-" + to_string(color+1) + ".jpg";				
		
	sprite.setPosition(pos.x, pos.y);	
}
Card::Card(Vector2i pos) {

	filename = "img/back.jpg";

	position = pos;
	sprite.setPosition(pos.x, pos.y);
}
Card::Card(){}
void Card::SetCard(int num, CardColor c) {	
		
	//Setegem el sprite
	number = num;
	color = c;
	filename = "img/" + to_string(number) + "-" + to_string(color + 1) + ".jpg";	
		
}
Sprite Card::GetSprite() { //aixo ho fem pq perdem al referencia a la textura al treballar amb vectors sino
	texture.loadFromFile(filename);
	sprite.setTexture(texture);
	return sprite;
}
void Card::SetRotation(int rotation) {
	sprite.setRotation(rotation);
}
void Card::SetPosition(Vector2i pos) {
	position = pos;
	sprite.setPosition(position.x, position.y);
}
