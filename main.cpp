// required header file
#include <conio.h>
#include <iostream>
#include <windows.h>
#include <ctime>
#include <cstdlib>
#include "game_con_ran.h"
#include <thread>

using namespace std;

// height and width of the boundary
const int width = 80;
const int height = 20;

// Snake head coordinates of snake (x-axis, y-axis)
Snake snake;
Point pre_snakeTail;
// Food coordinates
Food food;
// variable to store the score of he player
int playerScore;
// variable to store the length of the sanke's tail
int snakeTailLen;
// for storing snake's moving snakesDirection
// boolean variable for checking game is over or not
bool isGameOver;

// Điểm tăng thêm khi ăn thức ăn, tùy thuộc vào độ khó
int scoreIncrement;

// Hàm ẩn con trỏ nháy trong console
void HideCursor()
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO cursorInfo;
	GetConsoleCursorInfo(hConsole, &cursorInfo);
	cursorInfo.bVisible = false; // Ẩn con trỏ
	SetConsoleCursorInfo(hConsole, &cursorInfo);
}

void setConsoleBackgroundColor(int r, int g, int b, std::string str)
{
	std::cout << "\033[48;2;" << r << ";" << g << ";" << b << "m";
	std::cout << str;
}

void playSoundAsync(int frequency, int duration) {
	Beep(frequency, duration);  // Phát âm thanh
}

void GoToXY(int x, int y)
{
	COORD coord;
	coord.X = x;
	coord.Y = y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

void generateFood() 
{
	// Initialize a random position for food
	food = Food(rand() % width + 1, rand() % height + 1);

	// Ensure the food position does not overlap with the snake's head
	while (snake.getHead() == food.getFood())
	{
		food = Food(rand() % width + 1, rand() % height + 1);
	}
	bool check = false; // if check = true, regenerate 
	do 
	{
		// Check if the food overlaps with any part of the snake's body
		for (int i = 0; i < snakeTailLen; ++i)
		{
			if (snake.getBody()[i] == food.getFood()) {
				check = true;
				food = Food(rand() % width + 1, rand() % height + 1);
				break;
			}
		}
	} while (check);
}

// Function to initialize game
void GameInit()
{
	HideCursor(); // Ẩn con trỏ nháy
	GoToXY(0, 0);
	// Creating top walls
	for (int i = 0; i < width + 2; i++)
		setConsoleBackgroundColor(0, 255, 255, " ");
	cout << endl;

	// Creating bottom walls
	GoToXY(0, height + 1);
	for (int i = 0; i < width + 2; i++)
		cout << " ";
	cout << endl;

	// Creating side walls
	for (int i = 1; i < height + 1; ++i)
	{
		GoToXY(0, i); cout << " ";
		GoToXY(width + 1, i); cout << " ";
	}

	// Creating play field
	for (int i = 1; i <= height; i++)
		for (int j = 1; j <= width; j++)
		{
			GoToXY(j, i);
			setConsoleBackgroundColor(0, 0, 0, " ");
		}

	snake = Snake(width / 2, height / 2);
	isGameOver = false;
	generateFood();
	playerScore = 0;
	snakeTailLen = snake.getBody().size(); // Độ dài đuôi ban đầu của rắn 
}

void setTextColor(int color) // màu chữ 
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}

// Function for creating snake
void GameRender(string playerName)
{
	GoToXY(pre_snakeTail.getX(), pre_snakeTail.getY());
	setConsoleBackgroundColor(0, 0, 0, " ");

	GoToXY(snake.getHead().getX(), snake.getHead().getY());
	setConsoleBackgroundColor(255, 0, 0, " ");

	GoToXY(food.getFood().getX(), food.getFood().getY());
	setConsoleBackgroundColor(255, 255, 0, " ");
	for (int k = 0; k < snakeTailLen; k++) {
		GoToXY(snake.getBody()[k].getX(), snake.getBody()[k].getY());
		setConsoleBackgroundColor(255, 165, 0, " ");
	}
	cout << endl;
	// Display player's score
	GoToXY(0, height + 2);
	setConsoleBackgroundColor(0, 0, 0, "");
	setTextColor(10);
	cout << playerName << "'s Score: " << playerScore << endl;
	setTextColor(7);
}

// Function for updating the game state
void UpdateGame()
{
	snakeTailLen = snake.getBody().size();
	snake.move(snake.getDirection());
	// Checks for snake's collision with the wall (|)
	if (snake.getHead().getX() > width || snake.getHead().getX() == 0 || snake.getHead().getY() > height || snake.getHead().getY() == 0)
	{
		isGameOver = true;
		std::thread soundThread(playSoundAsync, 400, 500);  // Âm thanh khi rắn chết
		soundThread.detach();
	}

	// Checks for collision with the tail (o)
	for (int i = 0; i < snakeTailLen; i++) {
		if (snake.getBody()[i] == snake.getHead())
		{
			isGameOver = true;
			std::thread soundThread(playSoundAsync, 400, 500);  // Âm thanh khi rắn chết
			soundThread.detach();
		}
	}

	// Checks for snake's collision with the food (#)
	if (snake.foodCollision(food)) {
		playerScore += scoreIncrement;
		generateFood();
		std::thread soundThread(playSoundAsync, 600, 500);  // Âm thanh khi rắn ăn thức ăn
		soundThread.detach();
	}
}

// Function to set the game difficulty level
int SetDifficultyLevel()
{
	int dfc;
	char choice;
	setTextColor(14);
	cout << "\nSET DIFFICULTY\n1: Easy\n2: Medium\n3: Hard ";
	setTextColor(7);
	cout << "\nNOTE: if not chosen or pressed any other key, the difficulty will be automatically\n     set to medium ";
	setTextColor(10);
	cout << "\nChoose difficulty level : ";
	setTextColor(7);
	cin >> choice;
	switch (choice) {
	case '1':
		dfc = 150; // for easy 
		scoreIncrement = 10; // Điểm tăng 10
		break;
	case '2':
		dfc = 100; // medium 
		scoreIncrement = 20; // Điểm tăng 20
		break;
	case '3':
		dfc = 50; // higher for hard 
		scoreIncrement = 30; // Điểm tăng 30
		break;
	default:
		dfc = 100; // default to medium 
		scoreIncrement = 20; // Điểm tăng 20
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
	setTextColor(10);
	cout << "ENTER YOUR NAME: ";
	setTextColor(7);
	cin >> playerName;

	do {
		int dfc = SetDifficultyLevel();

		GameInit();
		while (!isGameOver) {

			pre_snakeTail = snake.getTail();
			UpdateGame();
			if (isGameOver) break;
			GameRender(playerName);
			UserInput();

			// creating a delay for according to the chosen
			// difficulty
			if (snake.getDirection() == DirectionUp || snake.getDirection() == DirectionDown)
				Sleep(dfc * 1.5);
			else
				Sleep(dfc);
		}

		system("cls");
		setTextColor(10);
		cout << "Game Over!" << playerName << "'s final score: " << playerScore << endl;
		setTextColor(7);

		// Hiển thị menu lựa chọn
		char choice;
		setTextColor(14);
		cout << "\nDo you want to play again? (Y/N): ";
		setTextColor(7);
		cin >> choice;

		if (choice == 'N' || choice == 'n')
		{
			isGameOver = true; // Đặt trạng thái game kết thúc nếu người chơi chọn thoát
		}
		else
		{
			isGameOver = false;
			playerScore = 0;
			snake = Snake(width / 2, height / 2);
			generateFood();
		}
	} while (!isGameOver); // nếu không kết thúc thì tiếp tục chơi lại 
	return 0;
}