// Improve player input validation
// Take care of pointless set extraction (if possible)
// Improve UI
// Fix accidental mistake of the computer to ask for the same rank twice
// Make player actions manual, rather than automated
// Implement second part of the game
// Add comments for readability

#include <iostream>
#include <cstdlib>
#include <ctime>
#include <vector>

// const int NUM_RANKS = 13;
const int NUM_SUIT = 4; // used entirely for readability
const int HAND_SIZE = 6; // used entirely for readability

std::vector<char> deck;
std::vector<char> playerHand;
std::vector<char> computerHand;
std::vector<char> playerSets;
std::vector<char> computerSets;

void clearInputBuffer() {
	std::cin.clear();
	std::cin.sync();
	std::cin.ignore(1000, '\n');
}

void swap(char& a, char& b) {
	char temp = a;
	a = b;
	b = temp;
}

void initialiseDeck(std::vector<char>& deck) {
	std::vector<char> ranks = { '2', '3', '4', '5', '6', '7', '8', '9', 'X', 'J', 'Q', 'K', 'A' };

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
	for (int i = 0; i < giverHand.size();) {
		if (giverHand[i] == rank) {
			askerHand.push_back(giverHand[i]);
			giverHand.erase(giverHand.begin() + i);
		}
		else
			i++;
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
	// Sort hand for to improve player experience
	sortHand(hand);

	// Print sorted hand
	for (int i = 0; i < hand.size(); i++) {
		std::cout << " | " << hand[i] << " | ";
	}
	std::cout << std::endl;
	std::cout << std::endl;
}

bool checkForSets(const std::vector<char>& hand, char& setRank) {
	for (int i = 0; i < hand.size(); i++) {
		char currentRank = hand[i];
		int count = 0;

		// Count occurrences of the current rank
		for (int j = 0; j < hand.size(); j++) {
			if (hand[j] == currentRank)
				count++;
		}

		// If a set is found, return true and the rank
		if (count == 4) {
			setRank = currentRank;
			return true;
		}
	}
	return false;
}

void extractAvailableSets(std::vector<char>& hand, std::vector<char>& sets, char setRank, bool isPlayer) {
	// Remove all cards of the set rank from the hand
	int index = 0;
	while (index < hand.size()) {
		if (hand[index] == setRank)
			hand.erase(hand.begin() + index);
		else
			index++;
	}

	sets.push_back(setRank);

	// If it's the player's hand, print a message and their hand
	if (isPlayer) {
		std::cout << "You have completed a set of " << setRank << "'s!" << std::endl << std::endl;
		printSortedHand(playerHand);
	}
}

void playerTurn() {
	char rank, setRank;
	bool playerHasNextTurn = true;
	
	// Extract sets if player has sets from card dealing
	if (checkForSets(playerHand, setRank))
		extractAvailableSets(playerHand, playerSets, setRank, true);

	while (playerHasNextTurn) {
		setRank = -1;

		std::cout << "It's your turn!" << std::endl;

		// Force player to enter valid input
		do {
			std::cout << "Enter a rank of a card PRESENT in your current hand: ";
			std::cin >> rank;
			std::cout << std::endl;
			clearInputBuffer();
		} while (!hasCard(playerHand, rank));

		// Check if opponent has card (player guessed correctly)
		if (hasCard(computerHand, rank)) {
			std::cout << "Opponent has given you all of his cards of the rank!" << std::endl << std::endl;
			surrenderCard(playerHand, computerHand, rank);
			printSortedHand(playerHand);
			
			// Extract any newly formed sets after stealing cards
			if (checkForSets(playerHand, setRank))
				extractAvailableSets(playerHand, playerSets, setRank, true);

			std::cout << "You may ask again!";
		}
		else {
			std::cout << "Opponent does NOT have any cards of such rank!" << std::endl;
			// If deck is empty, player can't draw and his turn is over
			if (deck.empty()) {
				std::cout << "Deck is empty! Your turn is over. Opponent is next..." << std::endl;
				playerHasNextTurn = false;
				break;
			}

			std::cout << "You may draw!" << std::endl << std::endl;
			drawCardFromDeck(playerHand);
			char drawnCard = playerHand.back();
			std::cout << "You drew rank " << drawnCard << std::endl;
			printSortedHand(playerHand);

			// Extract any newly formed sets after drawing
			if (checkForSets(playerHand, setRank))
				extractAvailableSets(playerHand, playerSets, setRank, true);

			// Check if drawn card == rank
			if (drawnCard != rank)
				playerHasNextTurn = false;
			else
				std::cout << "You may ask again! " << std::endl;
		}
	}
	return;
}

void computerTurn() {
	char rank, setRank;
	bool computerHasNextTurn = true;

	// Extract sets if computer has them from card dealing
	if (checkForSets(computerHand, setRank))
		extractAvailableSets(computerHand, computerSets, setRank, false);

	while (computerHasNextTurn) {
		setRank = -1;

		// Pick computer's input randomly
		int randomIndex = std::rand() % computerHand.size();
		rank = computerHand[randomIndex];
		std::cout << "Opponent has asked for: " << rank << std::endl;

		// Check if computer guessed right
		if (hasCard(playerHand, rank)) {
			std::cout << "You must now surrender all cards of rank " << rank << "..." << std::endl;
			surrenderCard(computerHand, playerHand, rank);
			printSortedHand(playerHand);
		}
		else {
			// If deck is empty, computer can't draw and his turn is over
			if (deck.empty()) {
				std::cout << "Deck is empty!" << std::endl;
				computerHasNextTurn = false;
				break;
			}

			std::cout << "He will now draw...";
			drawCardFromDeck(computerHand);
			char drawnCard = computerHand.back();
			std::cout << "Opponent drew rank " << drawnCard << std::endl << std::endl;

			// Check if randomly drawn card == rank
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
	std::cout << "---------------------------------" << std::endl;
	std::cout << std::endl;

	initialiseDeck(deck);
	dealCards(playerHand);
	dealCards(computerHand);

	std::cout << "These are the cards you have been dealt: " << std::endl;
	printSortedHand(playerHand);
	std::cout << "Let the game begin!" << std::endl;
	std::cout << "-------------------" << std::endl << std::endl;
	
	while (!deck.empty()) {
		// Player Turn
		playerTurn();

		// Computer Turn
		computerTurn();
	}

	std::cout << "End";
	return 0;
}