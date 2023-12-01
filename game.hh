#pragma once
#include "chess.hh"
#include "piece.hh"
#include "square.hh"
/**
 * @brief Classe Game contenant les informations sur la partie
 *
 */
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
	void savePositions();
	void undoMove();
	bool check;
	int isCheckMate(int color);
	int isCheck(int color);
	int isStalemate(int color);
	void getmove();
	int movePiece();
	int shortCastling();
	int longCastling();
	int promotePawn();

       public:
	   /**
	    * @brief Constructeur de la classe Game qui initialise la partie
	    * 
	    */
	Game();
	/**
	 * @brief retourne la pièce à la position x,y
	 * 
	 * @param x coordonnée x de la case
	 * @param y coordonnée y de la case
	 * @return Piece* 
	 */
	Piece *getPiece(int x, int y);
	/**
	 * @brief permet de remettre à zéro les pièces qui peuvent être prises en passant
	 * 
	 */
	void resetEnPassant();
	/**
	 * @brief vérifie si le chemin entre deux cases est libre
	 * 
	 * @param x1 coordonnée x de la case de départ
	 * @param y1 coordonnée y de la case de départ
	 * @param x2 coordonnée x de la case d'arrivée
	 * @param y2 coordonnée y de la case d'arrivée
	 * @return retourne 1 si le chemin est libre, 0 sinon
	 */
	int pathIsClear(int x1, int y1, int x2, int y2);
	/**
	 * @brief retourne l'état de la partie (en cours, quitté, blanc/noir gagne, pat)
	 * 
	 * @return enum GameState
	 */
	GameState getState();
	/**
	 * @brief récupère le mouvement du joueur et le joue si il est légal, sinon redemande un mouvement
	 * 
	 */
	void play();
	/**
	 * @brief affiche les statistiques en fin de partie
	 * 
	 */
	void quitGame();
	/**
	 * @brief affiche le plateau de jeu
	 * 
	 */
	void printBoard();
	~Game();
};