#pragma once

#pragma region DLL_PRE
void DLL_PRE_CmdStart(const edict_t* player, const struct usercmd_s* cmd, unsigned int random_seed);
void DLL_PRE_CmdEnd(const edict_t* player);
void DLL_PRE_ClientDisconnect(edict_t* player);
BOOL DLL_PRE_ClientConnect(edict_t* pEntity, const char* pszName, const char* pszAddress, char szRejectReason[128]);
#pragma endregion

#pragma region DLL_POST
void DLL_POST_ServerActivate(edict_t* pEdictList, int edictCount, int clientMax);
void DLL_POST_ServerDeactivate(void);
void DLL_POST_StartFrame(void);
void DLL_POST_ClientPutInServer(edict_t* player);
#pragma endregion