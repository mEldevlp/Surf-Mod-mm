#pragma once

namespace surfmod {

	enum CONSOLE_COMMANDS {
		MENUSELECT,
		CHOOSETEAM,
		SURF_DUEL
	};

	class CCommand
	{
	public:

		void ServerActivate();

		// On Client Command
		bool ClientCommand(CBasePlayer* Player, const char* pcmd, const char* parg1);
		CONSOLE_COMMANDS get_UserCommand(const char* pcmd);
	};

}; /* namespace surfmod */

extern surfmod::CCommand g_SurfModCommand;