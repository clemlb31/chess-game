#pragma once
#include "chess.hh"
class Game;

/**
 * @brief Classe Piece contenant les informations sur une pièce
 *
 */
class Piece {
       public:
	/**
	 * @brief couleur de la pièce
	 * 
	 */
	Color color;
	/**
	 * @brief type de la pièce
	 * 
	 */
	PieceType id;
	/**
	 * @brief position en X de la pièce
	 * 
	 */
	int posX;
	/**
	 * @brief position en Y de la pièce
	 * 
	 */
	int posY;
	/**
	 * @brief true si la pièce peut être prise en passant
	 * 
	 */
	bool EnPassant;
	/**
	 * @brief true si la pièce a bougé
	 * 
	 */
	bool hasMoved;
	/**
	 * @brief Constructeur de la classe Piece
	 * 
	 * @param type type de la pièce (Pawn, Rook, Bishop, Knight, King, Queen)
	 * @param color couleur de la pièce (White, Black)
	 * @param X position initiale en X
	 * @param Y position initiale en Y
	 */
	Piece(PieceType type, Color color, int X, int Y)
	    : color(color), id(type), posX(X), posY(Y), EnPassant(0), hasMoved(false) {}
	/**
	 * @brief Destructeur de la classe Piece
	 * 
	 */
	virtual ~Piece() {}
	/**
	 * @brief vérifie si le mouvement est légal jusqu'à la case (x2, y2)
	 * 
	 * @param x2 position en X de la case d'arrivée
	 * @param y2 position en Y de la case d'arrivée
	 * @param g pointeur vers la classe Game
	 * @return int, 1 si le mouvement est légal, 0 sinon
	 */
	virtual int isMoveLegal(int x2, int y2, Game *g) = 0;
	/**
	 * @brief retourne le caractère unicode de la pièce
	 *
	 * @return string, caractère unicode de la pièce
	 */
	virtual string toUtf() = 0;
};
/**
 * @brief sous classe Pawn qui contient le code utf de la piece et son mouvement
 *
 */
class Pawn : public Piece {
       public:
	Pawn(Color color, int X, int Y) : Piece(Pawn_, color, X, Y) {}
	string toUtf();
	int isMoveLegal(int x2, int y2, Game *g);
};
/**
 * @brief sous classe Rook qui contient le code utf de la piece et son mouvement
 *
 */
class Rook : public Piece {
       public:
	Rook(Color color, int X, int Y) : Piece(Rook_, color, X, Y) {}
	string toUtf();
	int isMoveLegal(int x2, int y2, Game *g);
};
/**
 * @brief sous classe Bishop qui contient le code utf de la piece et son mouvement
 *
 */
class Bishop : public Piece {
       public:
	Bishop(Color color, int X, int Y) : Piece(Bishop_, color, X, Y) {}
	string toUtf();
	int isMoveLegal(int x2, int y2, Game *g);
};
/**
 * @brief sous classe Knight qui contient le code utf de la piece et son mouvement
 *
 */
class Knight : public Piece {
       public:
	Knight(Color color, int X, int Y) : Piece(Knight_, color, X, Y) {}
	string toUtf();
	int isMoveLegal(int x2, int y2, Game *g);
};
/** 
 * @brief sous classe King qui contient le code utf de la piece et son mouvement
 *
 */
class King : public Piece {
       public:
	King(Color color, int X, int Y) : Piece(King_, color, X, Y) {}
	string toUtf();
	int isMoveLegal(int x2, int y2, Game *g);
};
/**
 * @brief sous classe Queen qui contient le code utf de la piece et son mouvement
 *
 */
class Queen : public Piece {
       public:
	Queen(Color color, int X, int Y) : Piece(Queen_, color, X, Y) {}
	string toUtf();
	int isMoveLegal(int x2, int y2, Game *g);
};

