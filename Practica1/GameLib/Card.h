#pragma once

enum color {red, blue, green, black};

class Card {
private:
	int number;
	color cardColor;

public:
	bool ValidAgainst(Card other) {
		if (cardColor == other.cardColor && number == other.number) // si es el mismo numero y color es valido
			return true;
		if (cardColor != other.cardColor && (number == (other.number + 1) || number == (other.number - 1))) //si no es mismo color pero hace escalera es valido.
			return true;
		else
			return false;
	}
};
