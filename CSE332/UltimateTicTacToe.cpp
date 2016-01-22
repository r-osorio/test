// 332 Lab 4: Sean McManus, Allen Osgood, Daniel Margulis
// Defines all methods and printing for playing 
//	Ultimate Tic Tac Toe

#include "stdafx.h"
#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <memory>
#include <fstream>
#include <algorithm>

using namespace std;

void UltimateTicTacToe::set_values()
{
	players = { "X", "O" };
	currentPlayer = 0;
	turns = 0;
	width = height = 9;
	spaces = width * height;

	string save;
	string piece;
	string turn;
	bool readSaved = false;
	int count = 0;

	// create the 9 inner TicTacToe games
	for (int i = 0; i < 9; i++) {
		games.push_back(new TicTacToe);
	}

	// initialize ult vector of pointers to TicTacToe boards
	for (int i = 0; i < spaces; i++) ult.push_back(nullptr);

	// link the big board and smaller ones
	for (int i = 0; i < height; i += 3) {
		for (int j = 0; j < width; j += 3) {
			TicTacToe &g = *games[i + (j / 3)];
			int boxCorner = width*i + j;
			for (int k = 0; k < 3; k++) {
				for (int l = 0; l < 3; l++) {
					ult[boxCorner + width*k + l] = &g.v[4 * (k + 1) + (l + 1)];
				}
			}
		}
	}

	ifstream inStream;
	inStream.open("UltimateTicTacToe.txt");
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

			// read in board and fill results vector
			while (getline(inStream, piece) && count < spaces + height) {
				
				// get the number of turns and check that input is valid
				if (piece == "X" || piece == "O" || piece == "C") turns++;
				else if (piece != " ") throw INVALID_SAVED_BOARD;

				if (count < spaces) {
					v.push_back(piece);
					*ult[count] = piece;
				}
				else {
					results.push_back(piece);
				}
				count++;
			}

			// check that the correct number of spaces have been inserted
			if (v.size() != spaces || results.size() != 9) throw INVALID_SAVED_BOARD;
			readSaved = true;

			for (int i = 0; i < 9; i++) {
				cout << "Results[" << i << "] = " << results[i] << endl;
			}
		}
	}
	inStream.close();

	// reset to default board if needed
	if (!readSaved) {
		for (int i = 0; i < spaces; ++i) v.push_back(" ");
		for (int i = 0; i < height; ++i) results.push_back(" ");
	}
}


bool UltimateTicTacToe::done()
{
	bool winner = false;
	unsigned int k = 0;

	// check for newly won square
	while (k < games.size() && !winner) {
		if (results[k] == " ") winner = games[k]->done();
		k++;
	}

	if (winner) {
		k--;
		// record the inner win and check for outer win
		results[k] = players[!currentPlayer];
		cout << "Board " << (k+1) << " has a winner!" << endl;

		// fill v so that the board can't have more plays
		int r = k - (k % 3);
		int c = (k % 3) * 3;
		int boxCorner = width*r + c;
		for (int i = 0; i < 3; i++) {
			for (int j = 0; j < 3; j++) {
				v[boxCorner + width*i + j] = results[k];
			}
		}

		// check for win in outer game
		winner = false;

		//check for UltimateTicTacToe win in results by moving diagonally up board
		bool row, col, dL, dR;
		int n = 3;

		for (int i = 0; i < n && !winner; ++i) {
			// use the diagonal square as the anchor
			int index = (n * i) + i;
			string str = results[index];

			row = col = dL = dR = !(str == " " || str == "C");

			// start at the respective extreme and go through the diagonal to the end
			for (int j = 0; j < n && (row || col || dL || dR); ++j) {
				row &= str == results[index + j - i]; //left to right
				col &= str == results[index + n*(j - i)]; // bottom to top
				if (i == n / 2) {
					dL &= str == results[index + (n + 1)*(j - i)]; // bottom right to top left
					dR &= str == results[index + (n - 1)*(j - i)]; // bottom left to top right
				}
			}
			winner = row || col;
			if (i == n / 2) winner |= dL || dR;
			//if (winner) cout << "Row: " << row << " Col: " << col << " dL: " << dL << " dR: " << dR << " i: " << i << endl;
			//if (winner) for (int i = 0; i < 9; i++) cout << "Results[" << i << "] = " << results[i] << endl;
		}
		//if (winner) cout << "Row: " << row << " Col: " << col << " dL: " << dL << " dR: " << dR << endl;
	}

	if (winner){
		ofstream main_stream;
		main_stream.open("UltimateTicTacToe.txt");
		if (main_stream.is_open()){
			main_stream << "No Save" << endl;
		}
		main_stream.close();
		turns++;

		// print the final board
		cout << *this << endl << endl;
	}

	return winner;
}

