#pragma once
#include "core/environment.h"

class Score: public Component<Score>
{
	public:
		Score(unsigned _wins, unsigned _losses);

		void addWin();
		void addLoss();
		void reset();
		unsigned getWins() const;
		unsigned getLosses() const;
		

	private: 
		unsigned wins, losses;
};

