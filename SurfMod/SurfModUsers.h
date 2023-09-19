#pragma once

#define ADMIN_LIST_AMXMODX_PATH "cstrike/addons/SurfMod/configs/admins.ini"

namespace surfmod {

	typedef struct S_ADMIN_INFO
	{
		std::string Auth;
		std::string Name;
		std::string Flag;
	} P_ADMIN_INFO, * LP_ADMIN_INFO;

	enum ACCES_ADMIN 
	{
		ALL = 0,
		IMMUNITY	= BIT(0),	// flag - a
		RESERVATION = BIT(1),	// flag - b
		KICK		= BIT(2),	// flag - c
		BAN			= BIT(3),	// flag - d
		SLAY		= BIT(4),	// flag - e
		MAP			= BIT(5),	// flag - f
		CVAR		= BIT(6),	// flag - g
		CFG			= BIT(7),	// flag - h
		CHAT		= BIT(8),	// flag - i
		VOTE		= BIT(9),	// flag - j
		PASSWORD	= BIT(10),	// flag - k
		RCON		= BIT(11),	// flag - l
		LEVEL_A		= BIT(12),	// flag - m
		LEVEL_B		= BIT(13),	// flag - n
		LEVEL_C		= BIT(14),	// flag - o
		LEVEL_D		= BIT(15),	// flag - p
		LEVEL_E		= BIT(16),	// flag - q
		LEVEL_F		= BIT(17),	// flag - r
		LEVEL_G		= BIT(18),	// flag - s
		LEVEL_H		= BIT(19),	// flag - t
		MENU		= BIT(20),	// flag - u
		BAN_TEMP	= BIT(21),	// flag - v

		ADMIN		= BIT(24),	// flag - y
		USER		= BIT(25)	// flag - z
	};

	class CUsers
	{
	public:
		void ServerActivate();

		int ReadFlags(const char* Flags);

		bool PlayerConnect(edict_t* pEntity, const char* pszName, const char* pszAddress, char szRejectReason[128]);

		int GetFlags(int EntityIndex);

		std::string GetFlags(edict_t* pEdict);

		int Access(int EntityIndex, ACCES_ADMIN Level);

		int Access(std::string Auth, ACCES_ADMIN Level);

	private:
		std::unordered_map<std::string, P_ADMIN_INFO> m_Data;
		
		std::unordered_map<int, int> m_Flag;
	};

}; /* namespace surfmod */

extern surfmod::CUsers g_SurfModUsers;