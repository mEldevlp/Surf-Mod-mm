#include "precompiled.h"

surfmod::CFix g_SurfModFix;

bool surfmod::CFix::CmdStart(edict_t* player, const usercmd_s* cmd, unsigned int random_seed)
{
	auto UserIndex = g_engfuncs.pfnGetPlayerUserId(player);

	if (UserIndex > 0)
	{
		this->m_flPlayer_start_velocity[UserIndex][0] = player->v.velocity.x;
		this->m_flPlayer_start_velocity[UserIndex][1] = player->v.velocity.y;
		this->m_flPlayer_start_velocity[UserIndex][2] = player->v.velocity.z;
	}

	return true;
}

bool surfmod::CFix::CmdEnd(edict_t* player)
{
	auto UserIndex = g_engfuncs.pfnGetPlayerUserId(player);
	
	if (UserIndex < 0 && UserIndex > 33)
	{
		return false;
	}

	if (player->v.velocity.x == 0.f || player->v.velocity.y == 0.f)
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

			if (this->m_flPlayer_start_velocity[UserIndex] != nullptr)
			{
				player->v.velocity.x = this->m_flPlayer_start_velocity[UserIndex][0];
				player->v.velocity.y = this->m_flPlayer_start_velocity[UserIndex][1];
				player->v.velocity.z = this->m_flPlayer_start_velocity[UserIndex][2];
			}
		}
	}
	
	return true;
}

bool surfmod::CFix::is_hull_vacant(const float origin[3], int hull, edict_t* player)
{
	TraceResult tr;

	g_engfuncs.pfnTraceHull(origin, origin, 0, hull, player, &tr);

	if (!tr.fStartSolid || !tr.fAllSolid)
	{
		return true;
	}

	return false;
}

bool surfmod::CFix::is_player_on_slope(edict_t* player)
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