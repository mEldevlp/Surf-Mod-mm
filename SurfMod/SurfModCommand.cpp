#include "precompiled.h"

SurfMod::Command gSurfModCommand;

bool SurfMod::Command::ClientCommand(CBasePlayer* Player, const char* pcmd, const char* parg1)
{
	// IF is not null
	if (!pcmd)
	{
		return false;
	}

	// If string is not empty
	if (pcmd[0u] == '\0')
	{
		return false;
	}
	
	// If is menuselect command
	if (!Q_strcmp(pcmd, "menuselect"))
	{
		// If has arguments
		if (parg1)
		{
			// If native CS menu is not being displayed
			if (Player->m_iMenu == Menu_OFF)
			{
				// Handle menu
				if (gSurfModmenu[Player->entindex()].Handle(Player->entindex(), atoi(parg1)))
				{
					// Return result
					return true;
				}
			}
		}
	}
	else if (!Q_strcmp(pcmd, "chooseteam"))
	{
		// Сделать чтобы выводило русский
		//gSurfModUtility.SayText(Player->edict(), PRINT_TEAM_DEFAULT, "Вызвано меню");

		gSurfModPlayerMenu.MainMenu(Player->entindex());
		return true;
	}
	
	return false;
}
