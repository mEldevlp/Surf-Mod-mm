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

	// Change Player Team
	gSurfModmenu[EntityIndex].AddItem(3, "Team Player");

	// Change map
	gSurfModmenu[EntityIndex].AddItem(4, "Change Map");

	// Control Match BOT
	gSurfModmenu[EntityIndex].AddItem(5, "Match Control");

	// Send global chat message
	gSurfModmenu[EntityIndex].AddItem(6, "Send Message");

	// Send server rcon command
	gSurfModmenu[EntityIndex].AddItem(7, "Send Command");

	// Swap teams instantly
	gSurfModmenu[EntityIndex].AddItem(8, "Swap Teams");

	// Player List
	gSurfModmenu[EntityIndex].AddItem(9, "Player List");

	// Console Variables Menu
	gSurfModmenu[EntityIndex].AddItem(10, "Cvars Menu");

	// Console Variables Menu
	gSurfModmenu[EntityIndex].AddItem(11, "Cvars Menu");

	// Console Variables Menu
	gSurfModmenu[EntityIndex].AddItem(12, "Cvarss Menu");

	// Console Variables Menu
	gSurfModmenu[EntityIndex].AddItem(13, "Cvarsss Menu");

	// Console Variables Menu
	gSurfModmenu[EntityIndex].AddItem(14, "Cvarssssss");

	// Console Variables Menu
	gSurfModmenu[EntityIndex].AddItem(15, "Cvarsssss55555");

	// Show menu 
	gSurfModmenu[EntityIndex].Show(EntityIndex);
}

void SurfMod::PlayerMenu::MainMenu_Handle(int EntityIndex, P_MENU_ITEM Item)
{
	auto Player = UTIL_PlayerByIndexSafe(EntityIndex);
	
	gSurfModUtility.SayText(Player->edict(), PRINT_TEAM_DEFAULT, "Menu has been handled (^4%s^1)", Item.Text.c_str());
}
