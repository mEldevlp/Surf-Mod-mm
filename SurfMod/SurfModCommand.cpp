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
		case CONSOLE_COMMANDS::RESIGN:
		{
			g_SurfModDuel.PlayerResign(Player->entindex());
			break;
		}
		case CONSOLE_COMMANDS::BOX:
		{
			g_SurfModBox.BoxMainMenu(Player->entindex());
			break;
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
	else if (!Q_strcmp(pcmd, "resignduel"))
	{
		return CONSOLE_COMMANDS::RESIGN;
	}
	else if (!Q_strcmp(pcmd, "boxmenu"))
	{
		return CONSOLE_COMMANDS::BOX;
	}
	else
	{
		return CONSOLE_COMMANDS::UNDEFINDED;
	}
}
