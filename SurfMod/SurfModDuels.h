#pragma once

namespace SurfMod {

	class Duel {
	public:

		// Main menu for Judge
		void JudgeMenuMain(int EntityIndex);
		static void JudgeMenuMain_Handle(int EntityIndex, P_MENU_ITEM Item);

		void JudgeChoosePlayerMenu(int EntityIndex);
		static void JudgeChoosePlayerMenu_Handle(int EntityIndex, P_MENU_ITEM Item);

		bool m_is_now_duel = false;
	private:
		int m_judge_choice[33][2] = { {0} , {0} };
	};
};

extern SurfMod::Duel gSurfModDuel;