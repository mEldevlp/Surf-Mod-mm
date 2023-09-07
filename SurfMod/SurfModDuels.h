#pragma once

namespace surfmod
{
	typedef struct duel_player_s
	{
		CBasePlayer* base;
		const char* name;
		int score;
	} duel_player_t;

	typedef struct duel_info_s
	{
		bool is_now_duel;
		duel_player_t player_ct;
		duel_player_t player_ter;

	} duel_info_t;

	class CDuel
	{
	public:

		void JudgeMenuMain(int EntityIndex);
		static void JudgeMenuMain_Handle(int EntityIndex, P_MENU_ITEM Item);

		void JudgeChoosePlayerMenu(int EntityIndex, bool isfirst_open);
		static void JudgeChoosePlayerMenu_Handle(int EntityIndex, P_MENU_ITEM Item);
		static void Duel_Countdown(int timer);
		void AbortDuel(int EntityIndex);
		void StartDuel();
		void DuelWon(duel_player_t* player);

		duel_info_t m_pDuel_info;
		int m_iTimer = 0;

	private:
		int m_iJudge_choice[33][2] = { {0} , {0} };
		static void Restarting(int time);
	};

}; /* namespace surfmod */

extern surfmod::CDuel g_SurfModDuel;