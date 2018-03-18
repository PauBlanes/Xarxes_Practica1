#include "Deck.h"

Deck::Deck() {
	std::srand(unsigned(std::time(0)));
	
	//Setegem pila
	for (int i = 2; i < 12; i++) {
		Card newRed; newRed.SetCard(i, red);
		Card newBlack; newBlack.SetCard(i, black);
		Card newBlue; newBlue.SetCard(i, blue);
		Card newGreen; newGreen.SetCard(i, green);

		stack.push_back(newRed);
		stack.push_back(newBlack);
		stack.push_back(newBlue);
		stack.push_back(newGreen);
	}
	std::random_shuffle(stack.begin(), stack.end());
}
