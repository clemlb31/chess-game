#pragma once
#include "chess.hh"

class Piece;

/**
 * @brief Classe Square contenant les informations sur une case
 *
 */
class Square {
       public:
	Piece *piece;  // NULL if no piece
	
	/**
	 * @brief contient le caractère unicode de la case
	 * 
	 */
	string utf;

	/**
	 * @brief met à jour le caractère unicode de la case
	 * 
	 */
	void updateUtf();
};
