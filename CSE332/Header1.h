// 332 Lab 4: Sean McManus, Allen Osgood, Daniel Margulis
#ifndef HEADER1_H
#define HEADER1_H
int usage_message(string str, string info);

string lower(string & x);

enum array_indices_t{ PROGRAM_NAME, INPUT_FILE_NAME, CMD_LINE_ARG_NUM };

enum result_{
	SUCCESS,
	DRAW_RESULT,
	INVALID_INPUT_STRING,
	INVALID_MOVE,
	USER_QUIT,
	INVALID_START,
	BAD_ALLOC,
	BAD_INSTANCE,
	BAD_PTR,
	BAD_NAME,
	OVERWRITE_INITIAL,
	NO_ORIGINAL_BOARD,
	INVALID_ORIGINAL_BOARD,
	INVALID_SAVED_BOARD,
	INVALID_BOARD_MOVE
};
#endif // !HEADER1_H
