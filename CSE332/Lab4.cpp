// 332 Lab 4: Sean McManus, Allen Osgood, Daniel Margulis
// Lab4.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <memory>
#include <fstream>
#include <algorithm>
#include <functional>

using namespace std;

#include "Header1.h"
#include "Games.h"
#include "GameBase.cpp"
#include "TicTacToe.cpp"
#include "Gomoku.cpp"
#include "Sudoku.cpp"
#include "UltimateTicTacToe.cpp"


int main(int argc, char* argv[])
{
	try {
		GameBase::chooseGame(argc, argv);
		return GameBase::instance()->play();
	}
	catch (result_ e) {
		if (e == BAD_NAME) return usage_message(argv[0], "TicTacToe|Gomoku|Sudoku|UltimateTicTacToe");
		return e;
	}
}

int usage_message(string program_name, string info)
{
	cout << "Usage: " << program_name << " " << info << endl;
	return INVALID_START;
}
