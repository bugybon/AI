#include <iostream>
#include <string>

enum Tile {
	X = 1,
	_ = 0,
	O = -1
};

int valueArray[9];

int maxValue(Tile* game, int& alpha, int& betta, const int& depth, const Tile& whose_turn, const bool& isFirstIter);
int minValue(Tile* game, int& alpha, int& betta, const int& depth, const Tile& whose_turn, const bool& isFirstIter);

bool isTerminalState(Tile* game) {
	for (int i = 0; i < 3; i++) {
		if ((game[i * 3] != Tile::_ && game[i * 3] == game[i * 3 + 1] && game[i * 3] == game[i * 3 + 2]) ||
			(game[i] != Tile::_ && game[i] == game[3 + i] && game[i] == game[6 + i]))
			return true;
	}

	if ((game[0] != Tile::_ && game[0] == game[4] && game[0] == game[8]) ||
		(game[3] != Tile::_ && game[2] == game[4] && game[2] == game[6]))
		return true;

	for(int i = 0; i < 9; i++)
		if(game[i] == Tile::_)return false;

	return true;
}

int terminalStateValue(Tile* game, const int& depth) {
	for (int i = 0; i < 3; i++) {
		if (game[i * 3] != _ && game[i * 3] == game[i * 3 + 1] && game[i * 3] == game[i * 3 + 2])
			return (game[i * 3] == Tile::O) ? (depth - 10) : (10 - depth);
		if (game[i] != _ && game[i] == game[3 + i] && game[i] == game[6 + i])
			return (game[i] == Tile::O) ? (depth - 10) : (10 - depth);
	}

	if ((game[0] != _ && game[0] == game[4] && game[0] == game[8]) ||
		(game[3] != _ && game[2] == game[4] && game[2] == game[6]))
		return (game[4] == Tile::O) ? (depth - 10) : (10 - depth);

	return 0;
}

void judge(Tile game[], const Tile& whose_turn, const int& depth) {
	int value, alpha = INT8_MIN, betta = INT8_MAX;

	if (whose_turn == X) {
		value = maxValue(game, alpha, betta, depth, whose_turn, true);
	}
	else {
		value = minValue(game, alpha, betta, depth, whose_turn, true);

	}
	

	for (int i = 0; i < 9; i++) {
		if (valueArray[i] == value)
			std::cout << i / 3 + 1 << " " << i % 3 + 1 << std::endl;
	}
}

int maxValue(Tile* game, int& alpha, int& betta, const int& depth, const Tile& whose_turn, const bool& isFirstIter) {
	if (isTerminalState(game)) 
		return terminalStateValue(game, depth);

	int value = INT8_MIN;

	for (int i = 0; i < 9; i++) {
		if (game[i] != Tile::_)
			continue;

		game[i] = whose_turn;
		value = std::max(value, minValue(game, alpha, betta, depth + 1, (whose_turn == Tile::X) ? Tile::O : Tile::X, false));
		game[i] = Tile::_;
		if (value >= betta) {
			if (isFirstIter)
				valueArray[i] = value;
			return value;
		}
		alpha = std::max(alpha, value);
	}

	return value;
}

int minValue(Tile* game, int& alpha, int& betta, const int& depth, const Tile& whose_turn, const bool& isFirstIter) {
	if (isTerminalState(game))
		return terminalStateValue(game, depth);

	int value = INT8_MAX;

	for (int i = 0; i < 9; i++) {
		if (game[i] != Tile::_)
			continue;

		game[i] = whose_turn;
		value = std::min(value, maxValue(game, alpha, betta, depth + 1, (whose_turn == Tile::X) ? Tile::O : Tile::X,false));
		game[i] = Tile::_;
		if (value <= alpha) {
			if (isFirstIter) valueArray[i] = value;
			return value;
		}
		betta = std::min(betta, value);
	}

	return value;
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
		return _;
	}
}

void translateStateToGame(const std::string& state, Tile* game) {
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
	Tile game[9], buffer[9];
	int depth = 0;
	
	std::cin >> inputType;
	std::getline(std::cin, inputTurn); //clear the remaining line from cin >>
	std::getline(std::cin, inputTurn);
	for (int i = 0; i < 7; i++) {
		std::getline(std::cin, input);
		inputState += translateInputToState(input);
	}

	if (inputState.size() != 9) std::cout << "Wrong state input";

	translateStateToGame(inputState, game);
	translateStateToGame(inputState, buffer);

	for (int i = 0; i < 9; i++)
		depth += (game[i] != _) ? 1 : 0;

	if(inputType == "JUDGE"){
		if (inputTurn[5] == 'X') {
			judge(buffer, X, depth);
		} else if (inputTurn[5] == 'O') {
			judge(buffer, O, depth);
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

