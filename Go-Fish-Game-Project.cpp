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

// CARD DEALING METHODS
void swap(char& a, char& b) {
	char temp = a;
	a = b;
	b = temp;
}

void initialiseDeck(std::vector<char>& deck) {
	std::vector<char> ranks = { '1', '2', '3', '4', '5' };

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

// STAGE 1 METHODS USED FOR BOTH PARTICIPANTS
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
	askerHand.push_back(deck.back());
	deck.pop_back();
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

void extractAvailableSets(std::vector<char>& hand, std::vector<char>& sets, char setRank) {
	// Remove all cards of the set rank from the hand
	int index = 0;
	while (index < hand.size()) {
		if (hand[index] == setRank)
			hand.erase(hand.begin() + index);
		else
			index++;
	}

	sets.push_back(setRank);
}

// STAGE 1 PLAYER TURN METHODS
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
	std::cout << "HAND: ";
	for (int i = 0; i < hand.size(); i++) {
		std::cout << " | " << hand[i] << " | ";
	}
	std::cout << std::endl;
	std::cout << std::endl;
}

void handleEmptyPlayerHand(bool& playerHasNextTurn) {
	if (playerHand.empty() && !deck.empty()) {
		std::cout << "Your hand is empty. Drawing...";
		drawCardFromDeck(playerHand);
		char drawnCard = playerHand.back();
		std::cout << "You drew rank " << drawnCard << std::endl;
		printSortedHand(playerHand);
	}
	else if (playerHand.empty() && deck.empty()) {
		playerHasNextTurn = false;
		std::cout << "The deck is empty! You cannot draw a card..." << std::endl;
	}
}

void handleSetCompletion(char setRank, bool& playerHasNextTurn) {
	char choice;
	do {
		std::cout << "You have completed a set of " << setRank << "'s! Enter (y/Y) to lower it: ";
		std::cin >> choice;
	} while (choice != 'y' && choice != 'Y');
	clearInputBuffer();

	if (choice == 'y' || choice == 'Y') {
		extractAvailableSets(playerHand, playerSets, setRank);
		std::cout << std::endl;
	}
	if (!playerHand.empty())
		printSortedHand(playerHand);
}

void handleInitialPlayerSetExtraction(bool& playerHasNextTurn) {
	char setRank;
	if (checkForSets(playerHand, setRank)) {
		handleSetCompletion(setRank, playerHasNextTurn);
	}
}

char getValidPlayerInput() {
	char rank;
	do {
		std::cout << "Enter a rank of a card PRESENT in your current hand: ";
		std::cin >> rank;
		std::cout << std::endl;
		clearInputBuffer();
	} while (!hasCard(playerHand, rank));

	return rank;
}

void handleOpponentHasCard(char rank, bool& playerHasNextTurn) {
	char setRank;

	surrenderCard(playerHand, computerHand, rank);
	std::cout << "Opponent has given you all of his cards of the rank!" << std::endl << std::endl;
	printSortedHand(playerHand);

	// Check for newly formed sets
	if (checkForSets(playerHand, setRank)) 
		handleSetCompletion(setRank, playerHasNextTurn);

	// Check if newly formed set emptieed hand
	handleEmptyPlayerHand(playerHasNextTurn);
	if (!playerHasNextTurn) return;

	std::cout << "You may ask again!" << std::endl << std::endl;
}

void handleOpponentDoesNotHaveCard(char rank, bool& playerHasNextTurn) {
	char setRank;

	// If deck is empty, player can't draw a card => turn is over
	if (deck.empty()) {
		std::cout << "Deck is empty! Your cannot draw" << std::endl;
		playerHasNextTurn = false;
		return;
	}

	std::cout << "Go fish! ";
	drawCardFromDeck(playerHand);
	char drawnCard = playerHand.back();
	std::cout << "You drew rank " << drawnCard << std::endl;
	printSortedHand(playerHand);

	// Extract any newly formed sets after drawing
	if (checkForSets(playerHand, setRank)) 
		handleSetCompletion(setRank, playerHasNextTurn);

	// Check if newly formed set emptieed hand
	handleEmptyPlayerHand(playerHasNextTurn);
	if (!playerHasNextTurn) return;

	// Check if drawn card == rank
	if (drawnCard != rank)
		playerHasNextTurn = false;
	else {
		std::cout << "You may ask again! " << std::endl;
		printSortedHand(playerHand);
	}
}

void playerTurn() {
	char rank;
	bool playerHasNextTurn = true;
	
	handleInitialPlayerSetExtraction(playerHasNextTurn);

	while (playerHasNextTurn) {
		handleEmptyPlayerHand(playerHasNextTurn);
		if (!playerHasNextTurn) return;

		//std::cout << "It's your turn!" << std::endl;

		rank = getValidPlayerInput();

		if (hasCard(computerHand, rank)) 
			handleOpponentHasCard(rank, playerHasNextTurn);
		else 
			handleOpponentDoesNotHaveCard(rank, playerHasNextTurn);
	}
}

