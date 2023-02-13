#include <cstddef>
#include <cstring>
#include <iostream>
#include <string>
#include <type_traits>
#include <vector>
#include <fstream>
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
enum GameState { inProgess = 0, BlackWin = 1, WhiteWin = 2, Stalemate = 3, quit = 4 };

enum errorMove {
	noError = 0,
	wrongColor = 1,
	wrongPiece = 2,
	wrongMove = 3,
	wrongPath = 4,
	wrongCastling = 5,
	wrongPromotion = 6
};

class Piece {
       public:
	PieceType id;
	Color color;
	int posX;
	int posY;
	bool EnPassant;
	bool hasMoved;
	Piece(PieceType id, Color color, int X, int Y)
	    : id(id), color(color), posX(X), posY(Y), EnPassant(0), hasMoved(false) {}
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
	GameState state;
	Piece *piecesW[16];
	Piece *piecesB[16];
	int prevousPositions[32][2];
	int previousState[32][2];  // 0 = Enpassant, 1 = hasMoved

       public:
	ofstream file;
	int moveInt[4];
	Game();
	string move;
	bool turn;
	Square board[8][8];
	int isTheatened(int x, int y, Color color);
	void updateBoard();
	int moveToFile(int nb);
	int moveToRank(int nb);
	void resetEnPassant();
	void savePositions();
	void undoMove();
	bool check;
	int isCheckMate(int color);
	int isCheck(int color);
	int isStalemate(int color);
	int getState() { return state; }
	void getmove();
	void play();
	int moveIsLegal(int x1, int y1, int x2, int y2);
	int movePiece();
	int shortCastling();
	int longCastling();
	int pathIsClear(int x1, int y1, int x2, int y2);
	void quitGame();
	int promotePawn();
	~Game() {
		for (int i(0); i < 16; i++) {
			delete piecesB[i];
			delete piecesW[i];
		}
		file.close();
	}
};
void Game::savePositions() {
	for (int i(0); i < 16; i++) {
		prevousPositions[i][0] = piecesB[i]->posX;
		prevousPositions[i][1] = piecesB[i]->posY;
		previousState[i][0] = piecesB[i]->EnPassant;
		previousState[i][1] = piecesB[i]->hasMoved;
		prevousPositions[i + 16][0] = piecesW[i]->posX;
		prevousPositions[i + 16][1] = piecesW[i]->posY;
		previousState[i + 16][0] = piecesW[i]->EnPassant;
		previousState[i + 16][1] = piecesW[i]->hasMoved;
	}
}
void Game::undoMove() {
	for (int i(0); i < 16; i++) {
		piecesB[i]->posX = prevousPositions[i][0];
		piecesB[i]->posY = prevousPositions[i][1];
		piecesB[i]->EnPassant = previousState[i][0];
		piecesB[i]->hasMoved = previousState[i][1];
		piecesW[i]->posX = prevousPositions[i + 16][0];
		piecesW[i]->posY = prevousPositions[i + 16][1];
		piecesW[i]->EnPassant = previousState[i + 16][0];
		piecesW[i]->hasMoved = previousState[i + 16][1];
	}
}

