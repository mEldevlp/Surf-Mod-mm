#include "precompiled.h"

surfmod::CDuel g_SurfModDuel;

void surfmod::CDuel::JudgeMenuMain(int EntityIndex)
{
	std::string title = g_SurfModUtility.FormatString("^rJudge Panel\n\n");
	bool& isduel = this->m_pDuel_info.is_now_duel;

	g_SurfModMenu[EntityIndex].Create(title, true, reinterpret_cast<void*>(this->JudgeMenuMain_Handle));

	g_SurfModMenu[EntityIndex].AddItem(isduel ? -1 : 0, _TXT("Choose players %s", isduel ? "^y(^rduel is going up^y)" : "\0"), isduel);

	g_SurfModMenu[EntityIndex].AddItem(!isduel ? -1 : 1, _TXT("Abort the duel %s", isduel ? "\0" : "^y(^rduel isnt going up^y)"), !isduel);

	g_SurfModMenu[EntityIndex].Show(EntityIndex);
}

void surfmod::CDuel::JudgeMenuMain_Handle(int EntityIndex, P_MENU_ITEM Item)
{
	switch (Item.Info)
	{
		case 0:
		{
			std::memset(g_SurfModDuel.m_iJudge_choice[EntityIndex], 0, sizeof(g_SurfModDuel.m_iJudge_choice[EntityIndex]));

			g_SurfModDuel.JudgeChoosePlayerMenu(EntityIndex, true);
			break;
		}
		case 1:
		{
			g_SurfModDuel.AbortDuel(EntityIndex);
			break;
		}
		default:
		{
			g_SurfModUtility.SayText(INDEXENT(EntityIndex), PRINT_TEAM_RED, _TXT("^3You can not do this right now!"));
		}
	}
}

void surfmod::CDuel::JudgeChoosePlayerMenu(int EntityIndex, bool isfirst_open)
{
	std::string title = _TXT("^rJudge Panel\n^yChoose a pair of players:\n\n");

	// First choice
	if (g_SurfModDuel.m_iJudge_choice[EntityIndex][0] != 0)
	{
		int choiceid = g_SurfModDuel.m_iJudge_choice[EntityIndex][0];
		const char* nameVictim = STRING(INDEXENT(choiceid)->v.netname);
		title += _TXT("^w%s - ^rCT\n%c", nameVictim, g_SurfModDuel.m_iJudge_choice[EntityIndex][1] == 0 ? '\n' : '\0');
	}

	// Second choice
	if(g_SurfModDuel.m_iJudge_choice[EntityIndex][1] != 0)
	{
		int choiceid = g_SurfModDuel.m_iJudge_choice[EntityIndex][1];
		const char* nameVictim = STRING(INDEXENT(choiceid)->v.netname);

		title += g_SurfModUtility.FormatString("^w%s - ^rTT\n\n", nameVictim);
		g_SurfModMenu[EntityIndex].Create(title, true, reinterpret_cast<void*>(this->JudgeChoosePlayerMenu_Handle));
		g_SurfModMenu[EntityIndex].AddItem(0, "Start duel!");

		g_SurfModMenu[EntityIndex].Show(EntityIndex);
		return void();
	}

	// display players
	g_SurfModMenu[EntityIndex].Create(title, true, reinterpret_cast<void*>(this->JudgeChoosePlayerMenu_Handle));
	
	auto Players = g_SurfModUtility.GetPlayers(false, true);

	for (auto const& Player : Players)
	{
		if (Player->entindex() == g_SurfModDuel.m_iJudge_choice[EntityIndex][0])
		{
			g_SurfModMenu[EntityIndex].AddItem(-1, STRING(Player->edict()->v.netname), true);
		}
		else if (Player->m_iTeam == TeamName::UNASSIGNED)
		{
			g_SurfModMenu[EntityIndex].AddItem(-1, _TXT("%s ^w(^rnot in game^w)", STRING(Player->edict()->v.netname)), true);
		}
		else
		{
			g_SurfModMenu[EntityIndex].AddItem(Player->entindex(), STRING(Player->edict()->v.netname));
		}
	}

	g_SurfModMenu[EntityIndex].Show(EntityIndex);
}

