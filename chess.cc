#include <iostream>
#include <string>
#include <vector>
using namespace std;


class piece{
    char id; // 0 
    char color;
    int posX;
    int posY;
    piece(char id,char color, int X,int Y):
        id(id),color(color),posX(X),posY(Y){}
};

class square{
    piece* piece; // NULL if no piece
};
class game{
    private:
    square board[8][8];
    piece *piecesW[16];
    piece *piecesB[16];
    game(){
        piecesW[0] = new Tour    (Noir, " \u265C ",  Square(7,0));
        piecesW[1] = new Cavalier(Noir, " \u265E ",  Square(7,1));
        piecesW[2] = new Fou     (Noir, " \u265D ",  Square(7,2));
        piecesW[3] = new Dame    (Noir, " \u265B ",  Square(7,3));
        piecesW[4] = new Roi     (Noir, " \u265A ",  Square(7,4));
        piecesW[5] = new Fou     (Noir, " \u265D ",  Square(7,5));
        piecesW[6] = new Cavalier(Noir, " \u265E ",  Square(7,6));
        piecesW[7] = new Tour    (Noir, " \u265C ",  Square(7,7));
        piecesB[0] = new Tour    (Blanc," \u2656 ",  Square(0,0));
        piecesB[1] = new Cavalier(Blanc," \u2658 ",  Square(0,1));
        piecesB[2] = new Fou     (Blanc," \u2657 ",  Square(0,2));
        piecesB[3] = new Dame    (Blanc," \u2655 ",  Square(0,3));
        piecesB[4] = new Roi     (Blanc," \u2654 ",  Square(0,4));
        piecesB[5] = new Fou     (Blanc," \u2657 ",  Square(0,5));
        piecesB[6] = new Cavalier(Blanc," \u2658 ",  Square(0,6));
        piecesB[7] = new Tour    (Blanc," \u2656 ",  Square(0,7));
    }

};


void printBoard(game *game){
    const char *kingB(" \u265A ");
    const char *queenB(" \u265B ");
    const char *rookB(" \u265C ");
    const char *bishopB(" \u265D ");
    const char *knightB(" \u265E ");
    const char *pawnB(" \u265F ");

    const char *kingW(" \u2654 ");
    const char *queenW(" \u2655 ");
    const char *rookW(" \u2656 ");
    const char *bishopW(" \u2657 ");
    const char *knightW(" \u2658 ");
    const char *pawnW(" \u2659 ");

    cout <<"    a   b   c   d   e   f   g   h "  << endl;
    cout <<"  ┌───┬───┬───┬───┬───┬───┬───┬───┐" << endl;
    for(int i(7);i>=0;i--){
        cout <<i+1 <<" │ ";
        for(int j(0);j<8;j++){
            cout<< " " << " │ ";
            
        }
        cout << endl;
        if(i){
            cout <<"  ├───┼───┼───┼───┼───┼───┼───┼───┤"<< endl;
        }
    }
    cout<<"  └───┴───┴───┴───┴───┴───┴───┴───┘" << endl;


}


int main(){
    int Elisa = 8;
    (void)Elisa;
    game game;
    printBoard(&game);


    return 1;
}

