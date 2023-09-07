#include "precompiled.h"

surfmod::CPlayerMenu g_SurfModPlayerMenu;

void surfmod::CPlayerMenu::MainMenu(int EntityIndex)
{
	// Menu
	g_SurfModMenu[EntityIndex].Create("Surf Mod Main Menu\n", true, (void*)this->MainMenu_Handle);

	// Kick Option
	g_SurfModMenu[EntityIndex].AddItem(0, "Kick Player");

	// Ban Player
	g_SurfModMenu[EntityIndex].AddItem(1, "Ban Player");

	// Kill Player
	g_SurfModMenu[EntityIndex].AddItem(2, "Slay Player");

	// Show menu 
	g_SurfModMenu[EntityIndex].Show(EntityIndex);
}

void surfmod::CPlayerMenu::MainMenu_Handle(int EntityIndex, P_MENU_ITEM Item)
{
	auto Player = UTIL_PlayerByIndexSafe(EntityIndex);
	
	g_SurfModUtility.SayText(Player->edict(), PRINT_TEAM_DEFAULT, "Menu has been handled by (^4%s^1)", Item.Text.c_str());
}
