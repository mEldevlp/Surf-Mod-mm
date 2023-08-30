#pragma once

namespace SurfMod
{
	class PlayerMenu
	{
	public:
		// Main Menu
		void MainMenu(int EntityIndex);
		static void MainMenu_Handle(int EntityIndex, P_MENU_ITEM Item);
	};
}

extern SurfMod::PlayerMenu gSurfModPlayerMenu;