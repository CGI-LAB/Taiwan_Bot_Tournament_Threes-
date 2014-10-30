#include "Threes.h"

#include "BitBoard.h"
#include "ExpectiMax.h"
#include "NewTilePredictor.h"

#include <ctime>
#ifdef _WIN32
#include <conio.h>
#elif defined(__linux__)
#include <termios.h>
#include <unistd.h>
#include <stdio.h>
#endif

dir_e dirTransform(MoveDirection myDir)
{
	if(myDir == Right)
		return RIGHT;
	else if(myDir == Left)
		return LEFT;
	else if(myDir == Up)
		return UP;
	else if(myDir == Down)
		return DOWN;
		
	return INVALID;
}



void transformToBitBoard(Game& inputGame,BitBoard& inputBoard,bool isInitialized)
{
	int tileValueTable[6145];
	tileValueTable[0] = 0;
	tileValueTable[1] = 1;
	tileValueTable[2] = 2;
	tileValueTable[3] = 3;
	tileValueTable[6] = 4;
	tileValueTable[12] = 5;
	tileValueTable[24] = 6;
	tileValueTable[48] = 7;
	tileValueTable[96] = 8;
	tileValueTable[192] = 9;
	tileValueTable[384] = 10;
	tileValueTable[768] = 11;
	tileValueTable[1536] = 12;
	tileValueTable[3072] = 13;
	tileValueTable[6144] = 14;

	Grid currentGrid;
	inputGame.getCurrentGrid(currentGrid);

	///////////////// set m_board///////////////////
	unsigned long long boardValue = 0ull;
	int shiftNum = 60;
	int encodeValue;
	for(int i=0;i<16;i++,shiftNum-=4)
	{
		encodeValue = tileValueTable[currentGrid[i]];
		if(isInitialized)	//if only take off from bag in initialized turn , others take off in bitBoard addinTile()
			inputBoard.takeOffTileInBoard(encodeValue);

		unsigned long long gridValue =  static_cast<unsigned long long>(encodeValue);
		boardValue +=   (gridValue <<shiftNum);
	}
	inputBoard.setBoard(boardValue);

	///////////////// set m_hintTile///////////////////
	char index = inputGame.getHint();
	int hintTile;
	if(index!='+')
		hintTile = tileValueTable[index-'0'];
	else
		hintTile = 4;
	inputBoard.setHintTile(hintTile);

	///////////////// set score ///////////////////
	inputBoard.setScore(currentGrid.getScore());


	///////////////// set maxValue ///////////////////
	encodeValue = tileValueTable[currentGrid.getMaxTile()];
	inputBoard.setMaxValue(encodeValue);
}

ExpectiMax emm(5,"tupleTable_before_1536");

void PlayNRounds(int n){

	int score;
	Game myGame;
	bool isGameOver;
	//ExpectiMax emm(5,"tupleTable_before_1536");
	if(myGame.isGameOver(score))  myGame.reset();
	
	BitBoard myBoard;
	transformToBitBoard(myGame,myBoard,true);
	int moveCount = 0;

	for(int i = 0;i < n;i++){
		moveCount = 0;
		isGameOver = false;
		while(!isGameOver){

			MoveDirection myDirection = emm.find_best_move(myBoard);
			dir_e dir = dirTransform(myDirection);

			myGame.insertDirection(dir);
			myBoard.move(myDirection);
			isGameOver = myGame.isGameOver(score);
			transformToBitBoard(myGame,myBoard,false);

			/////////////set last move//////////////////////
			myBoard.setLastMove(myDirection);

			/////////////set tile predictor/////////////////
			if(myGame.getHint()!='+')
				myBoard.takeOffTileInBoard(myGame.getHint()-'0');
			moveCount++ ;
		}
		if(i <= n - 1)  myGame.reset();
		transformToBitBoard(myGame,myBoard,true);
		printf("Move : %d\n", moveCount);
		printf("  Round:    %d      \n", i+1);
		printf("  Score:    %d      \n", score);
	}
}

int main(int argc, char* argv[]){
	// Note: API function calls performed by any 'Game' object effects the same set of static class members,
	// so even though the 2 following function calls use different 'Game' objects, the same game continues
	srand(time(0));
	BitBoard generateTableBoard;
	generateTableBoard.generateMovingTable();
	
	PlayNRounds(100);
	//PlayNRounds(1000);
	return 0;
}

