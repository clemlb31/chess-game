#include <iostream>
#include <string>
#include <vector>
using namespace std;

class piece{
    public:
    char id; // 0 
    char color;
    int posX;
    int posY;
    piece(char id,char color, int X,int Y):
        id(id),color(color),posX(X),posY(Y){}
};


class square{
    private:
    piece* piece; // NULL if no piece
    public:
    void setSquare(class piece* pointer){
        this->piece = pointer;
    }

};
class game{
    private:
    square board[8][8];
    piece *piecesW[16];
    piece *piecesB[16];
    public:
    game(){
        piecesB[0] = new piece('R','B',0,7);
        piecesB[1] = new piece('N','B',1,7);
        piecesB[2] = new piece('B','B',2,7);
        piecesB[3] = new piece('Q','B',3,7);
        piecesB[4] = new piece('K','B',4,7);
        piecesB[5] = new piece('B','B',5,7);
        piecesB[6] = new piece('N','B',6,7);
        piecesB[7] = new piece('R','B',7,7);

        piecesW[0] = new piece('R','W',0,0);
        piecesW[1] = new piece('N','W',1,0);
        piecesW[2] = new piece('B','W',2,0);
        piecesW[3] = new piece('Q','W',3,0);
        piecesW[4] = new piece('K','W',4,0);
        piecesW[5] = new piece('B','W',5,0);
        piecesW[6] = new piece('N','W',6,0);
        piecesW[7] = new piece('R','W',7,0);
        for(int i(0);i<7;i++){
            piecesB[i+8] = new piece('P','B',i,6);
            piecesW[i+8] = new piece('P','W',i,1);
        }
        updateBoard();
    }
    void updateBoard(){
        for(int i(0);i<64;i++){
            board[i/8][i%8].setSquare(nullptr);
        }
        for(int i(0);i<16;i++){
            board[piecesB[i]->posX][piecesB[i]->posY].setSquare(piecesB[i]);
            board[piecesW[i]->posX][piecesW[i]->posY].setSquare(piecesW[i]);
        }   
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
    //game game;
    cout << sizeof("\u2654") << endl;
    printBoard(&game);


    return 1;
}

