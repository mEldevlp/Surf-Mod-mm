#include "precompiled.h"

surfmod::CUtility g_SurfModUtility;

void surfmod::CUtility::SayText(edict_t* pEntity, int Sender, const char* Format, ...)
{
	static int iMsgSayText;

	if (iMsgSayText || (iMsgSayText = gpMetaUtilFuncs->pfnGetUserMsgID(PLID, "SayText", NULL)))
	{
		char Buffer[MAX_LINE_TXT_MSG] = { 0 };

		va_list ArgList;
		va_start(ArgList, Format);
		Q_vsnprintf(Buffer, sizeof(Buffer), Format, ArgList);
		va_end(ArgList);

		char Temp[MAX_LINE_TXT_MSG] = { 0 };

		Q_snprintf(Temp, sizeof(Temp), "^1[^4%s^1] %s", Plugin_info.logtag, Buffer);

		Q_strcpy_s(Buffer, Temp);

		if (Sender < PRINT_TEAM_BLUE || Sender > gpGlobals->maxClients)
		{
			Sender = PRINT_TEAM_DEFAULT;
		}
		else if (Sender < PRINT_TEAM_DEFAULT)
		{
			Sender = abs(Sender) + MAX_CLIENTS;
		}

		this->ParseLinesAndColors(Buffer);

		if (!FNullEnt(pEntity))
		{
			if (!(pEntity->v.flags & FL_FAKECLIENT))
			{
				g_engfuncs.pfnMessageBegin(MSG_ONE, iMsgSayText, nullptr, pEntity);
				g_engfuncs.pfnWriteByte(Sender ? Sender : ENTINDEX(pEntity));
				g_engfuncs.pfnWriteString("%s");
				g_engfuncs.pfnWriteString(Buffer);
				g_engfuncs.pfnMessageEnd();
			}
		}
		else
		{
			for (int i = 1; i <= gpGlobals->maxClients; ++i)
			{
				edict_t* pTempEntity = INDEXENT(i);

				if (!FNullEnt(pTempEntity))
				{
					if (!(pTempEntity->v.flags & FL_FAKECLIENT))
					{
						g_engfuncs.pfnMessageBegin(MSG_ONE, iMsgSayText, nullptr, pTempEntity);
						g_engfuncs.pfnWriteByte(Sender ? Sender : i);
						g_engfuncs.pfnWriteString("%s");
						g_engfuncs.pfnWriteString(Buffer);
						g_engfuncs.pfnMessageEnd();
					}
				}
			}
		}
	}
}

std::vector<CBasePlayer*> surfmod::CUtility::GetPlayers(bool InGameOnly, bool ReturnBots)
{
	std::vector<CBasePlayer*> Players;

	for (int i = 1; i <= gpGlobals->maxClients; ++i)
	{
		auto Player = UTIL_PlayerByIndexSafe(i);

		if (Player)
		{
			if (!Player->IsDormant())
			{
				if (InGameOnly)
				{
					if (Player->m_iTeam != TeamName::TERRORIST  &&  Player->m_iTeam != TeamName::CT)
					{
						continue;
					}
				}

				if (!ReturnBots && Player->IsBot())
				{
					continue;
				}

				Players.push_back(Player);
			}
		}
	}

	return Players;
}

int surfmod::CUtility::ParseLinesAndColors(char* Buffer)
{
	size_t len = strlen(Buffer);

	int offs = 0;

	if (len > 0)
	{
		int c;

		for (size_t i = 0; i < len; i++)
		{
			c = Buffer[i];

			if (c == '^' && (i != len - 1))
			{
				c = Buffer[++i];

				if (c == 'n' || c == 't' || (c >= '1' && c <= '4'))
				{
					switch (c)
					{
					case '1': c = '\x01'; break;
					case '2': c = '\x02'; break;
					case '3': c = '\x03'; break;
					case '4': c = '\x04'; break;
					case 'n': c = '\n'; break;
					case 't': c = '\t'; break;
					}

					offs++;
				}
			}

			Buffer[i - offs] = c;
		}

		Buffer[len - offs] = '\0';
	}

	return offs;
}

const char* surfmod::CUtility::FormatString(const char* Format, ...)
{
	va_list argList;

	static char VarArgs[512];

	va_start(argList, Format);

	vsnprintf(VarArgs, sizeof(VarArgs), Format, argList);

	va_end(argList);

	return VarArgs;
}

void surfmod::CUtility::ServerCommand(const char* Format, ...)
{
	char cmd[255] = { 0 };

	va_list	argptr;

	va_start(argptr, Format);

	vsnprintf(cmd, sizeof(cmd), Format, argptr);

	va_end(argptr);

	Q_strncat(cmd, "\n", 1);

	g_engfuncs.pfnServerCommand(cmd);
}

void surfmod::CUtility::ReplaceAll(std::string& String, const std::string& From, const std::string& To)
{
	if (!From.empty())
	{
		// Starting position
		size_t StartPos = 0;

		// While string is found on that position
		while((StartPos = String.find(From, StartPos)) != std::string::npos)
		{
			// Replace with string
			String.replace(StartPos, From.length(), To);

			// Increment starting position
			StartPos += To.length();
		}
	}
}
