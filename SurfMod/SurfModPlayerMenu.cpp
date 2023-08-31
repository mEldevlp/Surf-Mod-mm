#include "precompiled.h"

SurfMod::PlayerMenu gSurfModPlayerMenu;

void SurfMod::PlayerMenu::MainMenu(int EntityIndex)
{
	// Menu
	gSurfModmenu[EntityIndex].Create("Surf Mod Main Menu", true, (void*)this->MainMenu_Handle);

	// Kick Option
	gSurfModmenu[EntityIndex].AddItem(0, "Kick Player");

	// Ban Player
	gSurfModmenu[EntityIndex].AddItem(1, "Ban Player");

	// Kill Player
	gSurfModmenu[EntityIndex].AddItem(2, "Slay Player");

	// Show menu 
	gSurfModmenu[EntityIndex].Show(EntityIndex);
}

void SurfMod::PlayerMenu::MainMenu_Handle(int EntityIndex, P_MENU_ITEM Item)
{
	auto Player = UTIL_PlayerByIndexSafe(EntityIndex);
	
	gSurfModUtility.SayText(Player->edict(), PRINT_TEAM_DEFAULT, "Menu has been handled by (^4%s^1)", Item.Text.c_str());
}
