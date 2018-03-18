#pragma once
#include <SFML\Graphics.hpp>
#include <SFML\Network.hpp>
#include <string>
#include <iostream>

using namespace sf;
using namespace std;

enum CardColor { red, black, blue, green };

class Card { 
private:		
	Texture texture;
	Sprite sprite;
public:		
	Card(int num, CardColor c, Vector2i pos);
	Card(Vector2i pos);
	Card();
	void SetCard(int num, CardColor c);
	void SetRotation(int rotation);
	Vector2i position;
	void SetPosition(Vector2i pos);
	bool isClicked(RenderWindow* window);
	bool operator==(const Card& other) const;
	Sprite GetSprite();

	int number;
	CardColor color;
	
	string filename;
};




	

