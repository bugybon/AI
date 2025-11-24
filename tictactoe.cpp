#include <iostream>
#include <string>

enum Tile {
	_,
	O,
	X
};

bool isTerminalState(Tile* game) {
	for (int i = 0; i < 3; i++) {
		if ((game[i * 3] != _ && game[i * 3] == game[i * 3 + 1] && game[i * 3] == game[i * 3 + 2]) ||
			(game[i] != _ && game[i] == game[3 + i] && game[i] == game[6 + i]))
			return true;
	}

	if ((game[0] != _ && game[0] == game[4] && game[0] == game[8]) ||
		(game[3] != _ && game[2] == game[4] && game[2] == game[6]))
		return true;

	return false;
}

Tile translateCharToTile(const char& ch) {
	switch (ch) {
	case 'X':
		return X;
	case 'O':
		return O;
	case '_':
		return _;
	default:
		std::cout << "Char not a tile";
		return;
	}
}

void translateStateToArray(const std::string& state, Tile* game) {
	for (int i = 0; i < 9; i++) {
		game[i] = translateCharToTile(state[i]);
	}
}

std::string translateInputToState(const std::string& input) {
	std::string tempState = "";

	for (int i = 0; i < input.size(); i++) {
		if(input[i] == '_' || input[i] == 'O' || input[i] == 'X')
			tempState += input[i];
	}

	if (tempState.size() != 9) std::cout << "Wrong state input";

	return tempState;
}

void printState(Tile* game) {
	std::cout << "+---+---+---+" << std::endl;
	for (int row = 0; row < 3; row++) {
		for (int col = 0; col < 3; col++) {
			std::cout << "| " << game[row * 3 + col] << " ";
		}
		std::cout << "|" << std::endl << "+---+---+---+" << std::endl;
	}
}

int main() {
	std::string inputType, inputTurn, inputState, input;
	Tile game[9];
	int depth = 0;
	
	std::cin >> inputType;
	std::getline(std::cin, inputTurn);
	for (int i = 0; i < 7; i++) {
		std::getline(std::cin, input);
		inputState += translateInputToState(input);
	}
	translateStateToArray(inputState, game);

	for (int i = 0; i < 9; i++)
		depth += (game[i] != _) ? 1 : 0;

	if(inputType == "JUDGE"){
		if (inputTurn[5] == 'X') {
			judge(game, X, depth);
		} else if (inputTurn[5] == 'O') {
			judge(game, O, depth);
		}
		else {
			std::cout << "Not a possible turn";
		}
	}
	else if (inputType == "GAME") {
		// ...
	}
	else {
		std::cout << "Not a game type";
	}

	return 0;
}

