#include "precompiled.h"

SurfMod::Duel gSurfModDuel;

void SurfMod::Duel::JudgeMenuMain(int EntityIndex)
{
	std::string title = gSurfModUtility.FormatString("\\rJudge Panel \\yChoose players:");
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
			gSurfModDuel.JudgeChoosePlayerMenu(EntityIndex);
			break;
		}
		default: break;
	}
}

void SurfMod::Duel::JudgeChoosePlayerMenu(int EntityIndex)
{
	std::string title = gSurfModUtility.FormatString("^rJudge Panel^n ^yChoose players:^n");

	//gSurfModmenu[EntityIndex].Create(title, true, reinterpret_cast<void*>(this->JudgeChoosePlayerMenu_Handle));
	

	std::string Menu_Text = gSurfModUtility.FormatString("^rJudge Panel^n ^yChoose players:^n");



	std::string Menu_Text = gSurfModUtility.FormatString("^r[^w1^r] asdsdsds");

	/*auto Players = gSurfModUtility.GetPlayers(false, true);

	for (auto const& Player : Players)
	{
		gSurfModmenu[EntityIndex].AddItem(Player->entindex(), STRING(Player->edict()->v.netname));
	}

	gSurfModmenu[EntityIndex].Show(EntityIndex, iKeys);*/
}

void SurfMod::Duel::JudgeChoosePlayerMenu_Handle(int EntityIndex, P_MENU_ITEM Item)
{
	auto Player = UTIL_PlayerByIndexSafe(EntityIndex);
	gSurfModUtility.SayText(Player->edict(), PRINT_TEAM_DEFAULT, "(^4%s^1) was chooses.", Item.Text.c_str());
}