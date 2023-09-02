#include "precompiled.h"

SurfMod::Command gSurfModCommand;

void SurfMod::Command::ServerActivate()
{
	return void();
}

bool SurfMod::Command::ClientCommand(CBasePlayer* Player, const char* pcmd, const char* parg1)
{
	if (!pcmd)
	{
		return false;
	}

	if (pcmd[0u] == '\0')
	{
		return false;
	}

	switch (this->get_UserCommand(pcmd))
	{
		case CONSOLE_COMMANDS::MENUSELECT:
		{
			if (parg1 && Player->m_iMenu == Menu_OFF)
			{
				if (gSurfModmenu[Player->entindex()].Handle(Player->entindex(), atoi(parg1)))
				{
					return true;
				}
			}

			break;
		}
		case CONSOLE_COMMANDS::CHOOSETEAM:
		{
			gSurfModPlayerMenu.MainMenu(Player->entindex());
			return true;
		}
		case CONSOLE_COMMANDS::JUDGE_MENU:
		{
			gSurfModDuel.JudgeMenuMain(Player->entindex());
			return true;
		}
	}

	return false;
}

SurfMod::CONSOLE_COMMANDS SurfMod::Command::get_UserCommand(const char* pcmd)
{
	if (!Q_strcmp(pcmd, "menuselect"))
	{
		return CONSOLE_COMMANDS::MENUSELECT;
	}
	else if (!Q_strcmp(pcmd, "chooseteam"))
	{
		return CONSOLE_COMMANDS::CHOOSETEAM;
	}
	else if (!Q_strcmp(pcmd, "judge"))
	{
		return CONSOLE_COMMANDS::JUDGE_MENU;
	}
}

