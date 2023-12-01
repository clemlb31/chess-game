#include "game.hh"

#include <fstream>
#include <iostream>
#include <string>

#include "chess.hh"
#include "piece.hh"
using namespace std;

// constructeur de la classe Game
// initialise les pieces, le plateau et les variables du jeu
Game::Game() {
	check = false;
	turn = 1;
	state = inProgess;
	piecesB[0] = new Rook(Black, file_a, rank_8);
	piecesB[1] = new Knight(Black, file_b, rank_8);
	piecesB[2] = new Bishop(Black, file_c, rank_8);
	piecesB[3] = new Queen(Black, file_d, rank_8);
	piecesB[4] = new King(Black, file_e, rank_8);
	piecesB[5] = new Bishop(Black, file_f, rank_8);
	piecesB[6] = new Knight(Black, file_g, rank_8);
	piecesB[7] = new Rook(Black, file_h, rank_8);

	piecesW[0] = new Rook(White, file_a, rank_1);
	piecesW[1] = new Knight(White, file_b, rank_1);
	piecesW[2] = new Bishop(White, file_c, rank_1);
	piecesW[3] = new Queen(White, file_d, rank_1);
	piecesW[4] = new King(White, file_e, rank_1);
	piecesW[5] = new Bishop(White, file_f, rank_1);
	piecesW[6] = new Knight(White, file_g, rank_1);
	piecesW[7] = new Rook(White, file_h, rank_1);
	for (int i(0); i < 8; i++) {
		piecesB[i + 8] = new Pawn(Black, i, rank_7);
		piecesW[i + 8] = new Pawn(White, i, rank_2);
	}
	filePartie.open("last_game.txt");

	savePositions();
	updateBoard();
}

// Destructeur de la classe Game
Game::~Game() {
	for (int i(0); i < 16; i++) {
		delete piecesB[i];
		delete piecesW[i];
	}
	filePartie.close();
}

// fonction qui gere le déroulement d'un tour
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
	filePartie << move << endl;
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

GameState Game::getState() { return state; }

// sauvegarde la position des pièces sur le plateau ainsi que leurs états
// dans le but d'annuler les coups "virtuels" pour tester les coups légaux
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


// vérifie si une case est attaquée par une pièce adverse
int Game::isTheatened(int x, int y, Color color) {
	// pour chaque pièce adverse on vérifie si elle peut légalement aller à la case en question
	if (color == White) {
		for (int i(0); i < 16; i++) {
			if (piecesB[i]->posX == -1) continue;
			if (piecesB[i]->isMoveLegal(x, y, this)) {
				return 1;
			}
		}
	} else {
		for (int i(0); i < 16; i++) {
			if (piecesW[i]->posX == -1) continue;
			if (piecesW[i]->isMoveLegal(x, y, this)) {
				return 1;
			}
		}
	}
	return 0;
}


