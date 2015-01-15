#include "systems/score_system.h"
#include "Components.h"
#include "events.h"

ScoreSystem::ScoreSystem() {}

void ScoreSystem::update(Environment* _env)
{
	auto scoreEvt = _env->getEvents<ScoreEvent>();
	if(scoreEvt.size() > 0)
	{
		auto score = _env->get<Score>();
		if(_env->hasComponents<Score>(scoreEvt[0].winner)&&_env->hasComponents<Score>(scoreEvt[0].losser))
		{
			score[scoreEvt[0].losser].addLoss();
			score[scoreEvt[0].winner].addWin();
		}
	}
}
