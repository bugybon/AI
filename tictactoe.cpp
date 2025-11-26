#include <iostream>
#include <string>

enum Tile {
	X = 1,
	_ = 0,
	O = -1
};

struct Move {
	int value;
	int row;
	int col;
};

struct Coords {
	int row;
	int col;
};

int valueArray[9];

Move maxValue(Tile* game, const int& alpha, const int& betta, const int& depth, const Tile& whose_turn, const bool& isFirstIter);
Move minValue(Tile* game, const int& alpha, const int& betta, const int& depth, const Tile& whose_turn, const bool& isFirstIter);

std::string printTile(const Tile& tile) {
	if (tile == Tile::X)	return "X";
	else if (tile == Tile::O) return "O";
	else return "_";
}

void printState(Tile* game) {
	std::cout << "+---+---+---+" << std::endl;
	for (int row = 0; row < 3; row++) {
		for (int col = 0; col < 3; col++) {
			std::cout << "| " << printTile(game[row * 3 + col]) << " ";
		}
		std::cout << "|" << std::endl << "+---+---+---+" << std::endl;
	}
}

Tile reverseTile(const Tile& tile) {
	if (tile == Tile::O) return Tile::X;
	else if (tile == Tile::X) return Tile::O;
	else return Tile::_;
}

bool isTerminalState(Tile* game) {
	for (int i = 0; i < 3; i++) {
		if ((game[i * 3] != Tile::_ && game[i * 3] == game[i * 3 + 1] && game[i * 3] == game[i * 3 + 2]) ||
			(game[i] != Tile::_ && game[i] == game[3 + i] && game[i] == game[6 + i]))
			return true;
	}

	if ((game[0] != Tile::_ && game[0] == game[4] && game[0] == game[8]) ||
		(game[2] != Tile::_ && game[2] == game[4] && game[2] == game[6]))
		return true;

	for(int i = 0; i < 9; i++)
		if(game[i] == Tile::_)return false;

	return true;
}

int terminalStateValue(Tile* game, const int& depth) {
	for (int i = 0; i < 3; i++) {
		if (game[i * 3] != Tile::_ && game[i * 3] == game[i * 3 + 1] && game[i * 3] == game[i * 3 + 2])
			return (game[i * 3] == Tile::O) ? (depth - 10) : (10 - depth);
		if (game[i] != Tile::_ && game[i] == game[3 + i] && game[i] == game[6 + i])
			return (game[i] == Tile::O) ? (depth - 10) : (10 - depth);
	}

	if ((game[0] != Tile::_ && game[0] == game[4] && game[0] == game[8]) ||
		(game[2] != Tile::_ && game[2] == game[4] && game[2] == game[6]))
		return (game[4] == Tile::O) ? (depth - 10) : (10 - depth);

	return 0;
}

Move maxValue(Tile* game, const int& alpha, const int& betta, const int& depth, const Tile& whose_turn, const bool& isFirstIter) {
	if (isTerminalState(game)) 
		return { terminalStateValue(game, depth), -1, -1 };

	int value = INT8_MIN, row = -1, col = -1, next_alpha = alpha, candidate_value;

	for (int i = 0; i < 9; i++) {
		if (game[i] != Tile::_)
			continue;
		game[i] = whose_turn;

		candidate_value = minValue(game, next_alpha, betta, depth + 1, reverseTile(whose_turn), false).value;
		if(value < candidate_value) {
			if (isFirstIter) {
				row = 1 + i / 3;
				col = 1 + i % 3;
			}
			value = candidate_value;
		}
		game[i] = Tile::_;

		if (value >= betta)
			return { value, row, col };

		next_alpha = std::max(next_alpha, value);
	}

	return { value, row, col };
}

Move minValue(Tile* game, const int& alpha, const int& betta, const int& depth, const Tile& whose_turn, const bool& isFirstIter) {
	if (isTerminalState(game))
		return { terminalStateValue(game, depth), -1, -1 };

	int col = -1, row = -1,value= INT8_MAX,next_betta = betta, candidate_value;

	for (int i = 0; i < 9; i++) {
		if (game[i] != Tile::_)
			continue;

		game[i] = whose_turn;
		candidate_value = maxValue(game, alpha, next_betta, depth + 1, reverseTile(whose_turn), false).value;
		if (candidate_value < value){
			if (isFirstIter) {
				row = 1 + i / 3;
				col = 1 + i % 3;
			}
			value = candidate_value;
		}	
		game[i] = Tile::_;

		if (value <= alpha) 
			return { value, row, col };

		next_betta = std::min(next_betta, value);
	}

	return { value, row, col};
}

Coords judge(Tile* game, const Tile& whose_turn, const int& depth) {
	Move value;
	int alpha = INT8_MIN, betta = INT8_MAX;

	if (whose_turn == X) {
		value = maxValue(game, alpha, betta, depth, whose_turn, true);
	}
	else {
		value = minValue(game, alpha, betta, depth, whose_turn, true);

	}
	//std::cout << "Solution is with value = " << value.value << std::endl;
	return { value.row, value.col };
}

void gameType(Tile* game, const Tile& whose_turn, const Tile& human_turn) {
	int depth = 0, rowInput, colInput;

	for (int i = 0; i < 9; i++)
		depth += (game[i] != _) ? 1 : 0;

	Coords bot;
	Tile current_turn = whose_turn;
	while (!isTerminalState(game)) {
		if (current_turn == human_turn) {
			do {
				std::cin >> rowInput >> colInput;
			} while (game[(rowInput - 1) * 3 + colInput - 1] != Tile::_);
			game[(rowInput - 1) * 3 + colInput - 1] = current_turn;
		}
		else {
			bot = judge(game, current_turn, depth);
			std::cout << bot.row << " " << bot.col << std::endl;
			game[(bot.row - 1) * 3 + bot.col - 1] = current_turn;
		}

		printState(game);
		depth++;
		current_turn = reverseTile(current_turn);
	}

	int result = terminalStateValue(game, depth);
	if (result < 0)
		std::cout << "WINNER: O" << std::endl;
	else if (result > 0)
		std::cout << "WINNER: X" << std::endl;
	else
		std::cout << "DRAW" << std::endl;
}

Tile translateCharToTile(const char& ch) {
	switch (ch) {
	case 'X':
		return Tile::X;
	case 'O':
		return Tile::O;
	case '_':
		return Tile::_;
	default:
		std::cout << "Char not a tile";
		return Tile::_;
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

int main() {
	std::string inputType, inputTurn, inputState,inputHumanTurn, input;
	Tile game[9], buffer[9];
	int depth = 0;
	Coords result;
	
	std::cin >> inputType;
	std::getline(std::cin, inputTurn); //clear the remaining line from cin >>

	if(inputType == "JUDGE"){
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

		result = judge(buffer, translateCharToTile(inputTurn[5]), depth);

		if (result.row == -1 && result.col == -1)
			std::cout << -1 << std::endl;
		else
			std::cout << result.row << " " << result.col << std::endl;
	}
	else if (inputType == "GAME") {
		std::getline(std::cin, inputTurn);
		std::getline(std::cin, inputHumanTurn);
		for (int i = 0; i < 7; i++) {
			std::getline(std::cin, input);
			inputState += translateInputToState(input);
		}

		if (inputState.size() != 9) std::cout << "Wrong state input";

		translateStateToGame(inputState, game);
		translateStateToGame(inputState, buffer);

		gameType(buffer, translateCharToTile(inputTurn[6]), translateCharToTile(inputHumanTurn[6]));
	}
	else {
		std::cout << "Not a game type";
	}

	return 0;
}