void surfmod::CDuel::JudgeChoosePlayerMenu_Handle(int EntityIndex, P_MENU_ITEM Item)
{
	auto Player = UTIL_PlayerByIndexSafe(EntityIndex);

	if (Item.Info == -1)
	{
		g_SurfModUtility.SayText(Player->edict(), PRINT_TEAM_RED, "^3You can not choose him twice!");
		g_SurfModDuel.JudgeChoosePlayerMenu(EntityIndex, false);

		return void();
	}

	if (g_SurfModDuel.m_iJudge_choice[EntityIndex][0] != 0 && g_SurfModDuel.m_iJudge_choice[EntityIndex][1] != 0)
	{
		g_SurfModDuel.m_pDuel_info.player[Team::CT].init(g_SurfModDuel.m_iJudge_choice[EntityIndex][0], Team::CT);
		g_SurfModDuel.m_pDuel_info.player[Team::TER].init(g_SurfModDuel.m_iJudge_choice[EntityIndex][1], Team::TER);

		g_SurfModDuel.StartDuel();

		return void();
	}

	int id_first_choice = g_SurfModDuel.m_iJudge_choice[EntityIndex][0];
	g_SurfModDuel.m_iJudge_choice[EntityIndex][id_first_choice == 0 ? 0 : 1] = Item.Info;
	g_SurfModDuel.JudgeChoosePlayerMenu(EntityIndex, false);
}

void surfmod::CDuel::AbortDuel(int EntityIndex)
{
	auto judge = INDEXENT(EntityIndex);
	g_SurfModTask.Remove(TASK_MATCH_START);
	g_SurfModUtility.SayText(nullptr, PRINT_TEAM_RED, _TXT("^3Judge ^1[^4%s^1] ^3has terminated the duel."), STRING(judge->v.netname));
	g_SurfModUtility.ServerCommand("sv_restart %d", 5);
	CSGameRules()->m_bGameStarted = true;
	// play sound here

	this->State(DUEL_STATE::NOTHING);
}

void surfmod::CDuel::StartDuel()
{
	auto Players = g_SurfModUtility.GetPlayers(false, true);

	// everyone to spec
	for (auto const& Player : Players)
	{
		Player->CSPlayer()->JoinTeam(TeamName::SPECTATOR);
	}

	this->m_pDuel_info.player[Team::CT].base->CSPlayer()->JoinTeam(TeamName::CT);
	this->m_pDuel_info.player[Team::TER].base->CSPlayer()->JoinTeam(TeamName::TERRORIST);

	this->State(DUEL_STATE::PREPARING);
}

void surfmod::CDuel::DuelWon(duel_player_t* player)
{
	this->m_pDuel_info.player[Team::WINNER] = *player;

	this->State(DUEL_STATE::WINING);
}

void surfmod::CDuel::DuelPause(duel_player_t* player)
{
	this->m_pDuel_info.state = DUEL_STATE::PAUSE;

	g_SurfModUtility.SayText(nullptr, player->print_team, _TXT("^3%s ^1has been ^4disconnected^1! Duel on pause.", player->name));
	g_SurfModUtility.SayText(nullptr, PRINT_TEAM_GREY, _TXT("^3Waiting for the ^4judge ^3decision..."));
	// Открыть судье меню о присуждении поражения или ожидания коннекта игрока (придётся запоминать судью когда он запускает дуэль)
}

void surfmod::CDuel::State(DUEL_STATE state)
{
	switch (this->m_pDuel_info.state = state)
	{
		case DUEL_STATE::NOTHING:
		{
			this->m_pDuel_info.player.clear();
			this->m_pDuel_info.is_now_duel = false;
			break;
		}

		case DUEL_STATE::PREPARING:
		{
			this->StartRestarting();
			break;
		}

		case DUEL_STATE::PAUSE:
		{
			break;
		}

		case DUEL_STATE::UNPAUSE:
		{
			g_SurfModUtility.SayText(nullptr, PRINT_TEAM_DEFAULT, _TXT("^3Get ready for restart."));
			g_SurfModUtility.SayText(nullptr, PRINT_TEAM_DEFAULT, _TXT("^3Get ready for restart."));
			g_SurfModUtility.SayText(nullptr, PRINT_TEAM_DEFAULT, _TXT("^3Get ready for restart."));
			g_SurfModUtility.SayText(nullptr, PRINT_TEAM_DEFAULT, _TXT("^3Get ready for restart."));
			g_SurfModUtility.SayText(nullptr, PRINT_TEAM_DEFAULT, _TXT("^3Get ready for restart."));

			this->m_pDuel_info.state = DUEL_STATE::GOING;
			g_SurfModUtility.ServerCommand("sv_restart %d", 7);
			CSGameRules()->m_bGameStarted = true;
			break;
		}

		case DUEL_STATE::GOING:
		{
			g_SurfModUtility.HudMessage(nullptr, g_SurfModUtility.HudParam(50, 200, 255, -1.0, 0.35, 1, 13.0, 10.0, 0.0f, 0.0f, 1),
				_TXT("%s", g_SurfModDuel.m_pDuel_info.player[Team::CT].name));

			g_SurfModUtility.HudMessage(nullptr, g_SurfModUtility.HudParam(255, 255, 255, -1.0, 0.38, 1, 13.0, 10.0, 0.0f, 0.0f, 2),
				_TXT("---------- VS ----------"));

			g_SurfModUtility.HudMessage(nullptr, g_SurfModUtility.HudParam(220, 40, 40, -1.0, 0.41, 1, 13.0, 10.0, 0.0f, 0.0f, 3),
				_TXT("%s", g_SurfModDuel.m_pDuel_info.player[Team::TER].name));

			this->m_pDuel_info.player[Team::CT].score = 0;
			this->m_pDuel_info.player[Team::TER].score = 0;
			this->m_pDuel_info.is_now_duel = true;
			break;
		}

		case DUEL_STATE::WINING:
		{
			auto print_team = this->m_pDuel_info.player[Team::WINNER].print_team;
			auto winner_name = this->m_pDuel_info.player[Team::WINNER].name;

			g_SurfModUtility.SayText(nullptr, print_team, _TXT("^3%s ^4won the duel!", winner_name));
			g_SurfModUtility.SayText(nullptr, print_team, _TXT("^3%s ^4won the duel!", winner_name));
			g_SurfModUtility.SayText(nullptr, print_team, _TXT("^3%s ^4won the duel!", winner_name));
			g_SurfModUtility.SayText(nullptr, print_team, _TXT("^3%s ^4won the duel!", winner_name));

			this->State(DUEL_STATE::NOTHING);
			break;
		}

		default: break;
	}
}

