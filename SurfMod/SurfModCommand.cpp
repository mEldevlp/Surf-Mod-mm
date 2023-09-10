#include "precompiled.h"

surfmod::CCommand g_SurfModCommand;

void surfmod::CCommand::ServerActivate()
{
	return void();
}

bool surfmod::CCommand::ClientCommand(CBasePlayer* Player, const char* pcmd, const char* parg1)
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
				if (g_SurfModMenu[Player->entindex()].Handle(Player->entindex(), atoi(parg1)))
				{
					return true;
				}
			}

			break;
		}
		case CONSOLE_COMMANDS::SURF_DUEL:
		{
			g_SurfModDuel.JudgeMenuMain(Player->entindex());
			return true;
		}
	}

	return false;
}

surfmod::CONSOLE_COMMANDS surfmod::CCommand::get_UserCommand(const char* pcmd)
{
	if (!Q_strcmp(pcmd, "menuselect"))
	{
		return CONSOLE_COMMANDS::MENUSELECT;
	}
	else if (!Q_strcmp(pcmd, "surfduel"))
	{
		return CONSOLE_COMMANDS::SURF_DUEL;
	}
}

