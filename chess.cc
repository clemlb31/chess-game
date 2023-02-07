#include <cstddef>
#include <cstring>
#include <iostream>
#include <string>
#include <type_traits>
#include <vector>
using namespace std;

enum Color { Black = 0, White = 1 };
enum PieceType : char {
	Rook = 'R',
	Bishop = 'B',
	Knight = 'N',
	King = 'K',
	Queen = 'Q',
	Pawn = 'P'

};

enum File {
	file_a = 0,
	file_b = 1,
	file_c = 2,
	file_d = 3,
	file_e = 4,
	file_f = 5,
	file_g = 6,
	file_h = 7
};
enum Rank {
	rank_1 = 0,
	rank_2 = 1,
	rank_3 = 2,
	rank_4 = 3,
	rank_5 = 4,
	rank_6 = 5,
	rank_7 = 6,
	rank_8 = 7,
};
enum GameState { inProgess = 0, BlackWin = 1, WhiteWin = 2, quit = 3 };

class Piece {
       public:
	PieceType id;  // 0
	Color color;
	int posX;
	int posY;
	bool EnPassant;
	Piece(PieceType id, Color color, int X, int Y)
	    : id(id), color(color), posX(X), posY(Y),EnPassant(0) {}
	string toUtf();
};

class Square {
       private:
       public:
	Piece *piece;  // NULL if no piece
	void setSquare(class Piece *pointer) { this->piece = pointer; }
	string utf;
	void updateUtf();
};

string Piece::toUtf() {
	switch (id) {
	case Queen:
		if (color) {
			return "\u2655";
		} else {
			return "\u265B";
		}
		break;
	case King:
		if (color) {
			return "\u2654";
		} else {
			return "\u265A";
		}
		break;
	case Bishop:
		if (color) {
			return "\u2657";
		} else {
			return "\u265D";
		}
		break;
	case Rook:
		if (color) {
			return "\u2656";
		} else {
			return "\u265C";
		}
		break;
	case Knight:
		if (color) {
			return "\u2658";
		} else {
			return "\u265E";
		}
		break;
	case Pawn:
		if (color) {
			return "\u2659";
		} else {
			return "\u265F";
		}
		break;
	default:
		return " ";
	}
}

void Square::updateUtf() {
	if (piece == nullptr) {
		utf = " ";
	} else {
		utf = piece->toUtf();
	}
}

class Game {
       private:
	int state;
	Piece *piecesW[16];
	Piece *piecesB[16];

