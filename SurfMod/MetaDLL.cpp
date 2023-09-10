#include "precompiled.h"

// DLL PRE Functions Table
DLL_FUNCTIONS gDLL_FunctionTable_Pre;

// DLL POST Functions Table
DLL_FUNCTIONS gDLL_FunctionTable_Post;

#pragma region DLL_PRE
C_DLLEXPORT int GetEntityAPI2(DLL_FUNCTIONS* pFunctionTable, int* interfaceVersion)
{
	memset(&gDLL_FunctionTable_Pre, 0, sizeof(DLL_FUNCTIONS));
	
	gDLL_FunctionTable_Pre.pfnCmdStart = DLL_PRE_CmdStart;
	gDLL_FunctionTable_Pre.pfnCmdEnd = DLL_PRE_CmdEnd;

	gDLL_FunctionTable_Pre.pfnClientDisconnect = DLL_PRE_ClientDisconnect;

	memcpy(pFunctionTable, &gDLL_FunctionTable_Pre, sizeof(DLL_FUNCTIONS));

	return 1;
}

void DLL_PRE_CmdStart(const edict_t* player, const struct usercmd_s* cmd, unsigned int random_seed)
{
	if (!FNullEnt(player))
	{
		auto Player = UTIL_PlayerByIndexSafe(ENTINDEX(player));
		g_SurfModFix.CmdStart(Player->edict(), cmd, random_seed);
	}

	RETURN_META(MRES_IGNORED);
}

void DLL_PRE_CmdEnd(const edict_t* player)
{
	if (!FNullEnt(player))
	{
		auto Player = UTIL_PlayerByIndexSafe(ENTINDEX(player));
		g_SurfModFix.CmdEnd(Player->edict());
	}
	
	RETURN_META(MRES_IGNORED);
}

void DLL_PRE_ClientDisconnect(edict_t* player)
{
	if (g_SurfModDuel.m_pDuel_info.is_now_duel)
	{
		if (ENTINDEX(player) == g_SurfModDuel.m_pDuel_info.player[surfmod::Team::CT].id)
		{
			g_SurfModDuel.DuelPause(&g_SurfModDuel.m_pDuel_info.player[surfmod::Team::CT]);
			//g_SurfModDuel.ClientDisconnect(&g_SurfModDuel.m_pDuel_info.player[surfmod::Team::CT]);
		}
		else if (ENTINDEX(player) == g_SurfModDuel.m_pDuel_info.player[surfmod::Team::TER].id)
		{
			g_SurfModDuel.DuelPause(&g_SurfModDuel.m_pDuel_info.player[surfmod::Team::TER]);
		}
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

	memcpy(pFunctionTable, &gDLL_FunctionTable_Post, sizeof(DLL_FUNCTIONS));

	return 1;
}

void DLL_POST_ServerActivate(edict_t* pEdictList, int edictCount, int clientMax)
{
	g_SurfModCommand.ServerActivate();
	g_SurfModTask.ServerActivate();

	RETURN_META(MRES_IGNORED);
}

void DLL_POST_ServerDeactivate(void)
{
	g_SurfModTask.ServerDeactivate();

	RETURN_META(MRES_IGNORED);
}

void DLL_POST_StartFrame(void)
{
	g_SurfModTask.ServerFrame();

	RETURN_META(MRES_IGNORED);
}
#pragma endregion