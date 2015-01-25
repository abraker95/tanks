#pragma once

#include <vector>
#include <utility>

class ScoreManager
{
public:
	int newPlayer();
	void addWin(int index);
	void addLoss(int index);
	void getStatus(int index, int& win, int& loss);

private:
	std::vector<std::pair<int, int>> list_score;
};
