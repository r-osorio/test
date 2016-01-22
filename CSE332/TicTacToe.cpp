// 332 Lab 4: Sean McManus, Allen Osgood, Daniel Margulis
// Defines all methods and printing for playing 
//	Tic Tac Toe

#include "stdafx.h"
#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <memory>
#include <fstream>
#include <algorithm>

using namespace std;

void TicTacToe::set_values()
{
	players = { "X", "O" };
	currentPlayer = 0;
	turns = 0;
	width = height = 4;
	spaces = width * height;

	string save;
	string piece;
	string turn;
	bool readSaved = false;
	int count = 0;

	ifstream inStream;
	inStream.open("TicTacToe.txt");
	if (inStream.is_open()){
		getline(inStream, save);
		if (save == "Save"){
			getline(inStream, turn);
			if (turn == "X"){
				currentPlayer = 0;
			}
			else if (turn == "O") {
				currentPlayer = 1;
			}
			else throw INVALID_SAVED_BOARD;

			while (getline(inStream, piece)) {
				// get the number of turns and check that input is valid
				if (piece == "X" || piece == "O") turns++;
				else if ((piece != "X" && piece != "O" && piece != " ") || count >= spaces) throw INVALID_SAVED_BOARD;
		
				v.push_back(piece);
				count++;
			}

			// check that the correct number of spaces have been inserted
			if (v.size() != spaces) throw INVALID_SAVED_BOARD;
			readSaved = true;
		}
	}
	inStream.close();

	// reset to default board
	if (!readSaved) for (int i = 0; i < spaces; ++i) v.push_back(" ");
}

bool TicTacToe::done()
{
	bool winner = false;
	// check each row
	for (int i = 1; i < height && !winner; ++i)
	{
		int index = width*i + 1;
		winner = !v[index].compare(" ") == 0 && v[index].compare(v[index + 1]) == 0 && v[index + 1].compare(v[index + 2]) == 0;
	}

	// check each column
	for (int i = 1; i < width && !winner; ++i)
	{
		int index = width + i;
		winner = !v[index].compare(" ") == 0 && v[index].compare(v[index + width]) == 0 && v[index + width].compare(v[index + 2*width]) == 0;
	}

	// check the diagonals
	if (!winner) winner = !v[width + 1].compare(" ") == 0 && v[width + 1].compare(v[2*width + 2]) == 0 && v[2*width + 2].compare(v[3*width + 3]) == 0;
	if (!winner) winner = !v[width + 3].compare(" ") == 0 && v[width + 3].compare(v[2*width + 2]) == 0 && v[2*width + 2].compare(v[3*width + 1]) == 0;

	if (winner){
		ofstream main_stream;
		main_stream.open("TicTacToe.txt");
		if (main_stream.is_open()){
			main_stream << "No Save" << endl;
		}
		main_stream.close();
		turns++;
	}

	return winner;
}

bool TicTacToe::draw()
{
	bool draw = true;
	// if any space contains empty string, game is not a draw
	for (int i = 1; i < height && draw; ++i)
	{
		int index = width*i + 1;
		draw = !(v[index].compare(" ") == 0) && !v[index + 1].compare(" ") == 0 && !v[index + 2].compare(" ") == 0;
	}
	if (draw){
		ofstream main_stream;
		main_stream.open("TicTacToe.txt");
		if (main_stream.is_open()){
			main_stream << "No Save" << endl;
		}
		main_stream.close();
		turns++;
	}
	return draw;
}

int TicTacToe::turn()
{
	cout << "It is Player " << players[currentPlayer] << "'s turn." << endl;

	unsigned int col;
	unsigned int row;

	// prompt guarantees a valid string or quit 
	int result = prompt(col, row);
	if (result == USER_QUIT){
		cout << "Do you wish to save? (yes/no)" << endl;
		bool valid_response = false;
		string response;
		while (!valid_response){
			cin >> response;
			if (response == "yes" || response == "no"){
				valid_response = true;
			}
			else {
				cout << "Please enter yes or no" << endl;
			}
		}
		ofstream main_stream;
		main_stream.open("TicTacToe.txt");
		if (response == "yes"){
			if (main_stream.is_open()){
				main_stream << "Save" << endl;
				main_stream << players[currentPlayer] << endl;

				for (unsigned int i = 0; i < v.size(); i++){
					main_stream << v[i] << endl;
				}
			}
		}
		else {
			if (main_stream.is_open()){
				main_stream << "No Save" << endl;
			}
		}
		main_stream.close();
		return USER_QUIT;
	}

	// make the move
	v[width*row + col] = players[currentPlayer];

	// print the board
	cout << *this << endl << endl;

	// print all valid moves the player has made
	cout << "Player " << players[currentPlayer] << ": ";
	for (int i = 1; i < height; ++i)
	{
		for (int j = 1; j < width; ++j)
		{
			if (v[width*i + j].compare(players[currentPlayer]) == 0) cout << j << "," << i << "; ";
		}
	}
	cout << endl << endl;

	// switch the player after the method called
	currentPlayer = !currentPlayer;
	return SUCCESS;
}


ostream& operator<<(ostream& stream, const TicTacToe &game)
{
	// print from top left corner across, then jump to next line
	//	to start printing the next row, all the way until the bottom right
	unsigned int index;
	int width = game.width;
	int height = game.height;

	cout << endl;

	for (int i = height - 1; i >= 0; --i)
	{
		cout << endl << "  " << "-----------" << endl;

		for (int j = 0; j < width; ++j)
		{
			// print the label along left col
			if (j == 0) cout << i << (i == 0 ? "" : "| ");
			// print all labels along bottom
			else if (i == 0) cout << "  " << j << " ";
			// print the board
			else {
				index = width*i + j;
				cout << game.v[index];
				if (j < width && i < height) cout << " | ";
			}
		}
	}
	return stream;
}
