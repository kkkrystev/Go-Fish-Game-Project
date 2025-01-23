/*
* Solution to course project # 8
* Introduction to programming course
* Faculty of Mathematics and Informatics of Sofia University
* Winter semester 2024/2025
*
* @author Kristian Krastev
* @idnumber 4MI0600440* @compiler VC
*
* <file containing the game logic and execution methods>
* 
*/

#include <iostream>
#include <cstdlib>
#include <ctime>
#include <vector>

const int NUM_RANKS = 13; // used entirely for readability
const int NUM_SUIT = 4; // used entirely for readability
const int HAND_SIZE = 6; // used entirely for readability

std::vector<char> deck;
std::vector<char> ranks = { '2', '3', '4', '5', '6', '7', '8', '9', 'X', 'J', 'Q', 'K', 'A' };
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

// METHODS USED FOR BOTH PARTICIPANTS
bool hasRank(std::vector<char> vector, char rank) {
	for (int i = 0; i < vector.size(); i++) {
		if (vector.at(i) == rank) {
			return true;
		}
	}
	return false;
}

void transferItems(std::vector<char>& receiver, std::vector<char>& giver, char rank) {
	for (int i = 0; i < giver.size();) {
		if (giver.at(i) == rank) {
			receiver.push_back(giver.at(i));
			giver.erase(giver.begin() + i);
		}
		else {
			++i;
		}
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

void gameEndCheck() {
	if (playerSets.empty())
		std::cout << "Computer collected all the sets! You lose :(" << std::endl;
	else if (computerSets.empty())
		std::cout << "You collected all the sets! You win :) " << std::endl;;
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
		std::cout << "The deck is empty! You cannot draw a card..." << std::endl << std::endl;
	}
}

void handleSetCompletion(char setRank, bool& playerHasNextTurn) {
	char choice;
	do {
		std::cout << "You have completed a set of " << setRank << "'s! Enter (y/Y) to lower it: ";
		std::cin >> choice;
		clearInputBuffer();
	} while (choice != 'y' && choice != 'Y');

	extractAvailableSets(playerHand, playerSets, setRank);
	std::cout << std::endl;

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
	} while (!hasRank(playerHand, rank));

	return rank;
}

void handleOpponentHasCard(char rank, bool& playerHasNextTurn) {
	char setRank;

	transferItems(playerHand, computerHand, rank);
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
	std::cout << "You drew rank " << drawnCard << std::endl << std::endl;
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
		std::cout << "You may ask again! " << std::endl << std::endl;
		//printSortedHand(playerHand);
	}
}

void playerTurn_stage1() {
	char rank;
	bool playerHasNextTurn = true;
	
	handleInitialPlayerSetExtraction(playerHasNextTurn);

	while (playerHasNextTurn) {
		handleEmptyPlayerHand(playerHasNextTurn);
		if (!playerHasNextTurn) return;

		rank = getValidPlayerInput();

		if (hasRank(computerHand, rank)) 
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
		std::cout << "The deck is empty! Computer cannot draw a card." << std::endl << std::endl;
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
	do {
		std::cout << "Enter (y/Y) to surrender all cards of rank " << rank << ": ";
		std::cin >> choice;
		std::cout << std::endl;
		clearInputBuffer();
	} while (choice != 'y' && choice != 'Y');
	
	transferItems(computerHand, playerHand, rank);
	printSortedHand(playerHand);
	
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
		std::cout << "It's your turn! " << std::endl << std::endl;
	}
	else
		std::cout << "Opponent may ask again!" << std::endl << std::endl;
}

void computerTurn_stage1() {
	char rank;
	bool computerHasNextTurn = true;

	handleInitialComputerSetExtraction(computerHasNextTurn);

	while (computerHasNextTurn) {
		rank = getComputerInput();
		std::cout << "It's opponent's turn! Opponent has asked for: " << rank << std::endl;

		if (hasRank(playerHand, rank)) 
			handlePlayerHasCard(rank, computerHasNextTurn);
		else 
			handlePlayerDoesNotHaveCard(rank, computerHasNextTurn);	
	}
}

