#ifndef __BITBOARD_H__
#define __BITBOARD_H__
#include <vector>
#include "FixedValue.h"
#include "tileGenerator.h"
#include "NewTilePredictor.h"

using namespace std;

enum MoveDirection{Right,Left,Down,Up};

class BitBoard
{
public:
	BitBoard();
	~BitBoard();
	BitBoard(unsigned long long sBoard);
	BitBoard copyCurrentBoard();
	void initailBoard();
	//void initailValueTable();	// 4bits represent tile's value
	int getValueFromTable(int Index){return FixedValue::valueTable[Index];}
	void move(MoveDirection Direction);
	void addTile();
	void generateMovingTable();
	int getMaxValueInBoard();	// return the max value in board (0~15, not 0~6144)
	int getRowScore(unsigned int row){return FixedValue::scoreTable[row];}
	int getScore(){return m_gameScore;}
	void setScore(int newScore){m_gameScore = newScore;}
	inline unsigned long long getBoard() const {return m_board;}
	void setBoard(unsigned long long specificBoard) {m_board = specificBoard;}
	void setHintTile(int specificHintTile) {m_hintTile = specificHintTile;}
	int getHintTile() {return m_hintTile;}
	void setMaxValue(int specificValue) {m_maxValueInBoard = specificValue;}
	int getMaxValue() {return m_maxValueInBoard;}
	void setLastMove(MoveDirection dir) {m_lastMove = dir;}


	void get_isomorphic_boards(BitBoard isomorphic_boards[]) const;
	void refreshHintTile();
	inline int getHintTile() const {return m_hintTile;}
	void getBoardTileInArrayForm(int destinationArray[16]);
	int get_successor(MoveDirection successors[]);
	int get_all_possible_board(BitBoard boards[], float boardsPossibility[], bool has_hint=false);
	float get_probability_from_predictor(int inputNum){return m_tilePredictor.getProbability(inputNum,m_maxValueInBoard);}
	int get_total_count_from_predictor() {return m_tilePredictor.getTotalCount();}
	int get_tile_count_from_predictor(int Tile){return m_tilePredictor.getCountOfTile(Tile);}
	void takeOffTileInBoard(int tile){m_tilePredictor.takeOffTile(tile);}



	int power3(int exponent);
	void showHex(unsigned long long input);
	void showHex2(unsigned int input);
	void showBoard();
	bool isGameOver();

private:
	BitBoard get_flip_board() const;
	BitBoard get_mirror_board() const;
	BitBoard get_flip_diagonal_3_12_board() const;
	BitBoard get_flip_diagonal_0_15_board() const;

private:
	unsigned long long m_board;
	int m_maxValueInBoard;
	//tileGenerator m_tileGenerator;
	unsigned long long m_possibleNextTile[4];
	int m_addInTileValue[4]; // 0 or 3~11 , four elements are the same
	int m_possibleTileCount;
	int m_gameScore;
	int m_hintTile;
	MoveDirection m_lastMove;
	NewTilePredictor m_tilePredictor;
};

#endif