void surfmod::CDuel::ClientDisconnect(edict_t* player)
{
	if (this->m_pDuel_info.is_now_duel)
	{
		auto steamid = g_engfuncs.pfnGetPlayerAuthId(player);

		if (!Q_strcmp(steamid, this->m_pDuel_info.player[Team::CT].auth_id.c_str()))
		{
			g_SurfModDuel.DuelPause(&this->m_pDuel_info.player[Team::CT]);
		}
		else if (!Q_strcmp(steamid, this->m_pDuel_info.player[Team::TER].auth_id.c_str()))
		{
			g_SurfModDuel.DuelPause(&this->m_pDuel_info.player[Team::TER]);
		}
	}
}

void surfmod::CDuel::DuelUnpause(int time)
{
	g_SurfModDuel.State(DUEL_STATE::UNPAUSE);
}

void surfmod::CDuel::DuelistComeback(int player_id, Team player_team)
{
	int score = g_SurfModDuel.m_pDuel_info.player[player_team].score;
	g_SurfModDuel.m_pDuel_info.player[player_team].init(player_id, player_team);
	g_SurfModDuel.m_pDuel_info.player[player_team].base->CSPlayer()->JoinTeam(static_cast<TeamName>(player_team));
	g_SurfModDuel.m_pDuel_info.player[player_team].score = score;

	g_SurfModUtility.SayText(nullptr, g_SurfModDuel.m_pDuel_info.player[player_team].print_team, "^3%s ^1is back! ^4Continue the duel.", g_SurfModDuel.m_pDuel_info.player[player_team].name);

	g_SurfModTask.Create(TASK_DUEL_UNPAUSE, 5.f, false, reinterpret_cast<void*>(g_SurfModDuel.DuelUnpause), 5);
}

void surfmod::CDuel::StartRestarting()
{
	this->m_iRestart_times = 3;

	g_SurfModUtility.ServerCommand("sv_restart %d", this->m_iRestart_times);
	
	CSGameRules()->m_bGameStarted = true;

	g_SurfModUtility.SayText(nullptr, PRINT_TEAM_DEFAULT, _TXT("^3Initialization of the duel. ^1Get ready for restart."));
	g_SurfModUtility.SayText(nullptr, PRINT_TEAM_DEFAULT, _TXT("^3Initialization of the duel. ^1Get ready for restart."));
	g_SurfModUtility.SayText(nullptr, PRINT_TEAM_DEFAULT, _TXT("^3Initialization of the duel. ^1Get ready for restart."));
	g_SurfModUtility.SayText(nullptr, PRINT_TEAM_DEFAULT, _TXT("^3Initialization of the duel. ^1Get ready for restart."));
	g_SurfModUtility.SayText(nullptr, PRINT_TEAM_DEFAULT, _TXT("^3Initialization of the duel. ^1Get ready for restart."));
}

void surfmod::CDuel::RoundRestart()
{
	if (this->m_iRestart_times > 0)
	{
		g_SurfModUtility.ServerCommand("sv_restart %d", --this->m_iRestart_times);

		if (this->m_iRestart_times == 0)
		{
			this->State(DUEL_STATE::GOING);
		}
	}
}

// TODO: Возможность игроку сдаться /resign или т.п.
// TODO: Судья может принудительно засчитать поражение или отдать раунд игроку (сделать всё с подтверждением)
// если вышли оба игрока то AbortDuel