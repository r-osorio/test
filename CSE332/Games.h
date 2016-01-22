// 332 Lab 4: Sean McManus, Allen Osgood, Daniel Margulis
// Defines the overal structure of the games,
//	along with the different types available

#ifndef GAMES_H
#define GAMES_H

class GameBase {
protected:
	static shared_ptr<GameBase> ptr;

	vector<string> players;
	vector<string> v;
	vector<int> initial;
	
	int width, height, spaces;
	int currentPlayer;
	int turns;

public:
	static void chooseGame(int argc, char* argv[]);
	static shared_ptr<GameBase> instance();

	// pure virtual methods
	virtual bool done() = 0;
	virtual bool draw() = 0;
	virtual int turn() = 0;
	virtual void print() = 0;

	virtual int prompt(unsigned int&, unsigned int&);
	virtual int prompt(unsigned int&, unsigned int&, unsigned int&);

	int play();

};

class TicTacToe : public GameBase {

public:
	void set_values();
	virtual bool done();
	virtual bool draw();
	virtual int turn();
	virtual void print() override {
		cout << *this << endl;
	};

	TicTacToe() {
		set_values();
	}

	friend ostream& operator<<(ostream& stream, const TicTacToe &game);
	friend class UltimateTicTacToe;
};

class Gomoku : public GameBase {

public:
	void set_values();
	virtual bool done();
	virtual bool draw();
	virtual int turn();
	virtual void print() override {
		cout << *this << endl;
	};

	Gomoku() {
		set_values();
	}

	friend ostream& operator<<(ostream& stream, const Gomoku &game);
};

class Sudoku : public GameBase {

public:
	void set_values();
	virtual bool done();
	virtual bool draw();
	virtual int turn();
	virtual void print() override {
		cout << *this << endl;
	};

	Sudoku() {
		set_values();
	}

	friend ostream& operator<<(ostream& stream, const Sudoku &game);
};

class UltimateTicTacToe : public GameBase {

protected:
	vector<string *> ult;
	vector<TicTacToe *> games;
	vector<string> results;

public:
	void set_values();
	virtual bool done();
	virtual bool draw();
	virtual int turn();
	virtual void print() override {
		cout << *this << endl;
	};

	string getBoardValue(unsigned int&, unsigned int&, const TicTacToe &game);

	UltimateTicTacToe() {
		set_values();
	}

	friend ostream& operator<<(ostream& stream, const UltimateTicTacToe &game);
};
#endif
