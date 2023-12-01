#include "square.hh"
#include "piece.hh"

// met à jour le caractère unicode de la case
void Square::updateUtf() {
	if (piece == nullptr) {
		utf = " ";
	} else {
		utf = piece->toUtf();
	}
}
