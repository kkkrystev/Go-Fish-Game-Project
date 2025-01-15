#include <iostream>
#include <cstdlib>
#include <ctime>
#include <vector>

//const int NUM_RANKS = 13;
const int NUM_SUIT = 4; //used entirely for readability
const int HAND_SIZE = 6; //used entirely for readability

std::vector<char> deck;
std::vector<char> playerHand;
std::vector<char> computerHand;
std::vector<char> playerSets;
std::vector<char> computerSets;

void swap(char& a, char& b) {
	char temp = a;
	a = b;
	b = temp;
}

void initialiseDeck(std::vector<char>& deck) {
	std::vector<char> ranks = { 'A', '2', '3', '4', '5', '6', '7', '8', '9', 'J', 'Q', 'K' };

	//Create all the cards (NUM_SUIT is used for readability)
	for (int rank = 0; rank < ranks.size(); rank++) {
		for (int suit = 0; suit < NUM_SUIT; suit++) {
			deck.push_back(ranks.at(rank));
		}
	}

	//Shuffle deck by swapping every card with a random one
	for (int i = 0; i < deck.size(); i++) {
		int toSwapIndex = std::rand() % deck.size();
		swap(deck.at(i), deck.at(toSwapIndex));
	}
}

void dealCards(std::vector<char>& hand) {
	//Again, here HAND_SIZE is used for readability and adjustability
	for (int i = 0; i < HAND_SIZE; i++) {
		hand.push_back(deck.back());
		deck.pop_back();
	}
}

bool hasCard(std::vector<char>& hand, char rank) {
	for (int i = 0; i < hand.size(); i++) {
		if (hand[i] == rank) {
			return true;
		}
	}
	return false;
}

void surrenderCard(std::vector<char>& askerHand, std::vector<char>& giverHand, char rank) {
	for (int i = 0; i < giverHand.size(); i++) {
		if (giverHand[i] == rank) {
			askerHand.push_back(giverHand[i]);
			giverHand.erase(giverHand.begin() + i);
		}
	}
}

void drawCardFromDeck(std::vector<char>& askerHand) {
	if (!deck.empty()) {
		askerHand.push_back(deck.back());
		deck.pop_back();
	}
	else {
		std::cout << "The deck is empty! Cannot draw a card." << std::endl;
	}
}

void sortHand(std::vector<char>& hand) {
	for (int i = 0; i < hand.size(); i++) {
		for (int j = 0; j < hand.size() - 1 - i; j++) {
			if (hand[j] > hand[j + 1]) {
				char temp = hand[j];
				hand[j] = hand[j + 1];
				hand[j + 1] = temp;
			}
		}
	}
}

void printSortedHand(std::vector<char> hand) {
	sortHand(hand);
	for (int i = 0; i < hand.size(); i++) {
		std::cout << " | " << hand[i] << " | ";
	}
	std::cout << std::endl;
	std::cout << std::endl;
}

//unfinished
//void extractSetFromHand(std::vector<char>& hand, std::vector<char>& sets, char rank) {
//	for (int i = 0; i < deck.size(); i++) {
//		if (hand[i] == rank) {
//			hand.erase(hand.begin() + i);
//		}
//	}
//
//	sets.push_back(rank);
//}

//unfinished
//bool isNewSetAvailableInHand(std::vector<char> hand) {
//	for (int i = 0; i < hand.size(); i++) {
//		char currentRank = hand[i];
//		int count = 0;
//		for(int j=0; j<hand.size(); j++)
//			if (currentRank == hand[j]) {
//				count++;
//			}
//		if (count == 4) {
//			extractSetFromHand(hand, rank);
//		}
//	}
//}

void playerTurn() {
	char rank;
	bool playerHasNextTurn = true;
	while (playerHasNextTurn) {
		std::cout << "It's your turn!" << std::endl;
		//Force player to enter valid input
		do {
			std::cout << "Enter a rank of a card PRESENT in your current hand: ";
			std::cin >> rank;
		} while (!hasCard(playerHand, rank));

		//Check if player guessed right
		if (hasCard(computerHand, rank)) {
			std::cout << "Opponent has given you all of his cards of the rank!" << std::endl;
			std::cout << std::endl;
			surrenderCard(playerHand, computerHand, rank);
			printSortedHand(playerHand);
			std::cout << "You may ask again!";
		}
		else {
			std::cout << "Opponent does NOT have any cards of such rank!" << std::endl;
			//If deck is empty, player can't draw and his turn is over
			if (deck.empty()) {
				std::cout << "Deck is empty! Your turn is over. Opponent is next..." << std::endl;
				playerHasNextTurn = false;
				break;
			}

			std::cout << "You may draw!" << std::endl;
			std::cout << std::endl;
			drawCardFromDeck(playerHand);
			char drawnCard = playerHand.back();
			std::cout << "You drew a: " << drawnCard << std::endl;
			printSortedHand(playerHand);

			//Check if drawn card == rank
			if (drawnCard != rank)
				playerHasNextTurn = false;
			else
				std::cout << "You may ask again!" << std::endl;
		}
	}
	return;
}

void computerTurn() {
	char rank;
	bool computerHasNextTurn = true;
	while (computerHasNextTurn) {
		//Pick computer's input randomly
		int randomIndex = std::rand() % computerHand.size();
		rank = computerHand[randomIndex];
		std::cout << "Opponent has asked for: " << rank << std::endl;

		//Check if computer guessed right
		if (hasCard(playerHand, rank)) {
			std::cout << "You must now surrender all cards of rank " << rank << "..." << std::endl;
			surrenderCard(computerHand, playerHand, rank);
			printSortedHand(playerHand);
		}
		else {
			//If deck is empty, computer can't draw and his turn is over
			if (deck.empty()) {
				std::cout << "Deck is empty!" << std::endl;
				computerHasNextTurn = false;
				break;
			}

			std::cout << "Opponent guessed incorrect. He will now draw..." << std::endl;
			drawCardFromDeck(computerHand);
			char drawnCard = computerHand.back();
			std::cout << "Opponent drew a: " << drawnCard << std::endl;

			//Check if randomly drawn card == rank
			if (drawnCard != rank) {
				computerHasNextTurn = false;
			}
			else
				std::cout << "Opponent may ask again!" << std::endl;
		}
	}
	return;
}

int main() {
	std::srand(std::time(0));

	std::cout << "Welcome to the game of 'Go fish!'" << std::endl;
	std::cout << "=================================" << std::endl;
	std::cout << std::endl;

	initialiseDeck(deck);
	dealCards(playerHand);
	dealCards(computerHand);

	std::cout << "These are the cards you have been dealt: " << std::endl;
	printSortedHand(playerHand);
	std::cout << std::endl;
	
	while (!deck.empty()) {
		// Player Turn
		playerTurn();

		// Computer Turn
		computerTurn();
	}

	std::cout << "End";
	return 0;
}