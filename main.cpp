// required header file
#include <conio.h>
#include <iostream>
#include <windows.h>
#include <ctime>
#include <cstdlib>
#include "game_con_ran.h"
using namespace std;

// height and width of the boundary
const int width = 80;
const int height = 20;

// Snake head coordinates of snake (x-axis, y-axis)
Snake snake(width / 2, height / 2);
// Food coordinates
Food food;
// variable to store the score of he player
int playerScore;
// variable to store the length of the sanke's tail
int snakeTailLen;
// for storing snake's moving snakesDirection
// boolean variable for checking game is over or not
bool isGameOver;

// Hàm ẩn con trỏ nháy trong console
void HideCursor()
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO cursorInfo;
    GetConsoleCursorInfo(hConsole, &cursorInfo);
    cursorInfo.bVisible = false; // Ẩn con trỏ
    SetConsoleCursorInfo(hConsole, &cursorInfo);
}

// Function to initialize game variables
void GameInit()
{
    isGameOver = false;
    food = Food(rand() % width, rand() % height);
    playerScore = 0;
    HideCursor(); // Ẩn con trỏ nháy
    snakeTailLen = snake.getBody().size(); // Độ dài đuôi ban đầu của rắn 
}

void GoToXY(int x, int y) {
    COORD coord;
    coord.X = x;
    coord.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

// Function for creating the game board & rendering
void GameRender(string playerName)
{
    GoToXY(0, 0);

    // Creating top walls with '='
    for (int i = 0; i < width + 2; i++)
        cout << "=";
    cout << endl;

    for (int i = 0; i < height; i++) {
        for (int j = 0; j <= width; j++) {
            // Creating side walls with '+'
            if (j == 0 || j == width)
                cout << "+";
            // Creating snake's head with ':'
            if (snake.getHead() == Point(j, i))
                cout << ":";
            // Creating the sanke's food with '#'
            else if (food.getFood() == Point(j, i))
                cout << "#";

            else {
                bool prTail = false;
                for (int k = 0; k < snakeTailLen; k++) {
                    if (snake.getBody()[k] == Point(j, i)) {
                        cout << "o";
                        prTail = true;
                    }
                }
                if (!prTail)
                    cout << " ";
            }
        }
        cout << endl;
    }

    // Creating bottom walls with '='
    for (int i = 0; i < width + 2; i++)
        cout << "=";
    cout << endl;

    // Display player's score
    cout << playerName << "'s Score: " << playerScore
        << endl;
}

// Function for updating the game state
void UpdateGame()
{
    snakeTailLen = snake.getBody().size();
    snake.move(snake.getDirection());
    // Checks for snake's collision with the wall (|)
    if (snake.getHead().getX() >= width || snake.getHead().getX() < 0 || snake.getHead().getY() >= height || snake.getHead().getY() < 0)
        isGameOver = true;

    // Checks for collision with the tail (o)
    for (int i = 0; i < snakeTailLen; i++) {
        if (snake.getBody()[i] == snake.getHead())
            isGameOver = true;
    }

    // Checks for snake's collision with the food (#)
    if (snake.foodCollision(food)) {
        playerScore += 10;
        food = Food(rand() % width, rand() % height);
    }
}

// Function to set the game difficulty level
int SetDifficultyLevel()
{
    int dfc;
    char choice;
    cout << "\nSET DIFFICULTY\n1: Easy\n2: Medium\n3: hard "
        "\nNOTE: if not chosen or pressed any other key, the difficulty will be automatically\n     set to medium \nChoose difficulty level: ";
    cin >> choice;
    switch (choice) {
    case '1':
        dfc = 150; // for easy 
        break;
    case '2':
        dfc = 100; // medium 
        break;
    case '3':
        dfc = 50; // higher for hard 
        break;
    default:
        dfc = 100; // default to medium 
    }
    return dfc;
}

// Function to handle user UserInput
void UserInput()
{
    // Checks if a key is pressed or not
    if (_kbhit()) {
        int key = _getch();
        if (key == 224) { // 224 cho phím mũi tên 
            key = _getch(); // lấy mã tiếp theo để xđ hướng 
            // Getting the pressed key
            switch (key) {
            case 75: // left 
                if (snake.getDirection() != DirectionRight) snake.setDirection(DirectionLeft);
                break;
            case 77: // right 
                if (snake.getDirection() != DirectionLeft) snake.setDirection(DirectionRight);
                break;
            case 72: // up 
                if (snake.getDirection() != DirectionDown) snake.setDirection(DirectionUp);
                break;
            case 80: // down 
                if (snake.getDirection() != DirectionUp) snake.setDirection(DirectionDown);
                break;
            }
        }
        else if (key == 'x') isGameOver = true;
    }
}

// Main function / game looping function
int main()
{
    srand(time(NULL)); // seed for random number generation 
    string playerName;
    cout << "enter your name: ";
    cin >> playerName;
    int dfc = SetDifficultyLevel();

    GameInit();
    while (!isGameOver) {
        GameRender(playerName);
        UserInput();
        UpdateGame();
        // creating a delay for according to the chosen
        // difficulty
        if (snake.getDirection() == DirectionUp || snake.getDirection() == DirectionDown)
            Sleep(dfc * 1.5);
        else
            Sleep(dfc);
    }
    system("cls");
    cout << "Game Over!" << playerName << "'s final score: " << playerScore << endl;
    return 0;
}
