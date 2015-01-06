#pragma once
#include "core/environment.h"

class HUD_Manager
{
	friend class HUDSystem;

	public:
		HUD_Manager();
		virtual ~HUD_Manager();

		void createHUD(Environment* _env);

	private:
		std::vector<int> IDs;

		void createAmmoHUD(Environment* _env);
		void createHealthHUD(Environment* _env);
		void createLivesHUD(Environment* _env);
};

