#pragma once

namespace surfmod
{
	class CPlayerMenu
	{
	public:
		// Main Menu
		void MainMenu(int EntityIndex);
		static void MainMenu_Handle(int EntityIndex, P_MENU_ITEM Item);
	};

}; /* namespace surfmod */

extern surfmod::CPlayerMenu g_SurfModPlayerMenu;