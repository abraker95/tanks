#include "components/Score.h"

Score::Score(unsigned _wins, unsigned _losses): wins(_wins), losses(_losses) {}

void Score::addWin()
{
	wins++;
}

unsigned Score::getWins() const
{
	return wins;
}

void Score::addLoss()
{
	losses++;
}

unsigned Score::getLosses() const
{
	return losses;
}

void Score::reset()
{
	wins = 0;
	losses = 0;
}