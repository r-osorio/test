// 332 Lab 4: Sean McManus, Allen Osgood, Daniel Margulis
// Defines all methods and printing for playing 
//	Sudoku

#include "stdafx.h"
#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <memory>
#include <fstream>
#include <algorithm>

using namespace std;

void Sudoku::set_values()
{
	width = height = 9;
	spaces = width*height;

	string save;
	string line;

	bool readSaved = false;
	bool readOrig = false;

	ifstream inStream;
	inStream.open("Sudoku.txt");
	if (inStream.is_open()){
		getline(inStream, save);
		// check for saved game
		if (save == "Save"){
			readSaved = true;
		}
		else { // there is not an actively saved game
			inStream.close();
		}
	}

	if (!readSaved) {
		// attempt to open default
		inStream.open("sudoku0");
		if (inStream.is_open()) {
			readOrig = true;
		}
		// there is no default board to read
		else {
			throw NO_ORIGINAL_BOARD;
		}
	}

	// read in the board
	int count = 0;
	while (getline(inStream, line)) {
		// erase any trailing whitespace
		// http://stackoverflow.com/questions/216823/whats-the-best-way-to-trim-stdstring
		line.erase(find_if(line.rbegin(), line.rend(), not1(ptr_fun<int, int>(isspace))).base(), line.end());

		// validate the line input
		int num;
		stringstream s(line);
		while (s.rdbuf()->in_avail() > 0) {
			if (!(s >> num)) throw (readOrig ? INVALID_ORIGINAL_BOARD : INVALID_SAVED_BOARD);
			else if (num < 0 || num > 9 || count >= spaces) throw (readOrig ? INVALID_ORIGINAL_BOARD : INVALID_SAVED_BOARD);
			
			if (readOrig && num != 0) initial.push_back(count);
			v.push_back(to_string(num));
			count++;
		}
		if (readSaved && v.size() == spaces) break;
	}

	// check that the correct number of spaces were read in
	if (v.size() != spaces) throw (readOrig ? INVALID_ORIGINAL_BOARD : INVALID_SAVED_BOARD);

	// reading the saved we have to read the initial indicies after the saved board
	if (readSaved) {
		int num;
		while (getline(inStream, line)){
			stringstream s(line);
			if (!(s >> num)) throw INVALID_SAVED_BOARD;
			else if (num < 0 || num > 80) throw INVALID_SAVED_BOARD;
			initial.push_back(num);
		}
	}
	inStream.close();
}


bool Sudoku::done()
{
	bool done = true;

	// check for filled board
	for (int i = 0; i < width*height && done; i++) {
		done = v[i] != "0";
	}

	if (!done) return false;

	// check for completion horizontally
	for (int i = 0; i < height && done; i++){
		vector<string> v1;
		// check moving horizontally along row
		for (int j = 0; j < width; j++) {
			int index = width*i + j;
			v1.push_back(v[index]);
		}
		// sort the vector to use with unique function
		sort(v1.begin(), v1.end());
		auto last = unique(v1.begin(), v1.end());
		v1.erase(last, v1.end());
		done = (v1.size() == 9);
	}

	if (!done) return false;

	// check for completion vertically
	for (int j = 0; j < width && done; j++) {
		vector<string> v1;

		// check moving vertically up col
		for (int i = 0; i < height; i++){
			int index = width*i + j;
			v1.push_back(v[index]);
		}

		// sort the vector to use with unique function
		sort(v1.begin(), v1.end());
		auto last = unique(v1.begin(), v1.end());
		v1.erase(last, v1.end());
		done = (v1.size() == 9);
	}

	if (!done) return false;

	// check each inner box for completion
	for (int i = 0; i < height && done; i += 3) {
		for (int j = 0; j < width && done; j += 3) {

			int boxCorner = width*i + j;
			vector<string> v1;
			// iterate through the inner box to get all 9 nums
			for (int k = 0; k < 3; k++) {
				for (int l = 0; l < 3; l++) {
					int index = boxCorner + width*k + l;
					v1.push_back(v[index]);
				}
			}
			sort(v1.begin(), v1.end());
			auto last = unique(v1.begin(), v1.end());
			v1.erase(last, v1.end());
			done = (v1.size() == 9);
		}
	}

	if (!done) return false;

	// here we know the game is over
	ofstream main_stream;
	main_stream.open("Sudoku.txt");
	if (main_stream.is_open()){
		main_stream << "No Save" << endl;
	}
	main_stream.close();
	return true;
}

bool Sudoku::draw()
{
	bool draw = true;

	for (int i = 0; i < width*height && draw; i++) {
		draw = v[i] != "0";
	}
	return draw;
}

int Sudoku::turn()
{
	unsigned int col;
	unsigned int row;
	unsigned int num;

	// prompt guarantees a valid string or quit 
	int result = prompt(col, row, num);
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
		main_stream.open("Sudoku.txt");
		if (response == "yes"){
			if (main_stream.is_open()){
				main_stream << "Save" << endl;
				// write all the moves to the file
				for (unsigned int i = 0; i < v.size(); i++){
					main_stream << v[i] << endl;
				}
				// append the indicies of the initial board to the file
				for (unsigned int j = 0; j < initial.size(); j++){
					main_stream << initial[j] << endl;
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
	v[width*row + col] = to_string(num);

	// print the board
	cout << *this << endl << endl;

	return SUCCESS;
}

ostream& operator<<(ostream& stream, const Sudoku &game)
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
