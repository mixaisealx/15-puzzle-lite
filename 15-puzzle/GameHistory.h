#pragma once
#include "pch.h"

template<class T>
class GameHistory {
public:
	using ErrorMsgCallback = void (T:: *)(const wchar_t *text, const wchar_t *title);

	GameHistory(ErrorMsgCallback msg_func, T *receiver);
	~GameHistory();

	void Clear();

	bool Undo(unsigned char &direction);

	void Event_tileMoved(unsigned char direction);

	void SaveBoard(const char *board);
	void RestoreBoard(char *board);

private:
	size_t undo_array_size = 32;
	char *board, byte_data_pos = -1;
	unsigned char *undo_array, 
		*ldata_pos, *write_end;

	ErrorMsgCallback msg;
	T *receiver;

	GameHistory &operator=(const GameHistory &other);
	GameHistory(const GameHistory &other);
};

template<class T>
GameHistory<T>::GameHistory(ErrorMsgCallback msg_func, T *receiver): msg(msg_func), receiver(receiver){
	board = (char *)calloc(16, 1);
	ldata_pos = undo_array = (unsigned char *)malloc(32);
	write_end = undo_array + 32;
}

template<class T>
GameHistory<T>::~GameHistory() {
	free(board);
	free(undo_array);
}

template<class T>
inline void GameHistory<T>::Clear() {
	ldata_pos = undo_array;
	byte_data_pos = -1;
}

template<class T>
inline void GameHistory<T>::Event_tileMoved(unsigned char direction) {
	if (++byte_data_pos == 4) {
		if (++ldata_pos == write_end) {
			int cpos = ldata_pos - undo_array;
		#pragma warning (push)
		#pragma warning (disable: 6308)
			ldata_pos = (unsigned char *)realloc(undo_array, undo_array_size <<= 1);
		#pragma warning (pop)
			if (!ldata_pos) {
				if (undo_array_size)
					(receiver->*msg)(L"You have made too many moves.\nI can't remember that much ...\nYou are a machine as much as I am ...", L"People say that programs do not know how to be surprised, but I was able to");
				else 
					(receiver->*msg)(L"You have made too many moves.\nTo continue, add some RAM to this computer, but for now my credentials are all ...", L"People say that programs do not know how to be surprised, but I was able to");
				this->~GameHistory();
			}
			ldata_pos = (undo_array = ldata_pos) + cpos;
			write_end = undo_array + undo_array_size;
		}

		*ldata_pos = direction;
		byte_data_pos = 0;
	} else {
		char bdata = byte_data_pos << 1;
		*ldata_pos = *ldata_pos & ~(0x3 << bdata) | (direction << bdata);
	}
}

template<class T>
inline bool GameHistory<T>::Undo(unsigned char &direction) {
	if (byte_data_pos != -1) {
		direction = (*ldata_pos >> (byte_data_pos << 1));
		if (--byte_data_pos == -1) {
			if (ldata_pos != undo_array) {
				--ldata_pos;
				byte_data_pos = 3;
				return true;
			} else
				return false;
		}
		return true;
	}
	return false;
}

template<class T>
inline void GameHistory<T>::RestoreBoard(char *board) {
	memcpy(board, this->board, 16);
	Clear();
}

template<class T>
inline void GameHistory<T>::SaveBoard(const char *board) {
	memcpy(this->board, board, 16);
}