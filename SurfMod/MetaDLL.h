#pragma once

#pragma region DLL_PRE

#pragma endregion

#pragma region DLL_POST
void DLL_CmdStart(const edict_t* player, const struct usercmd_s* cmd, unsigned int random_seed);
void DLL_CmdEnd(const edict_t* player);
#pragma endregion