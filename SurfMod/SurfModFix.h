#pragma once

constexpr auto TRACE_HULL_HEIGHT = 0.001f;
constexpr auto UNSTUCK_HEIGHT = 0.001f;

#define HULL_POINT                      0
#define HULL_HUMAN                      1
#define HULL_LARGE                      2
#define HULL_HEAD                       3

namespace surfmod
{
	class CFix
	{
	public:
		bool CmdStart(edict_t* player, const struct usercmd_s* cmd, unsigned int random_seed);
		bool CmdEnd(edict_t* player);

	private:		
		bool is_hull_vacant(const float origin[3], int hull, edict_t* player);
		bool is_player_on_slope(edict_t* player);	

		vec_t m_flPlayer_start_velocity[32][3];
	};

}; /* namespace surfmod */

extern surfmod::CFix g_SurfModFix;