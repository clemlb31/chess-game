#include <string>
#include <fstream>
using namespace std;
// enum pour une meilleur lisibilité
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

// Classe Piece contenant les informations sur une pièce
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

// Classe Square contenant les informations sur une case
class Square {
       public:
	Piece *piece;  // NULL if no piece
	void setSquare(class Piece *pointer) { this->piece = pointer; }
	string utf;
	void updateUtf();
};

// Classe Game contenant les informations sur la partie
class Game {
       private:
	GameState state;
	Piece *piecesW[16];
	Piece *piecesB[16];
	int prevousPositions[32][2];
	int previousState[32][2];  // 0 = Enpassant, 1 = hasMoved

	Square board[8][8];
	ofstream filePartie;
	int moveInt[4];
	string move;
	bool turn;
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
	void getmove();
	int moveIsLegal(int x1, int y1, int x2, int y2);
	int movePiece();
	int shortCastling();
	int longCastling();
	int pathIsClear(int x1, int y1, int x2, int y2);
	int promotePawn();

       public:
	Game();
	int getState();
	void play();
	void quitGame();
	void printBoard();
	~Game();
};