#include "TdLearningModel.h"
#include <limits>

int largeValueTile[5] = {0};

TdLearningModel::TdLearningModel()
{
	m_learningBoard.initailBoard();
	m_learningBoard.generateMovingTable();
	m_learningRate = 0.0025;
	m_gameCount = 0;
	m_highestScore = 0;
	m_averageScore = 0;
	m_maxTileValue = 0;
	tupleHandler = new TupleNetwork();
	//tupleHandlerStep2 = new TupleNetwork(2);
	//tupleHandler->load_tuple_network("TupleNetwork/tupleTable");
}

void TdLearningModel::startTraining(int Round)
{
	BitBoard* BigValueBoard = new BitBoard[500000];
	int BigBoardCount = 0;

	MoveDirection nextMove;
	for(int i=0;i<Round;i++)
	{
		while(!m_learningBoard.isGameOver())
		{
			nextMove = getNextBestMove(m_learningBoard);
			m_learningBoard.move(nextMove);
			m_stateStacks.push(m_learningBoard);
			m_learningBoard.addTile();
			m_stateStacks.push(m_learningBoard);
			//m_learningBoard.showBoard();
		}

		if(m_learningBoard.getMaxValueInBoard()>=12) // >=1536
			BigValueBoard[BigBoardCount++] = m_learningBoard.copyCurrentBoard();

		learnEvaluation();
		m_gameCount++;
		updateLearningState();

		/*reset board*/
		BitBoard newBoard;
		newBoard.initailBoard();
		m_learningBoard = newBoard.copyCurrentBoard();
	}
	tupleHandler->save_tuple_network("TupleNetwork/tupleTable");

	ofstream fout("TupleNetwork/BigValueBoard");
	fout.write((char*)BigValueBoard, sizeof(BitBoard) * 500000);

	// 			fin.read((char*)score_table_, sizeof(float) * 1024);
}

MoveDirection TdLearningModel::getNextBestMove(BitBoard currentBoard)
{
	BitBoard boardAfterMoving = currentBoard.copyCurrentBoard();
	MoveDirection direction;
	float score = 0;
	float maxScore = 0.0;
	int maxIndex = 0;
	bool firstLegalDirection = true;
	for(int i=0;i<4;i++)
	{
		direction = static_cast<MoveDirection> (i);
		boardAfterMoving.move(direction);
		score = boardAfterMoving.getScore() - currentBoard.getScore();
		score += evaluateState(boardAfterMoving);
		if(boardAfterMoving.getBoard()==currentBoard.getBoard())
		{
			boardAfterMoving = currentBoard.copyCurrentBoard();
			continue;
		}
		if (firstLegalDirection || maxScore < score)
		{
			maxScore = score;
			maxIndex = i;
			//cout<<"here"<<i<<"score is "<<maxScore<<endl;
		}
		boardAfterMoving = currentBoard.copyCurrentBoard();
		firstLegalDirection = false;
	}
	return static_cast<MoveDirection> (maxIndex);
}

float TdLearningModel::evaluateState(BitBoard board)
{
	return tupleHandler->getBoard_value(board);

	//return 0;
	if(board.getMaxValueInBoard() >= 12) // if tile >= 1536
		return tupleHandlerStep2->getBoard_value(board);
	else
		return tupleHandler->getBoard_value(board);
}

void TdLearningModel::learnEvaluation()
{
	/* (s+1)' - s' */
	BitBoard topBoard;
	BitBoard afterStateBoard , nextAfterStateBoard;
	MoveDirection nextMove;
	bool startTrackingBack = false;
	float newScore;
	int reward = 0;
	while(!m_stateStacks.empty())
	{
		topBoard = m_stateStacks.top();

		if(startTrackingBack)
		{
			nextAfterStateBoard = topBoard;
			nextMove = getNextBestMove(topBoard);
			nextAfterStateBoard.move(nextMove);
			reward = nextAfterStateBoard.getScore() - topBoard.getScore();
		}

		m_stateStacks.pop();
		afterStateBoard = m_stateStacks.top();
		newScore = 0;
		if(!startTrackingBack)
			newScore = 0 - evaluateState(afterStateBoard);
		else
		{
			newScore += reward;
			newScore += evaluateState(nextAfterStateBoard) - evaluateState(afterStateBoard);	//v(s'next) - v(s')
		}
		updateStateValue(afterStateBoard, newScore);
		m_stateStacks.pop();
		startTrackingBack = true;
	}
}

void TdLearningModel::updateStateValue(BitBoard board , float score)
{
	return tupleHandler->update_board_value(board,m_learningRate * score);


	if(board.getMaxValueInBoard() >= 12) // if tile >= 1536
		return tupleHandlerStep2->update_board_value(board,m_learningRate * score);
	else
		return tupleHandler->update_board_value(board,m_learningRate * score);
}

void TdLearningModel::updateLearningState()
{

	int currentScore = m_learningBoard.getScore();
	int maxTileValue = m_learningBoard.getMaxValueInBoard();
	m_averageScore += currentScore;

	if(currentScore > m_highestScore)
		m_highestScore = currentScore;
	if(maxTileValue > m_maxTileValue)
		m_maxTileValue = maxTileValue;

	if(maxTileValue >= 13)
		largeValueTile[4]++;
	else if(maxTileValue >= 12)
		largeValueTile[3]++;
	else if(maxTileValue >= 11)
		largeValueTile[2]++;
	else if(maxTileValue >= 10)
		largeValueTile[1]++;
	else if(maxTileValue >= 9)
		largeValueTile[0]++;

	if(m_gameCount%1000 == 0)
	{
		m_averageScore /= 1000.0;
		cout<<"\n===========================================\n";
		cout<<"Games Count:  "<<m_gameCount<<endl;
		cout<<"highest score is "<<m_highestScore<<endl;
		cout<<"max tile is "<<FixedValue::valueTable[m_maxTileValue]<<endl;
		cout<<"average score in 1000 games is "<<m_averageScore<<endl;
		cout<<"count of 192 in 1000 games :"<<largeValueTile[0]<<endl;
		cout<<"count of 384 in 1000 games :"<<largeValueTile[1]<<endl;
		cout<<"count of 768 in 1000 games :"<<largeValueTile[2]<<endl;
		cout<<"count of 1536 in 1000 games :"<<largeValueTile[3]<<endl;
		cout<<"count of 3072 in 1000 games :"<<largeValueTile[4]<<endl;

// 		ofstream outputFile("trainingRecord.txt",ios::out|ios::app);
// 		if(outputFile.is_open())
// 		{
// 			outputFile<<"\n===========================================\n";
// 			outputFile<<"Games Count:  "<<m_gameCount<<endl;
// 			outputFile<<"highest score is "<<m_highestScore<<endl;
// 			outputFile<<"max tile is "<<FixedValue::valueTable[m_maxTileValue]<<endl;
// 			outputFile<<"average score in 1000 games is "<<m_averageScore<<endl;
// 			outputFile<<"count of 192 in 1000 games :"<<largeValueTile[0]<<endl;
// 			outputFile<<"count of 384 in 1000 games :"<<largeValueTile[1]<<endl;
// 			outputFile<<"count of 768 in 1000 games :"<<largeValueTile[2]<<endl;
// 			outputFile<<"count of 1536 in 1000 games :"<<largeValueTile[3]<<endl;
// 			outputFile<<"count of 3072 in 1000 games :"<<largeValueTile[4]<<endl;
// 		}

		m_averageScore = 0;
		for(int j=0;j<5;j++)
			largeValueTile[j] = 0;

		m_maxTileValue = 0;
	}
	
}


