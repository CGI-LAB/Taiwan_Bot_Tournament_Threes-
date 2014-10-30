#ifndef __TDLEARNINGMODEL_H__
#define __TDLEARNINGMODEL_H__
#include "BitBoard.h"
#include "TupleNetwork.h"
#include <stack>

using namespace std;

class TdLearningModel
{
public:
	TdLearningModel();
	void startTraining(int Round);
	float evaluateState(BitBoard board);
	MoveDirection getNextBestMove(BitBoard currentBoard);
	void learnEvaluation();
	void updateStateValue(BitBoard board,float score);
	void updateLearningState();

private:
	stack<BitBoard> m_stateStacks;
	BitBoard m_learningBoard;
	TupleNetwork *tupleHandler;
	TupleNetwork *tupleHandlerStep2;
	float m_learningRate;
	int m_gameCount;
	int m_highestScore;
	float m_averageScore;
	int m_maxTileValue;
};


#endif