// STAGE 1 COMPUTER TURN METHODS
char getComputerInput() {
	char rank;

	int randomIndex = std::rand() % computerHand.size();
	rank = computerHand[randomIndex];

	return rank;
}

void handleEmptyComputerHand(bool& computerHasNextTurn) {
	if (computerHand.empty() && !deck.empty())
		drawCardFromDeck(computerHand);
	else if (computerHand.empty() && deck.empty()) {
		computerHasNextTurn = false;
		std::cout << "The deck is empty! Computer cannot draw a card." << std::endl;
	}
}

void handleInitialComputerSetExtraction(bool& computerHasNextTurn) {
	char setRank;
	if (checkForSets(computerHand, setRank)) {
		handleSetCompletion(setRank, computerHasNextTurn);
	}
}

void handlePlayerHasCard(char rank, bool& computerHasNextTurn) {
	char choice, setRank;
	std::cout << "Enter (y/Y) to surrender all cards of rank " << rank << ": ";
	std::cin >> choice;
	clearInputBuffer();
	if (choice == 'y' || choice == 'Y') {
		surrenderCard(computerHand, playerHand, rank);
		printSortedHand(playerHand);
	}

	// Check for newly formed sets
	if (checkForSets(computerHand, setRank))
		extractAvailableSets(computerHand, computerSets, setRank);

	// Check if newly formed set emptied hand
	handleEmptyComputerHand(computerHasNextTurn);
	if (!computerHasNextTurn) return;
}

void handlePlayerDoesNotHaveCard(char rank, bool& computerHasNextTurn) {
	char setRank;

	// If deck is empty, computer can't draw a card => turn is over
	if (deck.empty()) {
		std::cout << "Deck is empty!" << std::endl;
		computerHasNextTurn = false;
		return;
	}

	std::cout << "He will now draw...";
	drawCardFromDeck(computerHand);
	char drawnCard = computerHand.back();

	// Extract any newly formed sets after drawing
	if (checkForSets(computerHand, setRank))
		extractAvailableSets(computerHand, computerSets, setRank);

	// Check if newly formed set emptieed hand
	handleEmptyComputerHand(computerHasNextTurn);
	if (!computerHasNextTurn) return;

	// Check if randomly drawn card == rank
	if (drawnCard != rank) {
		computerHasNextTurn = false;
	}
	else
		std::cout << "Opponent may ask again!" << std::endl << std::endl;
}

void computerTurn() {
	char rank;
	bool computerHasNextTurn = true;
	//std::vector<char> lastAskedRank;

	handleInitialComputerSetExtraction(computerHasNextTurn);

	while (computerHasNextTurn) {
		rank = getComputerInput();
		/*If random rank has been asked during turn, pick another rank
		while (std::find(lastAskedRank.begin(), lastAskedRank.end(), rank) != lastAskedRank.end()) {
			randomIndex = std::rand() % computerHand.size();
			rank = computerHand[randomIndex];
		}
		lastAskedRank.push_back(rank);*/
		std::cout << "It's opponent's turn! Opponent has asked for: " << rank << std::endl;

		if (hasCard(playerHand, rank)) 
			handlePlayerHasCard(rank, computerHasNextTurn);
		else 
			handlePlayerDoesNotHaveCard(rank, computerHasNextTurn);	
	}
}

// STAGE 2 

// GAME METHODS
void gameStage_1() {
	std::cout << "These are the cards you have been dealt: " << std::endl;
	printSortedHand(playerHand);
	std::cout << "Let stage 1 begin!" << std::endl;
	std::cout << "------------------" << std::endl << std::endl;

	while (true) {
		if (!playerHand.empty())
			playerTurn();

		if (!computerHand.empty())
			computerTurn();

		if (playerHand.empty() && computerHand.empty() && deck.empty()) {
			std::cout << "All sets have been collected. End of stage 1!" << std::endl;
			std::cout << "---------------------------------------------" << std::endl << std::endl;
			break;
		}
	}
}
void gameStage_() {

}

int main() {
	std::srand(std::time(0));

	// UI
	std::cout << "Welcome to the game of 'Go fish!'" << std::endl;
	std::cout << "---------------------------------" << std::endl;
	std::cout << std::endl;

	// Initialise deck and deal cards
	initialiseDeck(deck);
	dealCards(playerHand);
	dealCards(computerHand);

	// Game stage 1
	gameStage_1();

	// Game stage 2
	std::cout << "Let stage 2 begin!" << std::endl;
	std::cout << "------------------" << std::endl << std::endl;

	return 0;
}