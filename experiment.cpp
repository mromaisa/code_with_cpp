#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>

using namespace std;

const int WIDTH = 20;
const int HEIGHT = 20;
const int INITIAL_SNAKE_LENGTH = 5;

struct Point {
    int x, y;
};

vector<Point> snake;
Point food;
int direction;
bool gameOver;

void initGame() {
    snake.clear();
    for (int i = 0; i < INITIAL_SNAKE_LENGTH; i++) {
        snake.push_back({WIDTH / 2 + i, HEIGHT / 2});
    }
    food = {rand() % WIDTH, rand() % HEIGHT};
    direction = 0; // 0: right, 1: down, 2: left, 3: up
    gameOver = false;
}

void drawGame() {
    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            bool isSnake = false;
            for (Point p : snake) {
                if (p.x == x && p.y == y) {
                    isSnake = true;
                    break;
                }
            }
            if (isSnake) {
                cout << "S ";
            } else if (x == food.x && y == food.y) {
                cout << "F ";
            } else {
                cout << ". ";
            }
        }
        cout << endl;
    }
}

void updateGame() {
    Point newHead = snake[0];
    switch (direction) {
        case 0: newHead.x++; break;
        case 1: newHead.y++; break;
        case 2: newHead.x--; break;
        case 3: newHead.y--; break;
    }
    snake.insert(snake.begin(), newHead);
    if (snake[0].x == food.x && snake[0].y == food.y) {
        food = {rand() % WIDTH, rand() % HEIGHT};
    } else {
        snake.pop_back();
    }
    if (snake[0].x < 0 || snake[0].x >= WIDTH || snake[0].y < 0 || snake[0].y >= HEIGHT) {
        gameOver = true;
    }
    for (int i = 1; i < snake.size(); i++) {
        if (snake[0].x == snake[i].x && snake[0].y == snake[i].y) {
            gameOver = true;
            break;
        }
    }
}

void handleInput() {
    char input;
    cin >> input;
    switch (input) {
        case 'w': direction = 3; break;
        case 's': direction = 1; break;
        case 'a': direction = 2; break;
        case 'd': direction = 0; break;
    }
}

int main() {
    srand(time(0));
    initGame();
    while (!gameOver) {
        drawGame();
        handleInput();
        updateGame();
    }
    cout << "Game Over!" << endl;
    return 0;
}