#pragma once

namespace surfmod
{
	enum DUEL_STATE {
		NOTHING,
		PREPARING,
		PAUSE,
		GOING,
		WINING
	};

	enum Team {
		TER = 1,
		CT,
		WINNER = 10
	};

	typedef struct duel_player_s
	{
		CBasePlayer* base;
		const char* name;
		TeamName team;
		int id;
		int score;
		int print_team;

	public:
		void init(int player_id, Team player_team)
		{
			this->base = UTIL_PlayerByIndexSafe(player_id);
			this->name = STRING(this->base->edict()->v.netname);
			this->team = static_cast<TeamName>(player_team);
			this->id = player_id;
			this->score = 0;
			this->print_team = player_team == Team::CT ? PRINT_TEAM_BLUE : PRINT_TEAM_RED;
		}
	} duel_player_t;

	typedef struct duel_info_s
	{
		bool is_now_duel = false;
		DUEL_STATE state = DUEL_STATE::NOTHING;
		std::map<Team, duel_player_t> player;

	} duel_info_t;

	class CDuel
	{
	public:

		void JudgeMenuMain(int EntityIndex);
		static void JudgeMenuMain_Handle(int EntityIndex, P_MENU_ITEM Item);

		void JudgeChoosePlayerMenu(int EntityIndex, bool isfirst_open);
		static void JudgeChoosePlayerMenu_Handle(int EntityIndex, P_MENU_ITEM Item);
		void AbortDuel(int EntityIndex);
		void StartDuel();
		void DuelWon(duel_player_t* player);
		void DuelPause(duel_player_t* player);

		void State(DUEL_STATE state);

		duel_info_t m_pDuel_info;

		void StartRestarting();
		void RoundRestart();

	private:
		int m_iJudge_choice[33][2] = { {0} , {0} };
		int m_iRestart_times;
	};

}; /* namespace surfmod */

extern surfmod::CDuel g_SurfModDuel;