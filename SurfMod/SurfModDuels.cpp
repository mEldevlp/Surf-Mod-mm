#include "precompiled.h"

SurfMod::Duel gSurfModDuel;

void SurfMod::Duel::JudgeMenuMain(int EntityIndex)
{
	std::string title = gSurfModUtility.FormatString("^rJudge Panel | ^yAccess level ^w[^rHIGH^w]\n\n");
	gSurfModmenu[EntityIndex].Create(title, true, reinterpret_cast<void*>(this->JudgeMenuMain_Handle));

	gSurfModmenu[EntityIndex].AddItem(0, "Choose players");

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
		default: break;
	}
}

void SurfMod::Duel::JudgeChoosePlayerMenu(int EntityIndex, bool isfirst_open)
{
	std::string title = gSurfModUtility.FormatString("^rJudge Panel\n^yChoose a pair of players:\n\n");

	if (gSurfModDuel.m_judge_choice[EntityIndex][0] != 0)
	{
		int choiceid = gSurfModDuel.m_judge_choice[EntityIndex][0];
		title += gSurfModUtility.FormatString("^yChosen:\n");
		const char* nameVictim = STRING(UTIL_PlayerByIndexSafe(choiceid)->edict()->v.netname);
		title += gSurfModUtility.FormatString("^w%s - ^rCT\n%c", nameVictim, gSurfModDuel.m_judge_choice[EntityIndex][1] == 0 ? '\n' : '\0');
	}

	if(gSurfModDuel.m_judge_choice[EntityIndex][1] != 0)
	{
		int choiceid = gSurfModDuel.m_judge_choice[EntityIndex][1];
		const char* nameVictim = STRING(UTIL_PlayerByIndexSafe(choiceid)->edict()->v.netname);

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
		gSurfModmenu[EntityIndex].AddItem(Player->entindex(), STRING(Player->edict()->v.netname));
	}

	gSurfModmenu[EntityIndex].Show(EntityIndex);
}

void SurfMod::Duel::JudgeChoosePlayerMenu_Handle(int EntityIndex, P_MENU_ITEM Item)
{
	auto Player = UTIL_PlayerByIndexSafe(EntityIndex);

	if (gSurfModDuel.m_judge_choice[EntityIndex][0] != 0 && gSurfModDuel.m_judge_choice[EntityIndex][1] != 0)
	{
		gSurfModUtility.SayText(Player->edict(), PRINT_TEAM_DEFAULT, "start duel");
		gSurfModUtility.SayText(Player->edict(), PRINT_TEAM_DEFAULT, "start duel");
		gSurfModUtility.SayText(Player->edict(), PRINT_TEAM_DEFAULT, "start duel");
		gSurfModUtility.SayText(Player->edict(), PRINT_TEAM_DEFAULT, "start duel");
		return void();
	}

	if (gSurfModDuel.m_judge_choice[EntityIndex][0] == 0)
	{
		gSurfModDuel.m_judge_choice[EntityIndex][0] = Item.Info;
	}
	else//(gSurfModDuel.m_judge_choice[EntityIndex][0] != 0)
	{
		gSurfModDuel.m_judge_choice[EntityIndex][1] = Item.Info;
	}

	gSurfModDuel.JudgeChoosePlayerMenu(EntityIndex, false);
}