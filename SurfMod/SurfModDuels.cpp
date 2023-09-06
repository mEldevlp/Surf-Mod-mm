#include "precompiled.h"

surfmod::CDuel g_SurfModDuel;

void surfmod::CDuel::JudgeMenuMain(int EntityIndex)
{
	std::string title = g_SurfModUtility.FormatString("^rJudge Panel\n\n");

	g_SurfModMenu[EntityIndex].Create(title, true, reinterpret_cast<void*>(this->JudgeMenuMain_Handle));

	g_SurfModMenu[EntityIndex].AddItem(this->m_is_now_duel ? -1 : 0,
		_TXT("Choose players %s", this->m_is_now_duel ? "^y(^rduel is going up^y)" : "\0"),
		this->m_is_now_duel);

	g_SurfModMenu[EntityIndex].AddItem(!this->m_is_now_duel ? -1 : 1,
		_TXT("Abort the duel %s", this->m_is_now_duel ? "\0" : "^y(^rduel isnt going up^y)"),
		!this->m_is_now_duel);

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
		g_SurfModUtility.SayText(Player->edict(), PRINT_TEAM_BLUE,		"^3Start duel!!!");
		g_SurfModUtility.SayText(Player->edict(), PRINT_TEAM_GREY,		"^3Start duel!!!");
		g_SurfModUtility.SayText(Player->edict(), PRINT_TEAM_DEFAULT,	"Start duel!!!");
		g_SurfModUtility.SayText(Player->edict(), PRINT_TEAM_RED,		"^3Start duel!!!");
		g_SurfModUtility.SayText(Player->edict(), PRINT_TEAM_DEFAULT,	"^4Start duel!!!");
		
		g_SurfModDuel.m_is_now_duel = true;
		g_SurfModDuel.m_pDuelists[0] = UTIL_PlayerByIndexSafe(g_SurfModDuel.m_iJudge_choice[EntityIndex][0]);
		g_SurfModDuel.m_pDuelists[1] = UTIL_PlayerByIndexSafe(g_SurfModDuel.m_iJudge_choice[EntityIndex][1]);

		g_SurfModDuel.StartDuel();

		return void();
	}

	int id_first_choice = g_SurfModDuel.m_iJudge_choice[EntityIndex][0];
	g_SurfModDuel.m_iJudge_choice[EntityIndex][id_first_choice == 0 ? 0 : 1] = Item.Info;
	g_SurfModDuel.JudgeChoosePlayerMenu(EntityIndex, false);
}

void surfmod::CDuel::Duel_Countdown(int timer)
{
	//auto Players = g_SurfModUtility.GetPlayers(false, true);

	if (g_SurfModDuel.m_iTimer != -1)
	{
		g_SurfModUtility.SayText(nullptr, PRINT_TEAM_GREY, _TXT("The match will start in ^3[^4%d^3] ^1seconds", g_SurfModDuel.m_iTimer--));
	}
	else
	{
		g_SurfModTask.Remove(TASK_MATCH_START);
		// Заменить на hudmessage
		//		[%name1%]		(blue)
		//			VS			(yellow or white)
		//		[%name2%]		(red)

		g_SurfModUtility.ServerCommand("sv_restart %d", 1);
		CSGameRules()->m_bGameStarted = true;

		g_SurfModUtility.SayText(nullptr, PRINT_TEAM_GREY, _TXT("^1[^4%s^1] ^3VS ^1[^4%s^1]",
																	STRING(Player_1->edict()->v.netname),
																	STRING(Player_2->edict()->v.netname)));
	}
}

void surfmod::CDuel::AbortDuel(int EntityIndex)
{
	auto judge = INDEXENT(EntityIndex);
	g_SurfModTask.Remove(TASK_MATCH_START);
	g_SurfModUtility.SayText(nullptr, PRINT_TEAM_RED, _TXT("^3Judge [^4%s^1] ^3has terminated the duel"), STRING(judge->v.netname));
	this->m_is_now_duel = false;
	this->m_has_duel_started = false;
	memset(reinterpret_cast<void*>(this->m_pDuelists), 0x0, sizeof(this->m_pDuelists));
}

void surfmod::CDuel::StartDuel()
{
	auto Players = g_SurfModUtility.GetPlayers(false, true);

	// everyone to spec
	for (auto const& Player : Players)
	{
		Player->CSPlayer()->JoinTeam(TeamName::SPECTATOR);
	}

	this->m_pDuelists[0]->CSPlayer()->JoinTeam(TeamName::CT);
	this->m_pDuelists[1]->CSPlayer()->JoinTeam(TeamName::TERRORIST);


	g_SurfModDuel.m_iTimer = 10;
	g_SurfModTask.Create(TASK_MATCH_START, 1.f, true, reinterpret_cast<void*>(g_SurfModDuel.Duel_Countdown), 10);
}