int Game::isTheatened(int x, int y, Color color) {
	if (color == White) {
		for (int i(0); i < 16; i++) {
			if (moveIsLegal(piecesB[i]->posX, piecesB[i]->posY, x, y)) {
				return 1;
			}
		}
	} else {
		for (int i(0); i < 16; i++) {
			if (moveIsLegal(piecesW[i]->posX, piecesW[i]->posY, x, y)) {
				return 1;
			}
		}
	}
	return 0;
}
int Game::promotePawn() {
	int x2(moveInt[2]);
	int y2(moveInt[3]);
	if (board[x2][y2].piece->id == Pawn) {
		if (board[x2][y2].piece->color == White) {
			if (y2 == 7) {
				cout << "Promote to (Q)ueen, (R)ook, (B)ishop "
					"or (K)night?"
				     << endl;
				char choice;
				cin >> choice;
				switch (choice) {
				case 'Q':
					board[x2][y2].piece->id = Queen;
					break;
				case 'R':
					board[x2][y2].piece->id = Rook;
					break;
				case 'B':
					board[x2][y2].piece->id = Bishop;
					break;
				case 'K':
					board[x2][y2].piece->id = Knight;
					break;
				default:
					cout << "Invalid choice" << endl;
					return 0;
				}
			}
		} else {
			if (y2 == 0) {
				cout << "Promote to (Q)ueen, (R)ook, (B)ishop "
					"or (K)night?"
				     << endl;
				char choice;
				cin >> choice;
				switch (choice) {
				case 'Q':
					board[x2][y2].piece->id = Queen;
					break;
				case 'R':
					board[x2][y2].piece->id = Rook;
					break;
				case 'B':
					board[x2][y2].piece->id = Bishop;
					break;
				case 'K':
					board[x2][y2].piece->id = Knight;
					break;
				default:
					cout << "Invalid choice" << endl;
					return 0;
				}
			}
		}
	}
	return 1;
}
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
	} else if (x1 < x2) {
		if (y1 < y2) {
			for (int i(1); i < x2 - x1; i++) {
				if (board[x1 + i][y1 + i].piece != nullptr) {
					return 0;
				}
			}
		} else {
			for (int i(1); i < x2 - x1; i++) {
				if (board[x1 + i][y1 - i].piece != nullptr) {
					return 0;
				}
			}
		}
	} else {
		if (y1 < y2) {
			for (int i(1); i < x1 - x2; i++) {
				if (board[x1 - i][y1 + i].piece != nullptr) {
					return 0;
				}
			}
		} else {
			for (int i(1); i < x1 - x2; i++) {
				if (board[x1 - i][y1 - i].piece != nullptr) {
					return 0;
				}
			}
		}
	}
	return 1;
}
int Game::moveToFile(int nb) { return move[nb] - 'a'; }
int Game::moveToRank(int nb) { return move[nb] - '1'; }
int Game::moveIsLegal(int x1, int y1, int x2, int y2) {
	switch (board[x1][y1].piece->id) {
	case Rook:
		if (x1 != x2 && y1 != y2) {
			return 0;
		}
		if (!pathIsClear(x1, y1, x2, y2)) {
			return 0;
		}
		resetEnPassant();
		break;
	case Bishop:
		if (abs(x1 - x2) != abs(y1 - y2)) {
			return 0;
		}
		if (!pathIsClear(x1, y1, x2, y2)) {
			return 0;
		}
		resetEnPassant();
		break;
	case Knight:
		if ((abs(x1 - x2) != 2 && abs(y1 - y2) != 2) ||
		    (abs(x1 - x2) != 1 && abs(y1 - y2) != 1)) {
			return 0;
		}
		resetEnPassant();
		break;
	case King:
		if (abs(x1 - x2) > 1 || abs(y1 - y2) > 1) {
			return 0;
		}
		resetEnPassant();
		break;
	case Queen:
		if (abs(x1 - x2) != abs(y1 - y2) && x1 != x2 && y1 != y2) {
			return 0;
		}
		if (!pathIsClear(x1, y1, x2, y2)) {
			return 0;
		}
		resetEnPassant();
		break;

	case Pawn:

		if (board[x1][y1].piece->color == White) {
			if (y1 > y2) {
				return 0;
			}
		} else {
			if (y1 < y2) {
				return 0;
			}
		}

		if (x1 != x2 || abs(y1 - y2) > 2) {
			if ((abs(x1 - x2) != 1) || (abs(y1 - y2) != 1)) {
				return 0;
			}
			if (board[x2][y2].piece == nullptr) {
				if (board[x2][y1].piece == nullptr) {
					return 0;
				}
				if (board[x2][y1].piece->EnPassant == true) {
					board[x2][y1].piece->posX = -1;
					board[x2][y1].piece->posY = -1;
					return 1;
				}
				return 0;
			}
		}
		if (abs(y1 - y2) == 1 && x1 == x2 && board[x2][y2].piece != nullptr) {
			return 0;
		}
		if (abs(y1 - y2) == 2) {
			if (pathIsClear(x1, y1, x2, y2)) {
				board[x1][y1].piece->EnPassant = true;
			}
			if (y1 != 1 && y1 != 6) {
				return 0;
			}
		} else {
			resetEnPassant();
		}
		break;
	}
	return 1;
}

