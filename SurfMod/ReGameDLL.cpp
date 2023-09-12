#include "precompiled.h"

IReGameApi			*g_ReGameApi;
const ReGameFuncs_t	*g_ReGameFuncs;
IReGameHookchains	*g_ReGameHookchains;
CGameRules			*g_pGameRules;

bool ReGameDLL_Init()
{
	const char *szGameDLLModule = GET_GAME_INFO(PLID, GINFO_DLL_FULLPATH);

	if (!szGameDLLModule)
	{
		LOG_CONSOLE(PLID, "[%s] Failed to get GameDLL module", Plugin_info.logtag);
		return false;
	}

	CSysModule *gameModule = Sys_LoadModule(szGameDLLModule);

	if (!gameModule)
	{
		LOG_CONSOLE(PLID, "[%s] Failed to locate GameDLL module", Plugin_info.logtag);
		return false;
	}

	CreateInterfaceFn ifaceFactory = Sys_GetFactory(gameModule);

	if (!ifaceFactory)
	{
		LOG_CONSOLE(PLID, "[%s] Failed to locate interface factory in GameDLL module", Plugin_info.logtag);
		return false;
	}

	int retCode = 0;

	g_ReGameApi = (IReGameApi *)ifaceFactory(VRE_GAMEDLL_API_VERSION, &retCode);

	if (!g_ReGameApi)
	{
		LOG_CONSOLE(PLID, "[%s] Failed to locate retrieve rehlds api interface from GameDLL module, return code is %d", Plugin_info.logtag, retCode);
		return false;
	}

	int majorVersion = g_ReGameApi->GetMajorVersion();
	int minorVersion = g_ReGameApi->GetMinorVersion();

	if (majorVersion != REGAMEDLL_API_VERSION_MAJOR)
	{
		LOG_CONSOLE(PLID, "[%s] ReGameDLL API major version mismatch; expected %d, real %d", Plugin_info.logtag, REGAMEDLL_API_VERSION_MAJOR, majorVersion);

		if (majorVersion < REGAMEDLL_API_VERSION_MAJOR)
		{
			LOG_CONSOLE(PLID, "[%s] Please update the ReGameDLL up to a major version API >= %d", Plugin_info.logtag, REGAMEDLL_API_VERSION_MAJOR);
		}
		else if (majorVersion > REGAMEDLL_API_VERSION_MAJOR)
		{
			LOG_CONSOLE(PLID, "[%s] Please update the %s up to a major version API >= %d", Plugin_info.logtag, Plugin_info.logtag, majorVersion);
		}

		return false;
	}

	if (minorVersion < REGAMEDLL_API_VERSION_MINOR)
	{
		LOG_CONSOLE(PLID, "[%s] ReGameDLL API minor version mismatch; expected at least %d, real %d", Plugin_info.logtag, REGAMEDLL_API_VERSION_MINOR, minorVersion);
		LOG_CONSOLE(PLID, "[%s] Please update the ReGameDLL up to a minor version API >= %d", Plugin_info.logtag, REGAMEDLL_API_VERSION_MINOR);
		return false;
	}

	g_ReGameFuncs = g_ReGameApi->GetFuncs();

	g_ReGameHookchains = g_ReGameApi->GetHookchains();

	if (!g_ReGameApi->BGetICSEntity(CSENTITY_API_INTERFACE_VERSION))
	{
		LOG_CONSOLE(PLID, "[%s] Interface CCSEntity API version '%s' not found", Plugin_info.logtag, CSENTITY_API_INTERFACE_VERSION);
		return false;
	}

	g_ReGameHookchains->InstallGameRules()->registerHook(ReGameDLL_InstallGameRules);
	g_ReGameHookchains->InternalCommand()->registerHook(ReGameDLL_InternalCommand);
	g_ReGameHookchains->RoundEnd()->registerHook(ReGameDLL_RoundEnd);
	g_ReGameHookchains->HandleMenu_ChooseTeam()->registerHook(ReGameDLL_ChooseTeam);
	g_ReGameHookchains->CSGameRules_RestartRound()->registerHook(ReGameDLL_RestartRound);
	g_ReGameHookchains->CBasePlayer_RoundRespawn()->registerHook(ReGameDLL_RoundRespawn);
	return true;
}

bool ReGameDLL_Stop()
{
	g_ReGameHookchains->InstallGameRules()->unregisterHook(ReGameDLL_InstallGameRules);
	g_ReGameHookchains->InternalCommand()->unregisterHook(ReGameDLL_InternalCommand);
	g_ReGameHookchains->RoundEnd()->unregisterHook(ReGameDLL_RoundEnd);
	g_ReGameHookchains->HandleMenu_ChooseTeam()->unregisterHook(ReGameDLL_ChooseTeam);
	g_ReGameHookchains->CSGameRules_RestartRound()->unregisterHook(ReGameDLL_RestartRound);
	g_ReGameHookchains->CBasePlayer_RoundRespawn()->unregisterHook(ReGameDLL_RoundRespawn);

	return true;
}

