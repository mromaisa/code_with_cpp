#include <iostream>
using namespace std;

const int size_w = 3, size_h = 3;
// draw the board
void printBoard(char board[size_w][size_h]){
    for(int i=0; i < size_w; i++){
        for(int j=0; j < size_h; j++){
            cout << board[i][j];
            if(j < size_w - 1)
            cout << " | ";
        }
        if( i < size_h - 1)
        cout << "\n--+---+--"<< endl;
    }
}

// function to check winner
char checkWinner(char board[size_w][size_h]){
    // check linears
    for(int i=0; i<size_w; i++){
        if(board[i][0]==board[i][1] && board[i][1] == board[i][2] && board[i][2] != ' ')
        return board[i][2];

        if(board[0][i]==board[1][i] && board[1][i] == board[2][i] && board[2][i] != ' ')
        return board[2][i];
    }
    // check diagonals
    if(board[0][0]==board[1][1] && board[1][1] == board[2][2] && board[2][2] != ' '){
        return board[2][2];
    }
    if(board[1][2] == board[1][1] && board[1][1] == board[3][1] && board[1][2] != ' '){
        return board[1][2];
    }
    return ' ';
}

bool isDraw(char board[size_w][size_h]){
    for(int i=0; i<size_w; i++){
        for(int j=0; j<size_h; j++){
            if(board[i][j] == ' ')
            return false;
        }
    }
    return true;
}
int main()
{
    char board[size_w][size_h] = 
    {{' ', ' ', ' '},
    {' ', ' ', ' '},
    {' ', ' ', ' '}
    };
    char currentplayer = 'X';
    int row, col;
    bool game_end = false;

    // Start
    cout << "***********Welcome to Tic Tac Toe***********\n";
    printBoard(board);
    // game logic
    while(!game_end){
        
        
        cout << "\nenter player "<<currentplayer<< " your rows(0-2) and column(0-2) : ";
        cin >> row >> col;
        if(row>=0 && row<=2 && col>=0 && col<=2 && board[row][col]==' '){
            board[row][col] = currentplayer;
            printBoard(board);
            char winner = checkWinner(board);
            if(winner != ' '){
                cout << "CONGRATULATIONS!! "<< winner << " won the game.\n";
                game_end=true;
            } else if(isDraw(board)){
                cout << "The Game is Draw.\n";
                game_end=true;
            } else {
                cout << "\nNext player : ";
                currentplayer = (currentplayer= 'X' ? 'O' : 'X' );
                cout<<currentplayer<<endl;   
            }
        }else{
            cout << "Invalid input!! Out of bonunds\n";
        }
    }
    return 0;
}
