#include "precompiled.h"

// DLL PRE Functions Table
DLL_FUNCTIONS gDLL_FunctionTable_Pre;

// DLL POST Functions Table
DLL_FUNCTIONS gDLL_FunctionTable_Post;

#pragma region DLL_PRE
C_DLLEXPORT int GetEntityAPI2(DLL_FUNCTIONS* pFunctionTable, int* interfaceVersion)
{
	memset(&gDLL_FunctionTable_Pre, 0, sizeof(DLL_FUNCTIONS));
	
	gDLL_FunctionTable_Pre.pfnCmdStart = DLL_CmdStart;
	gDLL_FunctionTable_Pre.pfnCmdEnd = DLL_CmdEnd;

	memcpy(pFunctionTable, &gDLL_FunctionTable_Pre, sizeof(DLL_FUNCTIONS));

	return 1;
}

void DLL_CmdStart(const edict_t* player, const struct usercmd_s* cmd, unsigned int random_seed)
{
	if (!FNullEnt(player))
	{
		auto Player = UTIL_PlayerByIndexSafe(ENTINDEX(player));
		gSurfModFix.CmdStart(Player->edict(), cmd, random_seed);
	}

	RETURN_META(MRES_IGNORED);
}

void DLL_CmdEnd(const edict_t* player)
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

	gDLL_FunctionTable_Post.pfnServerDeactivate = DLL_POST_ServerDeactivate;

	gDLL_FunctionTable_Post.pfnStartFrame = DLL_POST_StartFrame;

	//pFunctionTable->pfnCmdStart = DLL_CmdStart;
	//pFunctionTable->pfnCmdEnd = DLL_CmdEnd;

	memcpy(pFunctionTable, &gDLL_FunctionTable_Post, sizeof(DLL_FUNCTIONS));

	return 1;
}

void DLL_POST_ServerActivate(edict_t* pEdictList, int edictCount, int clientMax)
{
	RETURN_META(MRES_IGNORED);
}

void DLL_POST_ServerDeactivate(void)
{
	RETURN_META(MRES_IGNORED);
}

void DLL_POST_StartFrame(void)
{

	RETURN_META(MRES_IGNORED);
}
#pragma endregion