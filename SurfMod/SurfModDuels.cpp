#include "precompiled.h"

SurfMod::Duel gSurfModDuel;

void SurfMod::Duel::JudgeMenuMain(int EntityIndex)
{
	std::string title = gSurfModUtility.FormatString("^rJudge Panel\n\n");
	gSurfModmenu[EntityIndex].Create(title, true, reinterpret_cast<void*>(this->JudgeMenuMain_Handle));

	gSurfModmenu[EntityIndex].AddItem(this->is_now_duel ? -1 : 0, _TXT("Choose players %s", this->is_now_duel? "^y(^rduel is going up^y)" : "\0"), this->is_now_duel);
	gSurfModmenu[EntityIndex].AddItem(!this->is_now_duel ? -1 : 1, _TXT("Abort the duel %s", this->is_now_duel? "\0" : "^y(^rduel isnt going up^y)"), !this->is_now_duel);

	gSurfModmenu[EntityIndex].Show(EntityIndex);
}

void SurfMod::Duel::JudgeMenuMain_Handle(int EntityIndex, P_MENU_ITEM Item)
{
	switch (Item.Info)
	{
		case 0:
		{
			gSurfModDuel.m_judge_choice[EntityIndex][0] = 0;
			gSurfModDuel.m_judge_choice[EntityIndex][1] = 0;
			gSurfModDuel.JudgeChoosePlayerMenu(EntityIndex, true);
			break;
		}
		case 1:
		{
			gSurfModUtility.SayText(INDEXENT(EntityIndex), PRINT_TEAM_RED, _TXT("^3abort"));
			//gSurfModDuel.AbortDuel();
			break;
		}
		default:
		{
			gSurfModUtility.SayText(INDEXENT(EntityIndex), PRINT_TEAM_RED, _TXT("^3You can not do this right now!"));
		}
	}
}

void SurfMod::Duel::JudgeChoosePlayerMenu(int EntityIndex, bool isfirst_open)
{
	std::string title = _TXT("^rJudge Panel\n^yChoose a pair of players:\n\n");

	// First choice
	if (gSurfModDuel.m_judge_choice[EntityIndex][0] != 0)
	{
		int choiceid = gSurfModDuel.m_judge_choice[EntityIndex][0];
		const char* nameVictim = STRING(INDEXENT(choiceid)->v.netname);
		title += _TXT("^w%s - ^rCT\n%c", nameVictim, gSurfModDuel.m_judge_choice[EntityIndex][1] == 0 ? '\n' : '\0');
	}

	// Second choice
	if(gSurfModDuel.m_judge_choice[EntityIndex][1] != 0)
	{
		int choiceid = gSurfModDuel.m_judge_choice[EntityIndex][1];
		const char* nameVictim = STRING(INDEXENT(choiceid)->v.netname);

		title += gSurfModUtility.FormatString("^w%s - ^rTT\n\n", nameVictim);
		gSurfModmenu[EntityIndex].Create(title, true, reinterpret_cast<void*>(this->JudgeChoosePlayerMenu_Handle));
		gSurfModmenu[EntityIndex].AddItem(0, "Start duel!");

		gSurfModmenu[EntityIndex].Show(EntityIndex);
		return void();
	}

	// display players
	gSurfModmenu[EntityIndex].Create(title, true, reinterpret_cast<void*>(this->JudgeChoosePlayerMenu_Handle));
	
	auto Players = gSurfModUtility.GetPlayers(false, true);

	for (auto const& Player : Players)
	{
		if (Player->entindex() == gSurfModDuel.m_judge_choice[EntityIndex][0])
		{
			gSurfModmenu[EntityIndex].AddItem(-1, STRING(Player->edict()->v.netname), true);
		}
		else if (Player->m_iTeam == TeamName::UNASSIGNED)
		{
			gSurfModmenu[EntityIndex].AddItem(-1, _TXT("%s ^w(^rnot in game^w)", STRING(Player->edict()->v.netname)), true);
		}
		else
		{
			gSurfModmenu[EntityIndex].AddItem(Player->entindex(), STRING(Player->edict()->v.netname));
		}
	}

	gSurfModmenu[EntityIndex].Show(EntityIndex);
}

void SurfMod::Duel::JudgeChoosePlayerMenu_Handle(int EntityIndex, P_MENU_ITEM Item)
{
	auto Player = UTIL_PlayerByIndexSafe(EntityIndex);

	if (Item.Info == -1)
	{
		gSurfModUtility.SayText(Player->edict(), PRINT_TEAM_RED, "^3You can not choose him twice!");
		gSurfModDuel.JudgeChoosePlayerMenu(EntityIndex, false);
		return void();
	}

	if (gSurfModDuel.m_judge_choice[EntityIndex][0] != 0 && gSurfModDuel.m_judge_choice[EntityIndex][1] != 0)
	{
		gSurfModUtility.SayText(Player->edict(), PRINT_TEAM_BLUE,		"^3Start duel!!!");
		gSurfModUtility.SayText(Player->edict(), PRINT_TEAM_GREY,		"^3Start duel!!!");
		gSurfModUtility.SayText(Player->edict(), PRINT_TEAM_DEFAULT,	"Start duel!!!");
		gSurfModUtility.SayText(Player->edict(), PRINT_TEAM_RED,		"^3Start duel!!!");
		gSurfModUtility.SayText(Player->edict(), PRINT_TEAM_DEFAULT,	"^4Start duel!!!");
		
		gSurfModDuel.is_now_duel = true;
		gSurfModDuel.m_Duelists[0] = UTIL_PlayerByIndexSafe(gSurfModDuel.m_judge_choice[EntityIndex][0]);
		gSurfModDuel.m_Duelists[1] = UTIL_PlayerByIndexSafe(gSurfModDuel.m_judge_choice[EntityIndex][1]);

		gSurfModDuel.iTimer = 10;
		gSurfModTask.Create(TASK_MATCH_START, 1.f, true, reinterpret_cast<void*>(gSurfModDuel.Duel_Countdown), 10);

		return void();
	}

	int id_first_choice = gSurfModDuel.m_judge_choice[EntityIndex][0];
	gSurfModDuel.m_judge_choice[EntityIndex][id_first_choice == 0 ? 0 : 1] = Item.Info;
	gSurfModDuel.JudgeChoosePlayerMenu(EntityIndex, false);
}

void SurfMod::Duel::Duel_Countdown(int timer)
{
	auto Players = gSurfModUtility.GetPlayers(false, true);

	if (gSurfModDuel.iTimer != -1)
	{
		for (auto const& Player : Players)
		{
			gSurfModUtility.SayText(Player->edict(), PRINT_TEAM_GREY, _TXT("The match will start in ^3[^4%d^3] ^1seconds", gSurfModDuel.iTimer));
		}
		--gSurfModDuel.iTimer;
	}
	else
	{
		gSurfModTask.Remove(TASK_MATCH_START);
		for (auto const& Player : Players)
		{
			// Заменить на hudmessage
			//		[%name1%]		(blue)
			//			VS			(yellow or white)
			//		[%name2%]		(red)
			gSurfModUtility.SayText(Player->edict(), PRINT_TEAM_GREY, _TXT("^1[^4%s^1] ^3VS ^1[^4%s^1]",
																		STRING(gSurfModDuel.m_Duelists[0]->edict()->v.netname),
																		STRING(gSurfModDuel.m_Duelists[1]->edict()->v.netname)));
		}
	}
}
