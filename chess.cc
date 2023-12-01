#include "chess.hh"
#include "piece.hh"
#include "game.hh"
#include <fstream>
#include <iostream>
#include <string>
using namespace std;


/** @mainpage Document d'information
 *   
 * @section Résumé_général Résumé général
 * 
 * Ce document a pour but de présenter le jeu d'Echec de LE BERRE Clément
 * 
 * 
 */

int main() {
	// initialisation
	Game game;

	// boucle tant que la partie n'est pas finie
	while (!game.getState()) {
		game.printBoard();
		game.play();
	}
	// affichage du plateau final et du résultat
	game.printBoard();
	game.quitGame();
	return 0;
}