       public:
	int moveInt[4];
	Game();
	string move;
	int turn;
	Square board[8][8];
	void updateBoard();
	int moveToFile(int nb);
	int moveToRank(int nb);
	int getState() { return state; }
	void getmove();
	void play();
	int testWrongmove();
	int pathIsClear(int x1, int y1, int x2, int y2);
	~Game() {
		for (int i(0); i < 16; i++) {
			delete piecesB[i];
			delete piecesW[i];
		}
	}
};
int Game::pathIsClear(int x1, int y1, int x2, int y2) {
	if (x1 == x2) {
		if (y1 < y2) {
			for (int i(y1 + 1); i < y2; i++) {
				if (board[x1][i].piece != nullptr) {
					return 0;
				}
			}
		} else {
			for (int i(y1 - 1); i > y2; i--) {
				if (board[x1][i].piece != nullptr) {
					return 0;
				}
			}
		}
	} else if (y1 == y2) {
		if (x1 < x2) {
			for (int i(x1 + 1); i < x2; i++) {
				if (board[i][y1].piece != nullptr) {
					return 0;
				}
			}
		} else {
			for (int i(x1 - 1); i > x2; i--) {
				if (board[i][y1].piece != nullptr) {
					return 0;
				}
			}
		}
	} else {
		if (x1 < x2) {
			if (y1 < y2) {
				for (int i(1); i < x2 - x1; i++) {
					if (board[x1 + i][y1 + i].piece !=
					    nullptr) {
						return 0;
					}
				}
			} else {
				for (int i(1); i < x2 - x1; i++) {
					if (board[x1 + i][y1 - i].piece !=
					    nullptr) {
						return 0;
					}
				}
			}
		} else {
			if (y1 < y2) {
				for (int i(1); i < x1 - x2; i++) {
					if (board[x1 - i][y1 + i].piece !=
					    nullptr) {
						return 0;
					}
				}
			} else {
				for (int i(1); i < x1 - x2; i++) {
					if (board[x1 - i][y1 - i].piece !=
					    nullptr) {
						return 0;
					}
				}
			}
		}
	}
	return 1;
}
int Game::moveToFile(int nb) { return move[nb] - 'a'; }
int Game::moveToRank(int nb) { return move[nb] - '1'; }
int Game::testWrongmove() {
	if (moveInt[0] < 0 || moveInt[0] > 7 || moveInt[1] < 0 ||
	    moveInt[1] > 7 || moveInt[2] < 0 || moveInt[2] > 7 ||
	    moveInt[3] < 0 || moveInt[3] > 7) {
		cout << "erreur de saisie" << endl;
		return 0;
	}
	switch (board[moveInt[0]][moveInt[1]].piece->id) {
	case Rook:
		if (moveInt[0] != moveInt[2] && moveInt[1] != moveInt[3]) {
			cout << "mouvement illegal" << endl;
			return 0;
		}
		if (!pathIsClear(moveInt[0], moveInt[1], moveInt[2],
				 moveInt[3])) {
			cout << "chemin bloqué" << endl;
			return 0;
		}
		break;
	case Bishop:
		if (abs(moveInt[0] - moveInt[2]) !=
		    abs(moveInt[1] - moveInt[3])) {
			cout << "mouvement illegal" << endl;
			return 0;
		}
		if (!pathIsClear(moveInt[0], moveInt[1], moveInt[2],
				 moveInt[3])) {
			cout << "chemin bloqué" << endl;
			return 0;
		}
		break;
	case Knight:
		if ((abs(moveInt[0] - moveInt[2]) != 2 &&
		     abs(moveInt[1] - moveInt[3]) != 2) ||
		    (abs(moveInt[0] - moveInt[2]) != 1 &&
		     abs(moveInt[1] - moveInt[3]) != 1)) {
			cout << "mouvement illegal" << endl;
			return 0;
		}
		break;
	case King:
		if (abs(moveInt[0] - moveInt[2]) > 1 ||
		    abs(moveInt[1] - moveInt[3]) > 1) {
			cout << "mouvement illegal" << endl;
			return 0;
		}
		break;
	case Queen:
		if (abs(moveInt[0] - moveInt[2]) !=
			abs(moveInt[1] - moveInt[3]) &&
		    moveInt[0] != moveInt[2] && moveInt[1] != moveInt[3]) {
			cout << "mouvement illegal" << endl;
			return 0;
		}
		if (!pathIsClear(moveInt[0], moveInt[1], moveInt[2],
				 moveInt[3])) {
			cout << "chemin bloqué" << endl;
			return 0;
		}
		break;
	case Pawn:
		if (board[moveInt[0]][moveInt[1]].piece->color == White) {
			if (moveInt[1] > moveInt[3]) {
				cout << "mouvement illegal" << endl;
				return 0;
			}
		} else {
			if (moveInt[1] < moveInt[3]) {
				cout << "mouvement illegal" << endl;
				return 0;
			}
		}

		if (moveInt[0] != moveInt[2] ||
		    abs(moveInt[1] - moveInt[3]) > 2) {
			if ((abs(moveInt[0] - moveInt[2]) != 1) ||
			    (abs(moveInt[1] - moveInt[3]) != 1)) {
				cout << "mouvement illegal" << endl;
				return 0;
			}
			if(board[moveInt[2]][moveInt[3]].piece == nullptr){
				cout << "mouvement illegal" << endl;
				return 0;
			}
			if(board[moveInt[2]][moveInt[3]].piece->color == turn){
				cout << "mouvement illegal" << endl;
				return 0;
			}
		}

		if (abs(moveInt[1] - moveInt[3]) == 2) {
			if (moveInt[1] != 1 && moveInt[1] != 6) {
				cout << "mouvement illegal" << endl;
				return 0;
			}
		}
		if (!pathIsClear(moveInt[0], moveInt[1], moveInt[2],
				 moveInt[3])) {
			cout << "chemin bloqué" << endl;
			return 0;
		}
		break;
	}

	if (board[moveInt[2]][moveInt[3]].piece != nullptr) {
		if (board[moveInt[2]][moveInt[3]].piece->color == turn) {
			cout << "piece de votre couleur sur la case d'arrivée"
			     << endl;
			return 0;
		} else {
			board[moveInt[2]][moveInt[3]].piece->posX = -1;
			board[moveInt[2]][moveInt[3]].piece->posY = -1;
		}
	}
	if (board[moveInt[0]][moveInt[1]].piece == nullptr) {
		cout << "pas de piece sur la case de départ" << endl;
		return 0;
	}
	if (board[moveInt[0]][moveInt[1]].piece->color != turn) {
		cout << "pas votre piece" << endl;
		return 0;
	}
	return 1;
}
void Game::play() {
	do {
		getmove();
		if (move == "/quit") {
			state = quit;
			return;
		} else {
			moveInt[0] = moveToFile(0);
			moveInt[1] = moveToRank(1);
			moveInt[2] = moveToFile(2);
			moveInt[3] = moveToRank(3);
		}
		cout << moveInt[0] << moveInt[1] << moveInt[2] << moveInt[3]
		     << endl;
	} while (!testWrongmove());
	board[moveInt[0]][moveInt[1]].piece->posX = moveInt[2];
	board[moveInt[0]][moveInt[1]].piece->posY = moveInt[3];
	turn = (turn + 1) % 2;
}

