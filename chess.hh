#pragma once
#include <fstream>
#include <string>

using namespace std;
// enum pour une meilleur lisibilité
enum Color { Black = 0, White = 1 };

enum PieceType : char {
	Rook_ = 'R',
	Bishop_ = 'B',
	Knight_ = 'N',
	King_ = 'K',
	Queen_ = 'Q',
	Pawn_ = 'P'

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
