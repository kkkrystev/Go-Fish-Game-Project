#include <iostream>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <string>

const int NUM_RANKS = 13;
const int NUM_SUIT = 4; //used entirely for readability
const int HAND_SIZE = 6; //used entirely for readability hey

std::vector<int> deck;
std::vector<int> playerHand;
std::vector<int> computerHand;
std::vector<int> playerSets;
std::vector<int> computerSets;

void swap(int& a, int& b) {
	int temp = a;
	a = b;
	b = temp;
}
//uyuyiyiiyi

void initialiseDeck(std::vector<int>& deck) {
	//Create all the cards (NUM_SUIT is used for readability)
	for (int rank = 0; rank < NUM_RANKS; rank++) {
		for (int suit = 0; suit < NUM_SUIT; suit++) {
			deck.push_back(rank);
		}
	}

	//Shuffle deck by swapping every card with a random one
	std::srand(std::time(0));
	for (int i = 0; i < deck.size(); i++) {
		int toSwap = std::rand() % deck.size();
		swap(deck.at(i), deck.at(toSwap));
	}
}

void dealCards(std::vector<int>& hand) {
	//Again, here HAND_SIZE is used for readability and adjustability
	for (int i = 0; i < HAND_SIZE; i++) {
		hand.push_back(deck.back());
		deck.pop_back();
	}
}

bool hasCard(std::vector<int>& hand, int rank) {
	for (int i = 0; i < hand.size(); i++) {
		if (hand[i] == rank) {
			return true;
		}
	}
	return false;
}

void surrenderCard(std::vector<int>& askerHand, std::vector<int>& giverHand, int rank) {
	for (int i = 0; i < giverHand.size(); i++) {
		if (giverHand[i] == rank) {
			askerHand.push_back(giverHand[i]);
			giverHand.erase(giverHand.begin() + i);
		}
	}
}

void drawCardFromDeck(std::vector<int>& askerHand) {
	askerHand.push_back(deck.back());
	deck.pop_back();
}

void printHand(std::vector<int> hand) {
	for (int i = 0; i < hand.size(); i++) {
		std::cout << hand[i] << " | ";
	}
	std::cout << std::endl;
}

void playerTurn() {
	int rank;
	bool playerHasNextTurn = true;
	while (playerHasNextTurn) {
		//Force player to enter valid input
		do {
			std::cout << "Enter a rank of a card PRESENT in your current hand: ";
			std::cin >> rank;
		} while (!hasCard(playerHand, rank));

		//Check if player guessed right
		if (hasCard(computerHand, rank)) {
			std::cout << "Opponent has given you all of his cards of the rank!" << std::endl;
			surrenderCard(playerHand, computerHand, rank);
			std::cout << "You may ask again!";
		}
		else {
			std::cout << "Opponent does NOT have any cards of such rank!" << std::endl;
			//If deck is empty, player can't draw and his turn is over
			if (deck.empty()) {
				std::cout << "Deck is empty! Your turn is over. Opponent is next..." << std::endl;
				break;
			}

			std::cout << "You may draw!" << std::endl;
			drawCardFromDeck(playerHand);
			int drawnCard = playerHand.back();
			std::cout << "You drew a: " << drawnCard << std::endl;

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
	int rank;
	bool computerHasNextTurn = true;
	while (computerHasNextTurn) {
		//Pick computer's input randomly
		std::srand(std::time(0));
		int randomIndex = std::rand() % computerHand.size();
		rank = computerHand[randomIndex];
		std::cout << "Opponent has asked for: " << rank << std::endl;

		//Check if computer guessed right
		if (hasCard(playerHand, rank)) {
			std::cout << "You must now surrender all cards of rank " << rank << std::endl;
			surrenderCard(computerHand, playerHand, rank);
		}
		else {
			//If deck is empty, computer can't draw and his turn is over
			if (deck.empty()) {
				std::cout << "Deck is empty! Opponent's turn is over. You are next..." << std::endl;
				break;
			}

			std::cout << "Opponent guessed incorrect. He will now draw...";
			drawCardFromDeck(computerHand);
			int drawnCard = playerHand.back();
			std::cout << "Opponent drew a: " << drawnCard;

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