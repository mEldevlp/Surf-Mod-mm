#include "precompiled.h"

SurfMod::Fix gSurfModFix;

bool SurfMod::Fix::CmdStart(edict_t* player, const usercmd_s* cmd, unsigned int random_seed)
{
	auto UserIndex = g_engfuncs.pfnGetPlayerUserId(player);
	auto CPlayer = UTIL_PlayerByIndexSafe(UserIndex);

	if (!CPlayer->IsAlive())
	{
		return false;
	}
	else
	{
		gSurfModFix.m_player_start_velocity[UserIndex][0] = player->v.velocity.x;
		gSurfModFix.m_player_start_velocity[UserIndex][1] = player->v.velocity.y;
		gSurfModFix.m_player_start_velocity[UserIndex][2] = player->v.velocity.z;

		return true;
	}
}

bool SurfMod::Fix::CmdEnd(edict_t* player)
{
	auto UserIndex = g_engfuncs.pfnGetPlayerUserId(player);

	if (!UTIL_PlayerByIndexSafe(UserIndex)->IsAlive())
	{
		return false;
	}

	if (this->is_player_on_slope(player))
	{
		float playerOrigin[3] = { 0.f };

		auto hull = (player->v.flags & FL_DUCKING) ? HULL_HEAD : HULL_HUMAN;

		playerOrigin[0] = player->v.origin.x;
		playerOrigin[1] = player->v.origin.y;
		playerOrigin[2] = player->v.origin.z + UNSTUCK_HEIGHT;

		if (this->is_hull_vacant(playerOrigin, hull, player))
		{
			player->v.origin.z = player->v.origin.z + UNSTUCK_HEIGHT;
			player->v.velocity.x = gSurfModFix.m_player_start_velocity[UserIndex][0];
			player->v.velocity.y = gSurfModFix.m_player_start_velocity[UserIndex][1];
			player->v.velocity.z = gSurfModFix.m_player_start_velocity[UserIndex][2];
		}
	}

	return false;
}

bool SurfMod::Fix::is_vector_zero(const vec_t velocity[3])
{
	return (velocity[0] == 0.f) && (velocity[1] == 0.f);
}

bool SurfMod::Fix::is_hull_vacant(const float origin[3], int hull, edict_t* player)
{
	TraceResult tr;

	g_engfuncs.pfnTraceHull(origin, origin, 0, hull, player, &tr);

	if (!tr.fStartSolid || !tr.fAllSolid)
	{
		return true;
	}

	return false;
}

bool SurfMod::Fix::is_player_on_slope(edict_t* player)
{
	float traceHullEnd[3] = { 0.f }, playerOrigin[3] = { 0.f };
	TraceResult tr;

	playerOrigin[0] = player->v.origin.x;
	playerOrigin[1] = player->v.origin.y;
	playerOrigin[2] = player->v.origin.z;

	traceHullEnd[0] = playerOrigin[0];
	traceHullEnd[1] = playerOrigin[1];
	traceHullEnd[2] = playerOrigin[2] - TRACE_HULL_HEIGHT;

	auto hull = (player->v.flags & FL_DUCKING) ? HULL_HEAD : HULL_HUMAN;

	g_engfuncs.pfnTraceHull(playerOrigin, traceHullEnd, IGNORE_MONSTERS::ignore_monsters | IGNORE_MONSTERS::missile, hull, player, &tr);

	float rampAngle = tr.vecPlaneNormal.z;

	return (rampAngle != 0.0f) && (rampAngle != 1.0f);
}