void Game::getmove() {
	if (turn) {
		cout << "White -> ";
	} else {
		cout << "Black -> ";
	}
	cin >> move;
}
Game::Game() {
	turn = 1;
	state = inProgess;
	piecesB[0] = new Piece(Rook, Black, file_a, rank_8);
	piecesB[1] = new Piece(Knight, Black, file_b, rank_8);
	piecesB[2] = new Piece(Bishop, Black, file_c, rank_8);
	piecesB[3] = new Piece(Queen, Black, file_d, rank_8);
	piecesB[4] = new Piece(King, Black, file_e, rank_8);
	piecesB[5] = new Piece(Bishop, Black, file_f, rank_8);
	piecesB[6] = new Piece(Knight, Black, file_g, rank_8);
	piecesB[7] = new Piece(Rook, Black, file_h, rank_8);

	piecesW[0] = new Piece(Rook, White, file_a, rank_1);
	piecesW[1] = new Piece(Knight, White, file_b, rank_1);
	piecesW[2] = new Piece(Bishop, White, file_c, rank_1);
	piecesW[3] = new Piece(Queen, White, file_d, rank_1);
	piecesW[4] = new Piece(King, White, file_e, rank_1);
	piecesW[5] = new Piece(Bishop, White, file_f, rank_1);
	piecesW[6] = new Piece(Knight, White, file_g, rank_1);
	piecesW[7] = new Piece(Rook, White, file_h, rank_1);
	for (int i(0); i < 8; i++) {
		piecesB[i + 8] = new Piece(Pawn, Black, i, rank_7);
		piecesW[i + 8] = new Piece(Pawn, White, i, rank_2);
	}
	updateBoard();
}
void printBoard(Game *game) {
	game->updateBoard();
	for (int i(0); i < 64; i++) {
		game->board[i / 8][i % 8].updateUtf();
	}
	cout << "    a   b   c   d   e   f   g   h " << endl;
	cout << "  ┌───┬───┬───┬───┬───┬───┬───┬───┐" << endl;
	for (int i(7); i >= 0; i--) {
		cout << i + 1 << " │ ";
		for (int j(0); j < 8; j++) {
			cout << game->board[j][i].utf << " │ ";
		}
		cout << endl;
		if (i) {
			cout << "  ├───┼───┼───┼───┼───┼───┼───┼───┤" << endl;
		}
	}
	cout << "  └───┴───┴───┴───┴───┴───┴───┴───┘" << endl;
}
void Game::updateBoard() {
	for (int i(0); i < 64; i++) {
		board[i / 8][i % 8].setSquare(nullptr);
	}
	for (int i(0); i < 16; i++) {
		if (piecesB[i]->posX != -1 && piecesB[i]->posY != -1) {
			board[piecesB[i]->posX][piecesB[i]->posY].setSquare(
			    piecesB[i]);
		}
		if (piecesW[i]->posX != -1 && piecesW[i]->posY != -1) {
			board[piecesW[i]->posX][piecesW[i]->posY].setSquare(
			    piecesW[i]);
		}
	}
}

int main() {
	int Elisa = 8;
	(void)Elisa;
	Game game;
	while (!game.getState()) {
		printBoard(&game);
		game.play();
	}
	return 1;
}
