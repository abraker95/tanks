#include "managers/score_manager.h"

int ScoreManager::newPlayer()
{
	list_score.push_back(std::make_pair(0, 0));
	return list_score.size() - 1;
}


void ScoreManager::addWin(int index)
{
	list_score[index].first++;
}

void ScoreManager::addLoss(int index)
{
	list_score[index].second++;
}

void ScoreManager::getStatus(int index, int& win, int& loss)
{
	win = list_score[index].first;
	loss = list_score[index].second;
}
