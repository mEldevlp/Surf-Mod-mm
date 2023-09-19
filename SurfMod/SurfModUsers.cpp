#include "precompiled.h"

surfmod::CUsers g_SurfModUsers;

void surfmod::CUsers::ServerActivate()
{
	this->m_Data.clear();
	this->m_Flag.clear();

	CMemScript* lpMemScript = new CMemScript;

	if (!lpMemScript->SetBuffer(ADMIN_LIST_AMXMODX_PATH))
	{
		LOG_CONSOLE(PLID, "[%s] cannot SetBufer (%s)", __func__, ADMIN_LIST_AMXMODX_PATH);
		
		delete lpMemScript;
		return;
	}

	try
	{
		while (true)
		{
			if (lpMemScript->GetToken() == eTokenResult::TOKEN_END)
			{
				break;
			}

			P_ADMIN_INFO Info = { };

			Info.Name = lpMemScript->GetString();

			Info.Auth = lpMemScript->GetAsString();

			Info.Flag = lpMemScript->GetAsString();

			this->m_Data.insert(std::make_pair(Info.Auth, Info));
		}
	}
	catch (...)
	{
		LOG_CONSOLE(PLID, "[%s] %s", __func__, lpMemScript->GetError().c_str());
	}

	delete lpMemScript;
}

int surfmod::CUsers::ReadFlags(const char* Flags)
{
	int Result = ACCES_ADMIN::ALL;

	while (*Flags)
	{
		Result |= BIT(*Flags++ - 'a');
	}
	
	return Result;
}

bool surfmod::CUsers::PlayerConnect(edict_t* pEntity, const char* pszName, const char* pszAddress, char szRejectReason[128])
{
	if (!FNullEnt(pEntity))
	{
		auto EntityIndex = ENTINDEX(pEntity);

		this->m_Flag[EntityIndex] = ACCES_ADMIN::ALL;

		this->m_Flag[EntityIndex] |= this->ReadFlags("z");

		auto Auth = g_engfuncs.pfnGetPlayerAuthId(pEntity);

		if (this->m_Data.find(Auth) != this->m_Data.end())
		{
			this->m_Flag[EntityIndex] |= this->ReadFlags(this->m_Data[Auth].Flag.c_str());
		}	
	}

	return true;
}

int surfmod::CUsers::GetFlags(int EntityIndex)
{
	return this->m_Flag[EntityIndex];
}

std::string surfmod::CUsers::GetFlags(edict_t* pEdict)
{
	auto Auth = g_engfuncs.pfnGetPlayerAuthId(pEdict);

	if (Auth)
	{
		if (this->m_Data.find(Auth) != this->m_Data.end())
		{
			return this->m_Data[Auth].Flag;
		}
	}

	return "z";
}

int surfmod::CUsers::Access(int EntityIndex, ACCES_ADMIN Level)
{
	if (Level & ACCES_ADMIN::ADMIN)
	{
		return (this->m_Flag[EntityIndex] > ACCES_ADMIN::ALL && !(this->m_Flag[EntityIndex] & ACCES_ADMIN::USER));
	}
	else if (Level & ACCES_ADMIN::ALL)
	{
		return 1;
	}

	return (this->m_Flag[EntityIndex] & Level);
}

int surfmod::CUsers::Access(std::string Auth, ACCES_ADMIN Level)
{
	auto Admin = this->m_Data.find(Auth);

	if (Admin != this->m_Data.end())
	{
		auto Flags = this->ReadFlags(Admin->second.Flag.c_str());

		if (Level & ACCES_ADMIN::ADMIN)
		{
			return (Flags > ACCES_ADMIN::ALL && !(Flags & ACCES_ADMIN::USER));
		}
		else if (Level & ACCES_ADMIN::ALL)
		{
			return 1;
		}

		return (Flags & Level);
	}

	return 0;
}
