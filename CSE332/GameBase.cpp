// 332 Lab 4: Sean McManus, Allen Osgood, Daniel Margulis
// Defines all methods and printing for playing 
//	any game

#include "stdafx.h"
#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <memory>
#include <fstream>
#include <algorithm>

using namespace std;


shared_ptr<GameBase> GameBase::ptr = nullptr;

void GameBase::chooseGame(int argc, char* argv[]) {
	if (GameBase::ptr != nullptr) throw BAD_PTR;

	// See step 16-20 for implementation instructions
	if (argc < 2){
		throw BAD_NAME;
	}
	if (strcmp(argv[1], "TicTacToe") == 0) {
		try {
			ptr = shared_ptr<GameBase>(new TicTacToe);
		}
		catch (result_ e) {
			if (e == BAD_ALLOC) throw BAD_ALLOC;
			else if (e == INVALID_SAVED_BOARD) {
				cout << "Error: Invalid save board" << endl;
				cout << "Please correct the " << argv[1] << " save file to resume your game" << endl;

				throw INVALID_SAVED_BOARD;
			}
			else {
				cout << "Error: An unknown error occured" << endl;

				throw e;
			}
		}
	}

	else if (strcmp(argv[1], "Gomoku") == 0) {
		try {
			ptr = shared_ptr<GameBase>(new Gomoku);
		}
		catch (result_ e) {
			if (e == BAD_ALLOC) throw BAD_ALLOC;
			else if (e == INVALID_SAVED_BOARD) {
				cout << "Error: Invalid save board" << endl;
				cout << "Please correct the " << argv[1] << " save file to resume your game" << endl;

				throw INVALID_SAVED_BOARD;
			}
			else {
				cout << "Error: An unknown error occured" << endl;

				throw e;
			}
		}
	}

	else if (strcmp(argv[1], "Sudoku") == 0) {
		try {
			ptr = shared_ptr<GameBase>(new Sudoku);
		}
		catch (result_ e) {
			if (e == BAD_ALLOC) throw BAD_ALLOC;
			else if (e == NO_ORIGINAL_BOARD) {
				cout << "Error: There is no starting board" << endl;
				cout << "Please define a sudoku0 file to begin playing" << endl;

				throw NO_ORIGINAL_BOARD;
			}
			else if (e == INVALID_ORIGINAL_BOARD) {
				cout << "Error: Invalid original board" << endl;
				cout << "Please correct the sudoku0 file to begin playing" << endl;

				throw INVALID_ORIGINAL_BOARD;
			}
			else if (e == INVALID_SAVED_BOARD) {
				cout << "Error: Invalid save board" << endl;
				cout << "Please correct the " << argv[1] << " save file to resume your game" << endl;

				throw INVALID_SAVED_BOARD;
			}
			else {
				cout << "Error: An unknown error occured" << endl;

				throw e;
			}
		}
	}
	else if (strcmp(argv[1], "UltimateTicTacToe") == 0) {
		try {
			ptr = shared_ptr<GameBase>(new UltimateTicTacToe);
		}
		catch (result_ e) {
			if (e == BAD_ALLOC) throw BAD_ALLOC;
			else if (e == INVALID_SAVED_BOARD) {
				cout << "Error: Invalid save board" << endl;
				cout << "Please correct the " << argv[1] << " save file to resume your game" << endl;

				throw INVALID_SAVED_BOARD;
			}
			else {
				cout << "Error: An unknown error occured" << endl;

				throw e;
			}
		}
	}
	else {
		throw BAD_NAME;
	}
}

shared_ptr<GameBase> GameBase::instance() {

	if (ptr == nullptr) throw BAD_INSTANCE;
	else return ptr;
}

