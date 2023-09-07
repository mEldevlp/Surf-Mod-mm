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
			g_SurfModDuel.m_iJudge_choice[EntityIndex][0] = 0;
			g_SurfModDuel.m_iJudge_choice[EntityIndex][1] = 0;
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
		g_SurfModDuel.m_pDuel_info.is_now_duel = true;

		// CT init
		auto& ct = g_SurfModDuel.m_pDuel_info.player_ct;
		ct.base = UTIL_PlayerByIndexSafe(g_SurfModDuel.m_iJudge_choice[EntityIndex][0]);
		ct.name = STRING(ct.base->edict()->v.netname);
		ct.score = 0;
		
		// TER init
		auto& ter = g_SurfModDuel.m_pDuel_info.player_ter;
		ter.base = UTIL_PlayerByIndexSafe(g_SurfModDuel.m_iJudge_choice[EntityIndex][1]);
		ter.name = STRING(ter.base->edict()->v.netname);
		ter.score = 0;

		g_SurfModDuel.StartDuel();

		return void();
	}

	int id_first_choice = g_SurfModDuel.m_iJudge_choice[EntityIndex][0];
	g_SurfModDuel.m_iJudge_choice[EntityIndex][id_first_choice == 0 ? 0 : 1] = Item.Info;
	g_SurfModDuel.JudgeChoosePlayerMenu(EntityIndex, false);
}

void surfmod::CDuel::Duel_Countdown(int timer)
{
	if (g_SurfModDuel.m_iTimer != -1)
	{
		g_SurfModUtility.HudMessage(nullptr, g_SurfModUtility.HudParam(50, 200, 255, -1.0f, 0.4f, 1, 13.0f, 1.1f, 0.0f, 0.0f, 1),
			_TXT("The match will start in %d seconds", g_SurfModDuel.m_iTimer--));
	}
	else
	{
		g_SurfModTask.Remove(TASK_MATCH_START);

		CSGameRules()->m_bGameStarted = true;
		
		g_SurfModUtility.HudMessage(nullptr, g_SurfModUtility.HudParam(50, 200, 255, -1.f, 0.35f, 1, 13.f, 7.f, 0.f, 0.f, 2),
			_TXT("%s", g_SurfModDuel.m_pDuel_info.player_ct.name));

		g_SurfModUtility.HudMessage(nullptr, g_SurfModUtility.HudParam(255, 255, 255, -1.f, 0.37f, 1, 13.f, 7.f, 0.f, 0.f, 3),
			_TXT("VS"));

		g_SurfModUtility.HudMessage(nullptr, g_SurfModUtility.HudParam(220, 40, 40, -1.f, 0.39f, 1, 13.f, 7.f, 0.f, 0.f, 4),
			_TXT("%s", g_SurfModDuel.m_pDuel_info.player_ter.name));
		
		void* restarting = reinterpret_cast<void*>(g_SurfModDuel.Restarting);
		g_SurfModTask.Create(TASK_HUD_STOP, 5.f, false, restarting, 5);
		g_SurfModTask.Create(TASK_HUD_STOP + 10, 11.f, false, restarting, 3);
	}
}

void surfmod::CDuel::AbortDuel(int EntityIndex)
{
	auto judge = INDEXENT(EntityIndex);
	g_SurfModTask.Remove(TASK_MATCH_START);
	g_SurfModUtility.SayText(nullptr, PRINT_TEAM_RED, _TXT("^3Judge ^1[^4%s^1] ^3has terminated the duel"), STRING(judge->v.netname));
	// play sound here
	this->m_pDuel_info.is_now_duel = false;

	std::memset(reinterpret_cast<void*>(&m_pDuel_info), 0, sizeof(this->m_pDuel_info));
}

void surfmod::CDuel::StartDuel()
{
	auto Players = g_SurfModUtility.GetPlayers(false, true);

	// everyone to spec
	for (auto const& Player : Players)
	{
		Player->CSPlayer()->JoinTeam(TeamName::SPECTATOR);
	}

	this->m_pDuel_info.player_ct.base->CSPlayer()->JoinTeam(TeamName::CT);
	this->m_pDuel_info.player_ter.base->CSPlayer()->JoinTeam(TeamName::TERRORIST);

	g_SurfModDuel.m_iTimer = 10;
	g_SurfModTask.Create(TASK_MATCH_START, 1.f, true, reinterpret_cast<void*>(g_SurfModDuel.Duel_Countdown), 10);
}

void surfmod::CDuel::DuelWon(duel_player_t* player)
{
	auto print_type = (player->base->m_iTeam == TeamName::CT ? PRINT_TEAM_BLUE : PRINT_TEAM_RED);

	g_SurfModUtility.SayText(nullptr, print_type, _TXT("^3%s ^4won the duel!", player->name));
	g_SurfModUtility.SayText(nullptr, print_type, _TXT("^3%s ^4won the duel!", player->name));
	g_SurfModUtility.SayText(nullptr, print_type, _TXT("^3%s ^4won the duel!", player->name));
	g_SurfModUtility.SayText(nullptr, print_type, _TXT("^3%s ^4won the duel!", player->name));
	
	std::memset(reinterpret_cast<void*>(&m_pDuel_info), 0, sizeof(this->m_pDuel_info));
}

void surfmod::CDuel::Restarting(int time)
{
	g_SurfModUtility.ServerCommand("sv_restart %d", time);
}

// TODO: Чтобы нельзя было зайти за какую-либо команду пока идёт дуэль (в т.ч. и UNSIGNED игрокам)
// TODO: Режим ожидания, т.е. если игрок вылетел и т.д. дуэль не будет проиграна. (потом сделать квар)
// TODO: Возможность игроку сдаться /resign или т.п.
// TODO: Судья может принудительно засчитать поражение или отдать раунд игроку (сделать всё с подтверждением)
// TODO: Нормальный худ отсчёта начало дуэли (сейчас он вводит в заблуждение т.к. после начинаются рестарты и не понятно когда уже дуэль началась)