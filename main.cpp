// required header file
#include <conio.h>
#include <iostream>
#include <windows.h>
using namespace std;

// height and width of the boundary
const int width = 80;
const int height = 20;

// Snake head coordinates of snake (x-axis, y-axis)
int x, y;
// Food coordinates
int fruitCordX, fruitCordY;
// variable to store the score of he player
int playerScore;
// Array to store the coordinates of snake tail (x-axis,
// y-axis)
int snakeTailX[100], snakeTailY[100];
// variable to store the length of the sanke's tail
int snakeTailLen;
// for storing snake's moving snakesDirection
enum snakesDirection { STOP = 0, LEFT, RIGHT, UP, DOWN };
// snakesDirection variable
snakesDirection sDir;
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
    sDir = RIGHT;
    x = width / 2;
    y = height / 2;
    fruitCordX = rand() % width;
    fruitCordY = rand() % height;
    playerScore = 0;
    HideCursor(); // Ẩn con trỏ nháy

    // Khởi tạo chiều dài ban đầu của rắn
    snakeTailLen = 2; // Độ dài ban đầu của rắn (bao gồm cả đầu)
    
    // Khởi tạo vị trí phần thân ban đầu
    snakeTailX[0] = x - 1;
    snakeTailY[0] = y;
    snakeTailX[1] = x - 2;
    snakeTailY[1] = y;
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

    // Creating top walls with '-'
    for (int i = 0; i < width + 2; i++)
        cout << "=";
    cout << endl;

    for (int i = 0; i < height; i++) {
        for (int j = 0; j <= width; j++) {
            // Creating side walls with '|'
            if (j == 0 || j == width)
                cout << "+";
            // Creating snake's head with 'O'
            if (i == y && j == x)
                cout << ":";
            // Creating the sanke's food with '#'
            else if (i == fruitCordY && j == fruitCordX)
                cout << "#";
            // Creating snake's head with 'O'
            else {
                bool prTail = false;
                for (int k = 0; k < snakeTailLen; k++) {
                    if (snakeTailX[k] == j
                        && snakeTailY[k] == i) {
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

    // Creating bottom walls with '-'
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
    case LEFT:
        x--;
        break;
    case RIGHT:
        x++;
        break;
    case UP:
        y--;
        break;
    case DOWN:
        y++;
        break;
    }

    // Checks for snake's collision with the wall (|)
    if (x >= width || x < 0 || y >= height || y < 0)
        isGameOver = true;

    // Checks for collision with the tail (o)
    for (int i = 0; i < snakeTailLen; i++) {
        if (snakeTailX[i] == x && snakeTailY[i] == y)
            isGameOver = true;
    }

    // Checks for snake's collision with the food (#)
    if (x == fruitCordX && y == fruitCordY) {
        playerScore += 10;
        fruitCordX = rand() % width;
        fruitCordY = rand() % height;
        snakeTailLen++;
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
        if (key == 224){ // 224 cho phím mũi tên 
            key = _getch(); // lấy mã tiếp theo để xđ hướng 
            // Getting the pressed key
            switch (key) {
                case 75: // left 
                    if (sDir != RIGHT) sDir = LEFT;
                    break;
                case 77: // right 
                    if (sDir != LEFT) sDir = RIGHT;
                    break;
                case 72: // up 
                    if (sDir != DOWN) sDir = UP;
                    break;
                case 80: // down 
                    if (sDir != UP) sDir = DOWN;
                    break;
            }
        } else if (key == 'x') isGameOver = true;    
    }
}

// Main function / game looping function
int main()
{
    srand(time(0)); // seed for random number generation 
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
        Sleep(dfc);
    }
    system("cls");
    cout << "Game Over!" << playerName << "'s final score: " << playerScore << endl;
    return 0;
}
