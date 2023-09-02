#include "precompiled.h"

// DLL PRE Functions Table
DLL_FUNCTIONS gDLL_FunctionTable_Pre;

// DLL POST Functions Table
DLL_FUNCTIONS gDLL_FunctionTable_Post;

#pragma region DLL_PRE
C_DLLEXPORT int GetEntityAPI2(DLL_FUNCTIONS* pFunctionTable, int* interfaceVersion)
{
	memset(&gDLL_FunctionTable_Pre, 0, sizeof DLL_FUNCTIONS);
	
	gDLL_FunctionTable_Pre.pfnCmdStart = DLL_PRE_CmdStart;
	gDLL_FunctionTable_Pre.pfnCmdEnd = DLL_PRE_CmdEnd;


	memcpy(pFunctionTable, &gDLL_FunctionTable_Pre, sizeof(DLL_FUNCTIONS));

	return 1;
}

void DLL_PRE_CmdStart(const edict_t* player, const struct usercmd_s* cmd, unsigned int random_seed)
{
	if (!FNullEnt(player))
	{
		auto Player = UTIL_PlayerByIndexSafe(ENTINDEX(player));
		gSurfModFix.CmdStart(Player->edict(), cmd, random_seed);
	}

	RETURN_META(MRES_IGNORED);
}

void DLL_PRE_CmdEnd(const edict_t* player)
{
	if (!FNullEnt(player))
	{
		auto Player = UTIL_PlayerByIndexSafe(ENTINDEX(player));
		gSurfModFix.CmdEnd(Player->edict());
	}
	
	RETURN_META(MRES_IGNORED);
}
#pragma endregion

#pragma region DLL_POST
C_DLLEXPORT int GetEntityAPI2_Post(DLL_FUNCTIONS* pFunctionTable, int* interfaceVersion)
{
	memset(&gDLL_FunctionTable_Post, 0, sizeof(DLL_FUNCTIONS));

	gDLL_FunctionTable_Post.pfnServerActivate = DLL_POST_ServerActivate;

	memcpy(pFunctionTable, &gDLL_FunctionTable_Post, sizeof(DLL_FUNCTIONS));

	return 1;
}

void DLL_POST_ServerActivate(edict_t* pEdictList, int edictCount, int clientMax)
{
	gSurfModCommand.ServerActivate();

	RETURN_META(MRES_IGNORED);
}
#pragma endregion