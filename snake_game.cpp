#include <conio.h>
#include <iostream>
#include <windows.h>
using namespace std;

const int width = 80, height = 20;
int x, y, fruitCordX, fruitCordY, playerScore;
int snakeTailX[100], snakeTailY[100], snakeTailLen;
enum snakesDirection { STOP = 0, LEFT, RIGHT, UP, DOWN };
snakesDirection sDir;
bool isGameOver;

void GameInit() {
    isGameOver = false;
    sDir = RIGHT;  // Start moving to the right
    x = width / 2;
    y = height / 2;
    fruitCordX = rand() % width;
    fruitCordY = rand() % height;
    playerScore = 0;
    snakeTailLen = 0;
}

void GameRender(string playerName) {
    system("cls");
    for (int i = 0; i < width + 2; i++) cout << "-";
    cout << endl;

    for (int i = 0; i < height; i++) {
        for (int j = 0; j <= width; j++) {
            if (j == 0 || j == width)
                cout << "|";
            else if (i == y && j == x)
                cout << "O";
            else if (i == fruitCordY && j == fruitCordX)
                cout << "#";
            else {
                bool prTail = false;
                for (int k = 0; k < snakeTailLen; k++) {
                    if (snakeTailX[k] == j && snakeTailY[k] == i) {
                        cout << "o";
                        prTail = true;
                    }
                }
                if (!prTail) cout << " ";
            }
        }
        cout << endl;
    }
    for (int i = 0; i < width + 2; i++) cout << "-";
    cout << endl;

    cout << playerName << "'s Score: " << playerScore << endl;
}

int SetDifficulty() {
    int dfc;
    cout << "\nSET DIFFICULTY\n1: Easy\n2: Medium\n3: Hard\nChoose difficulty level: ";
    char choice;
    cin >> choice;
    switch (choice) {
        case '1': dfc = 150; break;  // Easy
        case '2': dfc = 100; break;  // Medium
        case '3': dfc = 50; break;   // Hard
        default: 
            cout << "Invalid input. Setting difficulty to Medium." << endl;
            dfc = 100;
    }
    return dfc;
}

void UserInput() {
    if (_kbhit()) {
        switch (_getch()) {
            case 'a': if (sDir != RIGHT) sDir = LEFT; break;
            case 'd': if (sDir != LEFT) sDir = RIGHT; break;
            case 'w': if (sDir != DOWN) sDir = UP; break;
            case 's': if (sDir != UP) sDir = DOWN; break;
            case 'x': isGameOver = true; break;
        }
    }
}

void UpdateGame() {
    int prevX = snakeTailX[0];
    int prevY = snakeTailY[0];
    int prev2X, prev2Y;
    snakeTailX[0] = x;
    snakeTailY[0] = y;

    for (int i = 1; i < snakeTailLen; i++) {
        prev2X = snakeTailX[i];
        prev2Y = snakeTailY[i];
        snakeTailX[i] = prevX;
        snakeTailY[i] = prevY;
        prevX = prev2X;
        prevY = prev2Y;
    }

    switch (sDir) {
        case LEFT: x--; break;
        case RIGHT: x++; break;
        case UP: y--; break;
        case DOWN: y++; break;
    }

    if (x >= width || x < 0 || y >= height || y < 0) isGameOver = true;
    for (int i = 0; i < snakeTailLen; i++) {
        if (snakeTailX[i] == x && snakeTailY[i] == y) isGameOver = true;
    }

    if (x == fruitCordX && y == fruitCordY) {
        playerScore += 10;
        fruitCordX = rand() % width;
        fruitCordY = rand() % height;
        snakeTailLen++;
    }
}

int main() {
    string playerName;
    cout << "Enter your name: ";
    cin >> playerName;
    int dfc = SetDifficulty();
    GameInit();

    while (!isGameOver) {
        GameRender(playerName);
        UserInput();
        UpdateGame();
        Sleep(dfc);  // Control game speed
    }
    cout << "Game Over! Final Score: " << playerScore << endl;
    return 0;
}