CGameRules* ReGameDLL_InstallGameRules(IReGameHook_InstallGameRules* chain)
{
	auto gamerules = chain->callNext();

	if (!g_ReGameApi->BGetIGameRules(GAMERULES_API_INTERFACE_VERSION))
	{
		LOG_CONSOLE(PLID, "[%s] Interface GameRules API version '%s' not found", Plugin_info.logtag, GAMERULES_API_INTERFACE_VERSION);
	}
	else
	{
		g_pGameRules = gamerules;
	}
	
	return gamerules;
}

void ReGameDLL_InternalCommand(IReGameHook_InternalCommand* chain, edict_t* pEntity, const char* pcmd, const char* parg1)
{
	auto Player = UTIL_PlayerByIndexSafe(ENTINDEX(pEntity));

	if (Player)
	{
		if (g_SurfModCommand.ClientCommand(Player, pcmd, parg1))
		{
			return void();
		}
	}

	chain->callNext(pEntity, pcmd, parg1);
}

#define SCORE_TO_WIN 10

bool ReGameDLL_RoundEnd(IReGameHook_RoundEnd* chain, int winStatus, ScenarioEventEndRound event, float tmDelay)
{
	auto roundend = chain->callNext(winStatus, event, tmDelay);

	if (!g_SurfModDuel.m_pDuel_info.is_now_duel)
	{
		return roundend;
	}

	if (g_SurfModDuel.m_pDuel_info.state == surfmod::DUEL_STATE::PAUSE)
	{
		return roundend;
	}

	auto& ct = g_SurfModDuel.m_pDuel_info.player[surfmod::Team::CT];
	auto& ter = g_SurfModDuel.m_pDuel_info.player[surfmod::Team::TER];

	switch (event)
	{
		case ScenarioEventEndRound::ROUND_CTS_WIN:
		{
			++ct.score;
			break;
		}
		case ScenarioEventEndRound::ROUND_TERRORISTS_WIN:
		{
			++ter.score;
			break;
		}
		default:
		{
			return roundend;
		}
	}

	if (ct.score >= SCORE_TO_WIN && (ct.score - ter.score) >= 2)
	{
		g_SurfModDuel.DuelWon(&ct);
	}
	else if (ter.score >= SCORE_TO_WIN && (ter.score - ct.score) >= 2)
	{
		g_SurfModDuel.DuelWon(&ter);
	}
	else
	{
		if ((ct.score + ter.score) != 0)
		{
			g_SurfModUtility.SayText(nullptr, PRINT_TEAM_BLUE, _TXT("^3%s ^1- ^4%d", ct.name, ct.score));
			g_SurfModUtility.SayText(nullptr, PRINT_TEAM_RED, _TXT("^3%s ^1- ^4%d", ter.name, ter.score));

			g_SurfModDuel.UpdateGameInfo();
		}
	}

	return roundend;
}

BOOL ReGameDLL_ChooseTeam(IReGameHook_HandleMenu_ChooseTeam* chain, CBasePlayer* pPlayer, int slot)
{
	if (g_SurfModDuel.m_pDuel_info.is_now_duel)
	{
		switch (pPlayer->m_iTeam)
		{
			case TeamName::UNASSIGNED:
			{
				pPlayer->CSPlayer()->JoinTeam(TeamName::SPECTATOR);
				g_SurfModUtility.SayText(pPlayer->edict(), PRINT_TEAM_RED, "^3You've been moved to spectators because ^4the duel ^3is going up.");
				break;
			}
			case TeamName::SPECTATOR:
			{
				g_SurfModUtility.SayText(pPlayer->edict(), PRINT_TEAM_RED, "^3You can't choose a team because ^4the duel ^3is going up.");
				break;
			}
			default: // for duelists
			{
				g_SurfModUtility.SayText(pPlayer->edict(), PRINT_TEAM_RED, "^3You can't change team. ^1Type ^4/resign ^1 to lose duel.");
				return 1;
			}
		}
	}

	return chain->callNext(pPlayer, slot);
}

void ReGameDLL_RestartRound(IReGameHook_CSGameRules_RestartRound* chain)
{
	chain->callNext();

	g_SurfModDuel.RoundRestart();
}

void ReGameDLL_RoundRespawn(IReGameHook_CBasePlayer_RoundRespawn* chain, CBasePlayer* pPlayer)
{
	if (g_SurfModDuel.m_pDuel_info.is_now_duel)
	{
		auto& ct = g_SurfModDuel.m_pDuel_info.player[surfmod::Team::CT];
		auto& ter = g_SurfModDuel.m_pDuel_info.player[surfmod::Team::TER];

		if (pPlayer->entindex() == ct.id)
		{
			CSGameRules()->m_iNumCTWins = ct.score;
		}
		else if (pPlayer->entindex() == ter.id)
		{
			CSGameRules()->m_iNumTerroristWins = ter.score;
		}

		CSGameRules()->UpdateTeamScores();
	}

	chain->callNext(pPlayer);
}