void Game::resetEnPassant() {
	for (int i(0); i < 8; i++) {
		for (int j(0); j < 8; j++) {
			if (board[i][j].piece != nullptr) {
				if (board[i][j].piece->color == turn) {
					board[i][j].piece->EnPassant = false;
				}
			}
		}
	}
}
int Game::movePiece() {
	if (moveInt[0] < 0 || moveInt[0] > 7 || moveInt[1] < 0 || moveInt[1] > 7 ||
	    moveInt[2] < 0 || moveInt[2] > 7 || moveInt[3] < 0 || moveInt[3] > 7) {
		return 0;
	}
	if (board[moveInt[0]][moveInt[1]].piece == nullptr) {
		cout << "No piece on the case" << endl;
		return 0;
	}
	if (board[moveInt[0]][moveInt[1]].piece->color != turn) {
		cout << "Not your piece" << endl;
		return 0;
	}
	if (board[moveInt[2]][moveInt[3]].piece != nullptr) {
		if (board[moveInt[2]][moveInt[3]].piece->color == turn) {
			cout << "You can't take your own piece" << endl;
			return 0;
		}
	}
	if (!moveIsLegal(moveInt[0], moveInt[1], moveInt[2], moveInt[3])) {
		cout << "Move is not legal" << endl;
		return 0;
	}
	if (board[moveInt[2]][moveInt[3]].piece != nullptr) {
		board[moveInt[2]][moveInt[3]].piece->posX = -1;
		board[moveInt[2]][moveInt[3]].piece->posY = -1;
	}
	board[moveInt[0]][moveInt[1]].piece->posX = moveInt[2];
	board[moveInt[0]][moveInt[1]].piece->posY = moveInt[3];
	board[moveInt[0]][moveInt[1]].piece->hasMoved = true;
	if (check) {
		updateBoard();
		if (isCheck(turn)) {
			cout << "You can't move you're in check" << endl;
			undoMove();

			updateBoard();
			return 0;
		} else {
			check = false;
		}
	}
	return 1;
}

int Game::isCheck(int color) {
	int kingX, kingY;
	if (color == White) {
		kingX = piecesW[4]->posX;
		kingY = piecesW[4]->posY;
	} else {
		kingX = piecesB[4]->posX;
		kingY = piecesB[4]->posY;
	}
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			if (board[i][j].piece != nullptr) {
				if (board[i][j].piece->color != color) {
					if (moveIsLegal(i, j, kingX, kingY)) {
						return 1;
					}
				}
			}
		}
	}
	return 0;
}
int Game::isCheckMate(int color) {
	for (int i = 0; i < 64; i++) {
		if (board[i % 8][i / 8].piece != nullptr &&
		    board[i % 8][i / 8].piece->color == color) {
			for (int j = 0; j < 64; j++) {
				if ((board[j % 8][j / 8].piece != nullptr &&
				     board[j % 8][j / 8].piece->color != color &&
				     board[j % 8][j / 8].piece->id != King) ||
				    board[j % 8][j / 8].piece == nullptr) {
					if (moveIsLegal(i % 8, i / 8, j % 8, j / 8)) {
						board[i % 8][i / 8].piece->posX = j % 8;
						board[i % 8][i / 8].piece->posY = j / 8;
						updateBoard();
						if (!isCheck(color)) {
							undoMove();
							return 0;
						}
						undoMove();
						updateBoard();
					}
				}
			}
		}
	}
	return 1;
}
int Game::isStalemate(int color) {
	for (int i = 0; i < 64; i++) {
		if (board[i % 8][i / 8].piece != nullptr &&
		    board[i % 8][i / 8].piece->color == color) {
			for (int j = 0; j < 64; j++) {
				if ((board[j % 8][j / 8].piece != nullptr &&
				     board[j % 8][j / 8].piece->color != color) ||
				    board[j % 8][j / 8].piece == nullptr) {
					if (moveIsLegal(i % 8, i / 8, j % 8, j / 8)) {
						board[i % 8][i / 8].piece->posX = j % 8;
						board[i % 8][i / 8].piece->posY = j / 8;
						updateBoard();
						if (!isCheck(color)) {
							undoMove();
							return 0;
						}
						undoMove();
						updateBoard();
					}
				}
			}
		}
	}
	return 1;
}

