#pragma once

namespace SurfMod
{
	enum CONSOLE_COMMANDS {
		MENUSELECT,
		CHOOSETEAM,
		JUDGE_MENU
	};


	class Command
	{
	public:

		void ServerActivate();

		// On Client Command
		bool ClientCommand(CBasePlayer* Player, const char* pcmd, const char* parg1);
		CONSOLE_COMMANDS get_UserCommand(const char* pcmd);

	};
}

extern SurfMod::Command gSurfModCommand;