// STAGE 2 PLAYER TURN METHODS
char getValidPlayerSetInput() {
	char setRank;
	do {
		std::cout << "Enter a VALID rank of a set: ";
		std::cin >> setRank;
		std::cout << std::endl;
		clearInputBuffer();
	} while (!hasRank(ranks, setRank));

	return setRank;
}

void handleOpponentHasSet(char setRank, bool& playerHasNextSetTurn) {
	transferItems(playerSets, computerSets, setRank);
	std::cout << "Correct! Opponent has given you his set of " << setRank << "'s... " << std::endl << std::endl;
	
	// Check if tranfer emptied computerSet vector
	if (computerSets.empty()) {
		playerHasNextSetTurn = false;
		return;
	}

	std::cout << "You may ask again!" << std::endl << std::endl;
}

void playerTurn_stage2() {
	char setRank;
	bool playerHasNextSetTurn = true;

	gameEndCheck();

	while (playerHasNextSetTurn) {
		setRank = getValidPlayerSetInput();

		if (hasRank(computerSets, setRank)) {
			handleOpponentHasSet(setRank, playerHasNextSetTurn);
		}
		else {
			std::cout << "Your guess is incorrect! It's opponent's turn..." << std::endl << std::endl;
			playerHasNextSetTurn = false;
		}
	}
}

// STAGE 2 COMPUTER METHODS
char getComputerSetInput() {
	int randomIndex = std::rand() % ranks.size();
	char setRank = ranks[randomIndex];
	std::cout << "Opponent has asked for your set of " << setRank << "'s... " << std::endl;

	return setRank;
}

void handlePlayerHasSet(char setRank, bool& computerHasNextTurn) {
	char choice;
	do {
		std::cout << "Enter (y/Y) to surrender the set of rank " << setRank << ": ";
		std::cin >> choice;
		clearInputBuffer();
	} while (choice != 'y' && choice != 'Y');
	
	std::cout << std::endl;
	transferItems(computerSets, playerSets, setRank);

	// Check if transfer emptied playerSet vector
	if (playerSets.empty()) {
		computerHasNextTurn = false;
		return;
	}
}

void computerTurn_stage2() {
	char setRank;
	bool computerHasNextTurn = true;

	gameEndCheck();

	while (computerHasNextTurn) {
		setRank = getComputerSetInput();
		if (hasRank(playerSets, setRank))
			handlePlayerHasSet(setRank, computerHasNextTurn);
		else {
			std::cout << "Oppont has guessed incorrectly! It's your turn" << std::endl << std::endl;
			computerHasNextTurn = false;
		}
	}
}

// GAME METHODS
void gameStage_1() {
	std::cout << "These are the cards you have been dealt: " << std::endl;
	printSortedHand(playerHand);
	std::cout << "Let stage 1 begin!" << std::endl;
	std::cout << "------------------" << std::endl << std::endl;

	while (true) {
		if (!playerHand.empty())
			playerTurn_stage1();

		if (!computerHand.empty())
			computerTurn_stage1();

		if (playerHand.empty() && computerHand.empty() && deck.empty()) {
			std::cout << "All sets have been collected. End of stage 1!" << std::endl;
			std::cout << "---------------------------------------------" << std::endl << std::endl;
			break;
		}
	}
}

void gameStage_2() {
	std::cout << "Let stage 2 begin!" << std::endl;
	std::cout << "------------------" << std::endl << std::endl;

	while (!playerSets.empty() && !computerSets.empty()) {
		if (!playerSets.empty()) 
			playerTurn_stage2();

		if (!computerSets.empty())
			computerTurn_stage2();
	}
		
	gameEndCheck();
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
	gameStage_2();

	return 0;
}