int Game::shortCastling() {
	if (turn == White) {
		if (piecesW[4]->hasMoved == false && piecesW[7]->hasMoved == false) {
			if (board[5][0].piece == nullptr && board[6][0].piece == nullptr) {
				if (isTheatened(4, 0, White) || isTheatened(5, 0, White) ||
				    isTheatened(6, 0, White)) {
					return 0;
				}
				board[4][0].piece->posX = 6;
				board[4][0].piece->posY = 0;
				board[7][0].piece->posX = 5;
				board[7][0].piece->posY = 0;
				return 1;
			}
		}

	} else {
		if (piecesB[4]->hasMoved == false && piecesB[7]->hasMoved == false) {
			if (board[5][7].piece == nullptr && board[6][7].piece == nullptr) {
				if (isTheatened(4, 7, Black) || isTheatened(5, 7, Black) ||
				    isTheatened(6, 7, Black)) {
					return 0;
				}
				board[4][7].piece->posX = 6;
				board[4][7].piece->posY = 7;
				board[7][7].piece->posX = 5;
				board[7][7].piece->posY = 7;
				return 1;
			}
		}
	}
	return 0;
}
int Game::longCastling() {
	if (turn == White) {
		if (piecesW[4]->hasMoved == false && piecesW[0]->hasMoved == false) {
			if (board[1][0].piece == nullptr && board[2][0].piece == nullptr &&
			    board[3][0].piece == nullptr) {
				if (isTheatened(4, 0, White) || isTheatened(3, 0, White) ||
				    isTheatened(2, 0, White) || isTheatened(1, 0, White)) {
					return 0;
				}
				board[4][0].piece->posX = 2;
				board[4][0].piece->posY = 0;
				board[0][0].piece->posX = 3;
				board[0][0].piece->posY = 0;
				return 1;
			}
		}

	} else {
		if (piecesB[4]->hasMoved == false && piecesB[0]->hasMoved == false) {
			if (board[1][7].piece == nullptr && board[2][7].piece == nullptr &&
			    board[3][7].piece == nullptr) {
				if (isTheatened(4, 7, Black) || isTheatened(3, 7, Black) ||
				    isTheatened(2, 7, Black) || isTheatened(1, 7, Black)) {
					return 0;
				}
				board[4][7].piece->posX = 2;
				board[4][7].piece->posY = 7;
				board[0][7].piece->posX = 3;
				board[0][7].piece->posY = 7;
				return 1;
			}
		}
	}
	return 0;
}
void Game::play() {
	do {
		if (check) {
			cout << "check" << endl;
		}
		getmove();
		if (move == "/quit") {
			state = quit;
			return;
		}
		if (move == "O-O") {
			if (shortCastling()) {
			cout << "short castling" << endl;
				break;
			}
		}
		if (move == "O-O-O") {
			if (longCastling()) {
			cout << "long castling" << endl;
				break;
			}
		} else {
			moveInt[0] = moveToFile(0);
			moveInt[1] = moveToRank(1);
			moveInt[2] = moveToFile(2);
			moveInt[3] = moveToRank(3);
		}
	} while (!movePiece());
	updateBoard();
	if (isCheck(turn)) {
cout << "This move put in check" << endl;
		undoMove();
		play();
		return;
	}
	while (!promotePawn()) {
	}
	file << move << endl;
	savePositions();
	if (isCheck(!turn)) {
		if (isCheckMate(!turn)) {
			if (turn == White) {
				state = WhiteWin;
			} else {
				state = BlackWin;
			}
			return;
		}
		check = true;
	} else {
		if (isStalemate(!turn)) {
			state = Stalemate;
			return;
		}
	}
	turn = !turn;
}
void Game::quitGame() {
	updateBoard();
	switch (state) {
	case WhiteWin:
		cout << "White Win" << endl;
		break;
	case BlackWin:
		cout << "Black Win" << endl;
		break;
	case Stalemate:
		cout << "Stalemate" << endl;
		break;
	case quit:
		cout << "Game quited" << endl;
		break;
	default:
		break;
	}
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			if (board[j][i].piece != nullptr) {
				if (board[j][i].piece->color == White) {
					cout << "W";
				} else {
					cout << "B";
				}

				cout << board[j][i].piece->id;
			}
			if (j != 7 || i != 7) {
				cout << ",";
			}
		}
	}
	cout << endl;
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
	check = false;
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
	ofstream file("partie.txt");

	/* 	for(int i(0); i < 16; i++){
			piecesB[i]->posX = -1;
			piecesB[i]->posY = -1;
		} */
	/* 	piecesB[4]->posX = file_a;
		piecesB[4]->posY = rank_8;
		piecesB[8]->posX = file_a;
		piecesB[8]->posY = rank_5;
		piecesW[0]->posX = file_b;
		piecesW[0]->posY = rank_3;
		piecesW[7]->posY = rank_7; */
	/* 	piecesB[8]->posY = rank_3;
		piecesW[8]->posY = rank_7;
		piecesB[0]->posY = rank_4;
		piecesB[1]->posY = rank_4;
		piecesB[2]->posY = rank_4;
		piecesB[3]->posY = rank_4;
		piecesW[0]->posY = rank_8;
		piecesB[11]->posY = rank_3; */

	savePositions();
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
			board[piecesB[i]->posX][piecesB[i]->posY].setSquare(piecesB[i]);
		}
		if (piecesW[i]->posX != -1 && piecesW[i]->posY != -1) {
			board[piecesW[i]->posX][piecesW[i]->posY].setSquare(piecesW[i]);
		}
	}
}

int main() {
	Game game;
	
	while (!game.getState()) {
		printBoard(&game);
		game.play();
	}
	printBoard(&game);
	game.quitGame();
	return 1;
}