// vérifie si le chemin entre deux cases est libre
int Game::pathIsClear(int x1, int y1, int x2, int y2) {
	// déplacement vertical
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
	}
	// déplacement horizontal
	else if (y1 == y2) {
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

	}
	// déplacement diagonal
	else if (x1 < x2) {
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

Piece *Game::getPiece(int x, int y) { return board[x][y].piece; }

// permet de remettre à 0 les EnPassant des pions lors d'un déplacement
// qui n'implique pas un EnPassant
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

// permet de promouvoir un pion si il atteint la dernière ligne
int Game::promotePawn() {
	int x2(moveInt[2]);
	int y2(moveInt[3]);
	if (board[x2][y2].piece->id == Pawn_) {
		if (y2 == 7 || y2 == 0) {
			cout << "Promote to (Q)ueen, (R)ook, (B)ishop "
				"or (K)night?"
			     << endl;
			char choice;
			cin >> choice;
			Piece *tmp = nullptr;
			switch (choice) {
			case 'Q':
				tmp =
				    new Queen(board[x2][y2].piece->color, board[x2][y2].piece->posX,
					      board[x2][y2].piece->posY);
				break;
			case 'R':

				tmp =
				    new Rook(board[x2][y2].piece->color, board[x2][y2].piece->posX,
					     board[x2][y2].piece->posY);
				break;
			case 'B':
				tmp = new Bishop(board[x2][y2].piece->color,
						 board[x2][y2].piece->posX,
						 board[x2][y2].piece->posY);
				break;
			case 'N':
				tmp = new Knight(board[x2][y2].piece->color,
						 board[x2][y2].piece->posX,
						 board[x2][y2].piece->posY);
				break;
			default:
				cout << "Invalid choice" << endl;
				return 0;
			}
			Piece *toDelete = board[x2][y2].piece;
			tmp->hasMoved = true;
			for(int i(8); i < 16; i++){
				if(turn == White){
					if(board[x2][y2].piece == piecesW[i]){
						piecesW[i] = tmp;
					}
				}else{
					if(board[x2][y2].piece == piecesB[i]){
						piecesB[i] = tmp;
					}
				}
			}
			delete toDelete;
			updateBoard();
		}
	}
	return 1;
}

// transforme un le caractère caractérisant une colonne en entier
int Game::moveToFile(int nb) { return move[nb] - 'a'; }
// transforme un le caractère caractérisant une rangée en entier
int Game::moveToRank(int nb) { return move[nb] - '1'; }

// vérifie si le déplacement est légal et le fait si c'est le cas sinon renvoie 0
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
	if (!board[moveInt[0]][moveInt[1]].piece->isMoveLegal(moveInt[2], moveInt[3], this)) {
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

// fonction qui permet de récuperer le mouvement du joueur
void Game::getmove() {
	if (turn) {
		cout << "White -> ";
	} else {
		cout << "Black -> ";
	}
	cin >> move;
}

// annule les coups "virtuels"
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

// verifie si le roi est en echec
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
					if (board[i][j].piece->isMoveLegal(kingX, kingY, this)) {
						return 1;
					}
				}
			}
		}
	}
	return 0;
}
// verifie si le roi est en echec et mat
int Game::isCheckMate(int color) {
	for (int i = 0; i < 64; i++) {
		if (board[i % 8][i / 8].piece != nullptr &&
		    board[i % 8][i / 8].piece->color == color) {
			for (int j = 0; j < 64; j++) {
				if ((board[j % 8][j / 8].piece != nullptr &&
				     board[j % 8][j / 8].piece->color != color &&
				     board[j % 8][j / 8].piece->id != King_) ||
				    board[j % 8][j / 8].piece == nullptr) {
					if (board[i % 8][i / 8].piece->isMoveLegal(j % 8, j / 8,
										   this)) {
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
// vérifie si il reste un coup
int Game::isStalemate(int color) {
	for (int i = 0; i < 64; i++) {
		if (board[i % 8][i / 8].piece != nullptr &&
		    board[i % 8][i / 8].piece->color == color) {
			for (int j = 0; j < 64; j++) {
				if ((board[j % 8][j / 8].piece != nullptr &&
				     board[j % 8][j / 8].piece->color != color) ||
				    board[j % 8][j / 8].piece == nullptr) {
					if (board[i % 8][i / 8].piece->isMoveLegal(j % 8, j / 8,
										   this)) {
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

// efectue le roque long si possible sinon retourne 0
int Game::longCastling() {
	// initialisation des variables pour les noirs
	bool kingHasMoved = piecesB[4]->hasMoved;
	bool rookHasMoved = piecesB[0]->hasMoved;
	int y = 7;
	Color color = Black;

	// modification des variables pour les blancs
	if (turn == White) {
		kingHasMoved = piecesW[4]->hasMoved;
		rookHasMoved = piecesW[0]->hasMoved;
		y = 0;
		color = White;
	}

	// on verifie les conditions pour le roque long
	if (kingHasMoved == false && rookHasMoved == false && board[1][y].piece == nullptr &&
	    board[2][y].piece == nullptr && board[3][y].piece == nullptr &&
	    !isTheatened(4, y, color) && !isTheatened(3, y, color) && !isTheatened(2, y, color)) {

		// on effectue le roque long
		board[4][y].piece->posX = 2;
		board[0][y].piece->posX = 3;
		return 1;
	}

	return 0;
}

// de meme pour le roque court
int Game::shortCastling() {
	// initialisation des variables pour les noirs
	bool kingHasMoved = piecesB[4]->hasMoved;
	bool rookHasMoved = piecesB[0]->hasMoved;
	int y = 7;
	Color color = Black;

	// modification des variables pour les blancs
	if (turn == White) {
		kingHasMoved = piecesW[4]->hasMoved;
		rookHasMoved = piecesW[0]->hasMoved;
		y = 0;
		color = White;
	}

	// on verifie les conditions pour le roque long
	if (kingHasMoved == false && rookHasMoved == false && board[5][y].piece == nullptr &&
	    board[6][y].piece == nullptr && !isTheatened(4, y, color) &&
	    !isTheatened(5, y, color) && !isTheatened(6, y, color)) {
		// on effectue le roque long
		board[4][y].piece->posX = 6;
		board[7][y].piece->posX = 5;
		return 1;
	}
	return 0;
}



// fonction qui met à jour le plateau en fonction des positions des pièces
void Game::updateBoard() {
	for (int i(0); i < 64; i++) {
		board[i / 8][i % 8].piece = nullptr;
	}
	for (int i(0); i < 16; i++) {
		if (piecesB[i]->posX != -1 && piecesB[i]->posY != -1) {
			board[piecesB[i]->posX][piecesB[i]->posY].piece = piecesB[i];
		}
		if (piecesW[i]->posX != -1 && piecesW[i]->posY != -1) {
			board[piecesW[i]->posX][piecesW[i]->posY].piece = piecesW[i];
		}
	}
}

// fonction qui affiche le plateau de jeu dans le terminal
void Game::printBoard() {
	updateBoard();

	for (int i(0); i < 64; i++) {
		board[i / 8][i % 8].updateUtf();
	}
	cout << "    a   b   c   d   e   f   g   h " << endl;
	cout << "  ┌───┬───┬───┬───┬───┬───┬───┬───┐" << endl;
	for (int i(7); i >= 0; i--) {
		cout << i + 1 << " │ ";
		for (int j(0); j < 8; j++) {
			cout << board[j][i].utf << " │ ";
		}
		cout << endl;
		if (i) {
			cout << "  ├───┼───┼───┼───┼───┼───┼───┼───┤" << endl;
		}
	}
	cout << "  └───┴───┴───┴───┴───┴───┴───┴───┘" << endl;
}

// fonction qui termine une partie
void Game::quitGame() {
	updateBoard();
	// affichage du résultat de la partie
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
	cout << endl;
	// affichage de l'échiquier
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			if (board[j][i].piece != nullptr) {
				if (board[j][i].piece->color == White) {
					cout << "w";
				} else {
					cout << "b";
				}

				cout << board[j][i].piece->id;
			}
			cout << ",";
		}
	}
	// affichage du résultat de la partie
	switch (state) {
	case WhiteWin:
		cout << " 1-0" << endl;
		break;
	case BlackWin:
		cout << " 0-1" << endl;
		break;
	case Stalemate:
		cout << " 1/2-1/2" << endl;
		break;
	case quit:
		cout << " ?-?" << endl;
		break;
	default:
		cout << " ?-?" << endl;
		break;
	}
}