bool UltimateTicTacToe::draw()
{
	bool draw = false;
	unsigned int k = 0;

	// check for newly drawn square
	while (k < games.size() && !draw) {
		if (results[k] == " ") draw = games[k]->draw();
		k++;
	}

	if (draw) {
		k--;
		// mark the square as a draw
		results[k] = "C";
		cout << "Board " << (k+1) << " is a draw!" << endl;

		// overwrite the values of the game in v
		// get the box corner locations
		int r = k - (k % 3);
		int c = (k % 3) * 3;
		int boxCorner = width*r + c;
		// fill v so that the board can't have more plays
		for (int i = 0; i < 3; i++) {
			for (int j = 0; j < 3; j++) {
				v[boxCorner + width*i + j] = "C";
			}
		}
	}

	// there is a draw in the outer game if 'results' contains no whitespace
	draw = false;
	draw = find(results.begin(), results.end(), " ") == results.end();

	// write to output
	if (draw){
		ofstream main_stream;
		main_stream.open("UltimateTicTacToe.txt");
		if (main_stream.is_open()){
			main_stream << "No Save" << endl;
		}
		main_stream.close();
		turns++;

		// print the final board
		cout << *this << endl << endl;
	}
	return draw;
}

int UltimateTicTacToe::turn()
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
		main_stream.open("UltimateTicTacToe.txt");
		if (response == "yes"){
			if (main_stream.is_open()){
				main_stream << "Save" << endl;
				main_stream << players[currentPlayer] << endl;

				for (unsigned int i = 0; i < v.size(); i++){
					main_stream << v[i] << endl;
				}
				for (unsigned int i = 0; i < results.size(); i++) {
					main_stream << results[i] << endl;
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

	// scale row and col
	row = row - 1;
	col = col - 1;

	// make the move
	int index = width*row + col;
	v[index] = players[currentPlayer];
	*ult[index] = players[currentPlayer];

	// print the board
	cout << *this << endl << endl;

	// switch the player after the method called
	currentPlayer = !currentPlayer;
	return SUCCESS;
}


ostream& operator<<(ostream& stream, const UltimateTicTacToe &game)
{
	// print from top left corner across, then jump to next line
	//	to start printing the next row, all the way until the bottom right
	unsigned int index;
	int width = game.width;
	int height = game.height;

	cout << endl;

	for (int i = height; i >= 0; --i)
	{
		// print the line separator
		if (i % 3 == 0) cout << endl << "   " << "=======================================" << endl;
		else cout << endl << "   " << "---------------------------------------" << endl;

		for (int j = 0; j <= width; ++j)
		{

			// print the label along left col
			if (j == 0) cout << " " << i << (i == 0 ? "" : "||");
			// print all labels along bottom
			else if (i == 0) cout << (j % 3 == 1 ? "   " : "  ") << j << " ";
			// print the board
			else {
				index = width*(i - 1) + (j - 1); 
				cout << " " << (game.v[index] == "0" ? " " : game.v[index]) << (j % 3 == 0 ? " ||" : " |");
			}
		}
	}
	return stream;
}
