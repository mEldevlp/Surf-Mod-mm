#pragma once

namespace SurfMod {

	class Duel {
	public:

		// Main menu for Judge
		void JudgeMenuMain(int EntityIndex);
		static void JudgeMenuMain_Handle(int EntityIndex, P_MENU_ITEM Item);

		void JudgeChoosePlayerMenu(int EntityIndex, bool isfirst_open);
		static void JudgeChoosePlayerMenu_Handle(int EntityIndex, P_MENU_ITEM Item);
		static void Duel_Countdown(int timer);

		bool is_now_duel = false;
		int iTimer = 0;
		std::map<std::string, CBasePlayer*> duelists;

	private:
		int m_judge_choice[33][2] = { {0} , {0} };
		CBasePlayer* m_Duelists[2];
	};
};

extern SurfMod::Duel gSurfModDuel;