#include "game.hh"
#include "piece.hh"

// retourne le caractère unicode de la pièce
string King::toUtf() {
	if (color) {
		return "\u2654";
	} else {
		return "\u265A";
	}
}
string Pawn::toUtf() {
	if (color) {
		return "\u2659";
	} else {
		return "\u265F";
	}
}
string Rook::toUtf() {
	if (color) {
		return "\u2656";
	} else {
		return "\u265C";
	}
}
string Knight::toUtf() {
	if (color) {
		return "\u2658";
	} else {
		return "\u265E";
	}
}
string Queen::toUtf() {
	if (color) {
		return "\u2655";
	} else {
		return "\u265B";
	}
}
string Bishop::toUtf() {
	if (color) {
		return "\u2657";
	} else {
		return "\u265D";
	}
}


// vérifie si un déplacement est légal sivant la pièce
int Rook::isMoveLegal(int x2, int y2, Game *g) {
	if (x2 != posX && y2 != posY) {
		return 0;
	}
	if (!g->pathIsClear(posX, posY, x2, y2)) {
		return 0;
	}
	g->resetEnPassant();
	return 1;
}
int Bishop::isMoveLegal(int x2, int y2, Game *g) {
	if (abs(posX - x2) != abs(posY - y2)) {
		return 0;
	}
	if (!g->pathIsClear(posX, posY, x2, y2)) {
		return 0;
	}
	g->resetEnPassant();
	return 1;
}
int Queen::isMoveLegal(int x2, int y2, Game *g) {
	if (abs(posX - x2) != abs(posY - y2) && (x2 != posX && y2 != posY)) {
		return 0;
	}
	if (!g->pathIsClear(posX, posY, x2, y2)) {
		return 0;
	}
	g->resetEnPassant();
	return 1;
}
int Knight::isMoveLegal(int x2, int y2, Game *g) {
	if ((abs(posX - x2) != 2 && abs(posY - y2) != 2) ||
	    (abs(posX - x2) != 1 && abs(posY - y2) != 1)) {
		return 0;
	}
	g->resetEnPassant();
	return 1;
}
int King::isMoveLegal(int x2, int y2, Game *g) {
	if (abs(posX - x2) > 1 || abs(posY - y2) > 1) {
		return 0;
	}
	g->resetEnPassant();
	return 1;
}
int Pawn::isMoveLegal(int x2, int y2, Game *g) {
	if (color == White) {
		if (posY > y2) {
			return 0;
		}
	} else {
		if (posY < y2) {
			return 0;
		}
	}

	if (posX != x2 || abs(posY - y2) > 2) {
		if ((abs(posX - x2) != 1) || (abs(posY - y2) != 1)) {
			return 0;
		}
		if (g->getPiece(x2, y2) == nullptr) {
			if (g->getPiece(x2, posY) == nullptr) {
				return 0;
			}
			if (g->getPiece(x2, posY)->EnPassant == true) {
				g->getPiece(x2, posY)->posX = -1;
				g->getPiece(x2, posY)->posY = -1;
				return 1;
			}
			return 0;
		}
	}
	if (abs(posY - y2) == 1 && posX == x2 && g->getPiece(x2, y2) != nullptr) {
		return 0;
	}
	if (abs(posY - y2) == 2) {
		if (g->pathIsClear(posX, posY, x2, y2)) {
			g->getPiece(posX, posY)->EnPassant = true;
		}
		if (posY != 1 && posY != 6) {
			return 0;
		}
	} else {
		g->resetEnPassant();
	}
	return 1;
}
