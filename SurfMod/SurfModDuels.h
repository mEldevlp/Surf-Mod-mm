#pragma once

#define Player_1 g_SurfModDuel.m_pDuelists[0]
#define Player_2 g_SurfModDuel.m_pDuelists[1]

namespace surfmod {

	class CDuel {
	public:

		void JudgeMenuMain(int EntityIndex);
		static void JudgeMenuMain_Handle(int EntityIndex, P_MENU_ITEM Item);

		void JudgeChoosePlayerMenu(int EntityIndex, bool isfirst_open);
		static void JudgeChoosePlayerMenu_Handle(int EntityIndex, P_MENU_ITEM Item);
		static void Duel_Countdown(int timer);
		void AbortDuel(int EntityIndex);
		void StartDuel();

		bool m_is_now_duel = false;
		bool m_has_duel_started = false;
		int m_iTimer = 0;
		CBasePlayer* m_pDuelists[2];

	private:
		int m_iJudge_choice[33][2] = { {0} , {0} };

	};

}; /* namespace surfmod */

extern surfmod::CDuel g_SurfModDuel;