int GameBase::prompt(unsigned int &col, unsigned int &row)
{
	bool validInput = false;

	while (!validInput)
	{
		int result = SUCCESS;
		cout << endl << "Player  " << players[currentPlayer] << ", enter a valid move 'x,y' or 'quit' to exit:" << endl;

		string input;
		cin >> input;

		// if string is quit, return quit instruction
		if (input == "quit") return USER_QUIT;

		// if the string contains a comma, attempt to extract values
		if (input.find(",") != string::npos) input.replace(input.find(","), sizeof(" ") - 1, " ");
		else result = INVALID_INPUT_STRING;
		istringstream iss(input);

		if (!(iss >> col)) result = INVALID_INPUT_STRING;
		if (!(iss >> row)) result = INVALID_INPUT_STRING;

		if (result == SUCCESS)
		{
			// if not UltimateTicTacToe
			if (width != 9) {
				if (col < 1 || col >= (unsigned)width || row < 1 || row >= (unsigned)height) result = INVALID_MOVE;
				if (v[width*row + col] != " ") result = INVALID_MOVE;
			}
			else {
				if (col < 1 || col > (unsigned)width || row < 1 || row > (unsigned)height) result = INVALID_MOVE;
				if (v[width*(row-1) + (col - 1)] != " ") result = INVALID_MOVE;
			}
		}

		// print out the correct error
		if (result != SUCCESS)
		{
			if (result == INVALID_INPUT_STRING) cout << "Error: Invalid string input" << endl;
			else if (result == INVALID_MOVE) cout << "Error: Invalid move" << endl
				<< "Please pick an open space within the board" << endl;
			else cout << "Error: An unknown error occured" << endl;
		}

		validInput = result == SUCCESS;
	}

	return SUCCESS;
}

int GameBase::prompt(unsigned int &col, unsigned int &row, unsigned int &num)
{
	bool validInput = false;

	while (!validInput)
	{
		int result = SUCCESS;
		cout << endl << "Enter a valid move 'x,y guess' or 'quit' to exit:" << endl;

		string input;
		string guess;
		cin >> input;

		// if string is quit, return quit instruction
		if (input == "quit") return USER_QUIT;

		cin >> guess;

		// ignore rest of input line to extract valid move from extraneous input
		cin.ignore(numeric_limits<streamsize>::max(), '\n');

		if (input.find(",") != string::npos) input.replace(input.find(","), sizeof(" ") - 1, " ");
		else result = INVALID_INPUT_STRING;

		istringstream iss(input);
		istringstream iss1(guess);

		if (!(iss >> col)){
			result = INVALID_INPUT_STRING;
		}
		if (!(iss >> row)){
			result = INVALID_INPUT_STRING;
		}
		if (!(iss1 >> num)){
			result = INVALID_INPUT_STRING;
		}

		row = row - 1;
		col = col - 1;

		if (result == SUCCESS)
		{
			// check that the spot is within the board and the guess is 1-9
			if (col < 0 || col > (unsigned) width || row < 0 || row > (unsigned) height || num < 1 || num > 9) result = INVALID_MOVE;
			// check that the player is not overwriting an initial board position
			else if (find(initial.begin(), initial.end(), width*row + col) != initial.end()) result = OVERWRITE_INITIAL;
		}

		// print out the correct error
		if (result != SUCCESS)
		{
			if (result == INVALID_INPUT_STRING) cout << "Error: Invalid string input" << endl;
			else if (result == INVALID_MOVE) cout << "Error: Invalid move" << endl
				<< "Please choose a valid guess that is within the board" << endl;
			else if (result == OVERWRITE_INITIAL) cout << "Error: You cannot overwrite a given value" << endl;
			else cout << "Error: An unknown error occured" << endl;
		}

		validInput = result == SUCCESS;
	}
	return SUCCESS;
}

int GameBase::play()
{
	// print board
	print();
	cout << endl;

	bool gameOver = false;

	while (!gameOver)
	{
		int result = turn();
		if (result == USER_QUIT) {
			if (players.size() != 0){
				cout << endl << "Player " << players[currentPlayer] << " has quit." << endl
					<< turns << " turns were played." << endl;
			}
			else {
				cout << "You have quit Sudoku!" << endl;
			}
			return USER_QUIT;
		}

		gameOver = done();
		if (gameOver) {
			if (players.size() != 0){
				cout << "Player " << players[!currentPlayer] << " has won!" << endl
					<< turns << " turns were played." << endl;
			}
			else{
				cout << "You successfully solved Sudoku!" << endl;
			}
			return SUCCESS;
		}

		gameOver = draw();
		if (gameOver) {
			if (players.size() != 0){
				cout << "It's a draw after " << turns << " turns!" << endl;
				return DRAW_RESULT;
			}
			else {
				cout << "Looks like there is a mistake somewhere... " << endl << "Keep trying!" << endl;
				gameOver = !gameOver;
			}
		}
		if (players.size() != 0) turns++;
	}
	return SUCCESS;
}
