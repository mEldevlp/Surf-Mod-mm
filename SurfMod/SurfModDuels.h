#pragma once

namespace surfmod
{
	enum DUEL_STATE {
		NOTHING,
		PREPARING,
		PAUSE,
		UNPAUSE,
		GOING,
		WINING
	};

	enum Team {
		TER = 1,
		CT,
		WINNER = 10
	};

	struct duel_player_t
	{
		CBasePlayer* base;
		const char* name;
		TeamName team;
		int id;
		std::string auth_id;
		int score;
		int print_team;

	public:
		void init(int player_id, Team player_team)
		{
			this->base = UTIL_PlayerByIndexSafe(player_id);
			this->name = STRING(this->base->edict()->v.netname);
			this->team = static_cast<TeamName>(player_team);
			this->id = player_id;
			this->auth_id = g_engfuncs.pfnGetPlayerAuthId(this->base->edict());
			this->score = 0;
			this->print_team = player_team == Team::CT ? PRINT_TEAM_BLUE : PRINT_TEAM_RED;
		}
	};

	struct duel_info_t
	{
		bool is_now_duel = false;
		DUEL_STATE state = DUEL_STATE::NOTHING;
		std::map<Team, duel_player_t> player;
		std::vector<std::array<int, 2>> round;
	};

	class CDuel
	{
	public:
		void JudgeMenuMain(int EntityIndex);
		static void JudgeMenuMain_Handle(int EntityIndex, P_MENU_ITEM Item);

		void JudgeChoosePlayerMenu(int EntityIndex);
		static void JudgeChoosePlayerMenu_Handle(int EntityIndex, P_MENU_ITEM Item);

		void BackupRoundMenu(int EntityIndex);
		static void BackupRoundMenu_Handle(int EntityIndex, P_MENU_ITEM Item);

		void AbortDuel(int EntityIndex);
		void StartDuel();
		void DuelWon(duel_player_t* player);
		void DuelPause(duel_player_t* player);

		static void DuelUnpause(int time);

		void DuelistComeback(int player_id, Team player_team);

		void ClientDisconnect(edict_t* player);

		void State(DUEL_STATE state);

		void StartRestarting();
		void RoundRestart();
		void UpdateGameInfo(bool isAddRound = true);

		duel_info_t m_pDuel_info;
		int m_iRoundsCount = 0;

	private:
		bool is_duelist_disconnected(const char* steamid, Team* player_team);

		int m_iJudge_choice[33][2] = { {0} , {0} };
		int m_iJudge_id[33];
		int m_iRestart_times;
	};

}; /* namespace surfmod */

extern surfmod::CDuel g_SurfModDuel;