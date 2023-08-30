#pragma once

namespace SurfMod
{
	class Command
	{
	public:
		// On Client Command
		bool ClientCommand(CBasePlayer* Player, const char* pcmd, const char* parg1);
	};
}

extern SurfMod::Command gSurfModCommand;