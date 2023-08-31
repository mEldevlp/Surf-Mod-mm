#pragma once

#pragma region DLL_PRE
void DLL_CmdStart(const edict_t* player, const struct usercmd_s* cmd, unsigned int random_seed);
void DLL_CmdEnd(const edict_t* player);
#pragma endregion

#pragma region DLL_POST
void DLL_POST_ServerActivate(edict_t* pEdictList, int edictCount, int clientMax);
void DLL_POST_ServerDeactivate(void);
void DLL_POST_StartFrame(void);
#pragma endregion