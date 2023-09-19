#include "precompiled.h"

surfmod::CDuel g_SurfModDuel;

void surfmod::CDuel::JudgeMenuMain(int EntityIndex)
{
	auto Flags = g_SurfModUsers.GetFlags(EntityIndex);

	if (!(Flags & ACCES_ADMIN::MENU))
	{
		g_SurfModUtility.SayText(INDEXENT(EntityIndex), PRINT_TEAM_RED, "^3You can not do this!");
		return;
	}

	std::string title = g_SurfModUtility.FormatString("^rJudge Panel\n\n");
	bool isduel = this->m_pDuel_info.is_now_duel;
	bool ispreparing = this->m_pDuel_info.state == DUEL_STATE::PREPARING;
	bool blockkey = isduel || ispreparing;

	g_SurfModMenu[EntityIndex].Create(title, true, reinterpret_cast<void*>(this->JudgeMenuMain_Handle));

	g_SurfModMenu[EntityIndex].AddItem(blockkey ? -1 : 0, _TXT("Choose players %s", blockkey ? "^y(^rduel is going up^y)\n" : "\0"), blockkey);

	if (isduel)
	{
		g_SurfModMenu[EntityIndex].AddItem(1, "Give Round");

		g_SurfModMenu[EntityIndex].AddItem(2, "Take Round\n\n^r-------- DANGER ZONE --------");

		g_SurfModMenu[EntityIndex].AddItem(3, "Backup Round");

		g_SurfModMenu[EntityIndex].AddItem(4, "Give Win");

		g_SurfModMenu[EntityIndex].AddItem(5, _TXT("%s", this->m_pDuel_info.state == DUEL_STATE::GOING ? "Pause the duel\n" : "Unpause the duel\n"));

		g_SurfModMenu[EntityIndex].AddItem(6, "Abort the duel ^y| \\d(^rcarefully\\d)");
	}

	g_SurfModMenu[EntityIndex].Show(EntityIndex);
}

void surfmod::CDuel::JudgeMenuMain_Handle(int EntityIndex, P_MENU_ITEM Item)
{
	switch (Item.Info)
	{
		case 0:
		{
			std::memset(g_SurfModDuel.m_iJudge_choice[EntityIndex], 0, sizeof(g_SurfModDuel.m_iJudge_choice[EntityIndex]));

			g_SurfModDuel.JudgeChoosePlayerMenu(EntityIndex);
			break;
		}
		case 1:
		{
			g_SurfModDuel.GiveRoundMenu(EntityIndex);
			
			break;
		}
		case 2:
		{
			g_SurfModDuel.TakeRoundMenu(EntityIndex);
			
			break;
		}
		case 3:
		{
			g_SurfModDuel.BackupRoundMenu(EntityIndex);
			break;
		}
		case 4:
		{
			g_SurfModDuel.GiveWinMenu(EntityIndex);
			break;
		}
		case 5:
		{
			if (g_SurfModDuel.m_pDuel_info.state == DUEL_STATE::GOING)
			{
				g_SurfModDuel.DuelPause(EntityIndex);
			}
			else
			{
				g_SurfModDuel.DuelUnpause(5);
			}

			break;
		}
		case 6:
		{
			g_SurfModDuel.AbortDuel(EntityIndex);
			break;
		}
		default:
		{
			g_SurfModUtility.SayText(INDEXENT(EntityIndex), PRINT_TEAM_RED, _TXT("^3You can not do this right now!"));
		}
	}
}

void surfmod::CDuel::JudgeChoosePlayerMenu(int EntityIndex)
{
	std::string title = _TXT("^rJudge Panel\n^yChoose a pair of players:\n\n");

	// First choice
	if (g_SurfModDuel.m_iJudge_choice[EntityIndex][0] != 0)
	{
		int choiceid = g_SurfModDuel.m_iJudge_choice[EntityIndex][0];
		const char* nameVictim = STRING(INDEXENT(choiceid)->v.netname);
		title += _TXT("^w%s - ^rCT\n%c", nameVictim, g_SurfModDuel.m_iJudge_choice[EntityIndex][1] == 0 ? '\n' : '\0');
	}

	// Second choice
	if(g_SurfModDuel.m_iJudge_choice[EntityIndex][1] != 0)
	{
		int choiceid = g_SurfModDuel.m_iJudge_choice[EntityIndex][1];
		const char* nameVictim = STRING(INDEXENT(choiceid)->v.netname);

		title += g_SurfModUtility.FormatString("^w%s - ^rTT\n\n", nameVictim);
		g_SurfModMenu[EntityIndex].Create(title, true, reinterpret_cast<void*>(this->JudgeChoosePlayerMenu_Handle));
		g_SurfModMenu[EntityIndex].AddItem(0, "Start duel!");

		g_SurfModMenu[EntityIndex].Show(EntityIndex);
		return void();
	}

	// display players
	g_SurfModMenu[EntityIndex].Create(title, true, reinterpret_cast<void*>(this->JudgeChoosePlayerMenu_Handle));
	
	auto Players = g_SurfModUtility.GetPlayers(false, true);

	for (auto const& Player : Players)
	{
		if (Player->entindex() == g_SurfModDuel.m_iJudge_choice[EntityIndex][0])
		{
			g_SurfModMenu[EntityIndex].AddItem(-1, STRING(Player->edict()->v.netname), true);
		}
		else if (Player->m_iTeam == TeamName::UNASSIGNED)
		{
			g_SurfModMenu[EntityIndex].AddItem(-2, _TXT("%s ^w(^rnot in game^w)", STRING(Player->edict()->v.netname)), true);
		}
		else
		{
			g_SurfModMenu[EntityIndex].AddItem(Player->entindex(), STRING(Player->edict()->v.netname));
		}
	}

	g_SurfModMenu[EntityIndex].Show(EntityIndex);
}

void surfmod::CDuel::JudgeChoosePlayerMenu_Handle(int EntityIndex, P_MENU_ITEM Item)
{

	if (Item.Info == -1)
	{
		g_SurfModUtility.SayText(INDEXENT(EntityIndex), PRINT_TEAM_RED, "^3You can not choose him twice!");
		g_SurfModDuel.JudgeChoosePlayerMenu(EntityIndex);

		return void();
	}
	else if (Item.Info == -2)
	{
		g_SurfModUtility.SayText(INDEXENT(EntityIndex), PRINT_TEAM_RED, "^3This player is not in game yet");
		g_SurfModDuel.JudgeChoosePlayerMenu(EntityIndex);

		return void();
	}

	if (g_SurfModDuel.m_iJudge_choice[EntityIndex][0] != 0 && g_SurfModDuel.m_iJudge_choice[EntityIndex][1] != 0)
	{
		g_SurfModDuel.m_iJudge_id = EntityIndex;
		g_SurfModDuel.m_pDuel_info.player[Team::CT].init(g_SurfModDuel.m_iJudge_choice[EntityIndex][0], Team::CT);
		g_SurfModDuel.m_pDuel_info.player[Team::TER].init(g_SurfModDuel.m_iJudge_choice[EntityIndex][1], Team::TER);

		g_SurfModDuel.StartDuel();

		return void();
	}

	g_SurfModDuel.m_iJudge_choice[EntityIndex][g_SurfModDuel.m_iJudge_choice[EntityIndex][0] == 0 ? 0 : 1] = Item.Info;
	g_SurfModDuel.JudgeChoosePlayerMenu(EntityIndex);
}

void surfmod::CDuel::AbortDuel(int EntityIndex)
{
	if (EntityIndex != -1)
	{
		auto judge = INDEXENT(EntityIndex);
		g_SurfModUtility.SayText(nullptr, PRINT_TEAM_RED, _TXT("^3Judge ^1[^4%s^1] ^3has terminated the duel."), STRING(judge->v.netname));
	}
	else
	{
		g_SurfModUtility.SayText(nullptr, PRINT_TEAM_RED, _TXT("^3Aborting duel..."));
	}

	g_SurfModTask.Remove(TASK_MATCH_START);
	g_SurfModUtility.ServerCommand("sv_restart %d", 5);
	CSGameRules()->m_bGameStarted = true;
	// play sound here

	this->State(DUEL_STATE::NOTHING);
}

void surfmod::CDuel::BackupRoundMenu(int EntityIndex)
{
	std::string title = g_SurfModUtility.FormatString("^rBackup round\n\n");

	g_SurfModMenu[EntityIndex].Create(title, true, reinterpret_cast<void*>(this->BackupRoundMenu_Handle));

	for (int i = 0; i < this->m_iRoundsCount; i++)
	{
		g_SurfModMenu[EntityIndex].AddItem(i, _TXT("CT - ^y%d \\d||| ^y%d ^w- TER", this->m_pDuel_info.round[i][0], this->m_pDuel_info.round[i][1]));
	}

	g_SurfModMenu[EntityIndex].Show(EntityIndex);
}

void surfmod::CDuel::BackupRoundMenu_Handle(int EntityIndex, P_MENU_ITEM Item)
{
	g_SurfModDuel.m_pDuel_info.state = DUEL_STATE::PAUSE;

	auto judge = INDEXENT(EntityIndex);

	g_SurfModUtility.SayText(nullptr, PRINT_TEAM_RED, _TXT("^3Judge ^1[^4%s^1] ^3has backup round ^1(%d)"), STRING(judge->v.netname), Item.Info);

	g_SurfModDuel.m_pDuel_info.player[Team::CT].score = g_SurfModDuel.m_pDuel_info.round[Item.Info][0];
	g_SurfModDuel.m_pDuel_info.player[Team::TER].score = g_SurfModDuel.m_pDuel_info.round[Item.Info][1];

	g_SurfModUtility.SayText(nullptr, PRINT_TEAM_GREY, _TXT("^3Scorecard: ^4%s ^1%d ^3: ^1%d ^4%s",
		g_SurfModDuel.m_pDuel_info.player[Team::CT].name,
		g_SurfModDuel.m_pDuel_info.player[Team::CT].score,
		g_SurfModDuel.m_pDuel_info.player[Team::TER].score,
		g_SurfModDuel.m_pDuel_info.player[Team::TER].name
	));

	g_SurfModDuel.State(DUEL_STATE::UNPAUSE);

	g_SurfModDuel.UpdateGameInfo(false);
}

void surfmod::CDuel::GiveRoundMenu(int EntityIndex)
{
	std::string title = g_SurfModUtility.FormatString("^rGive Round now\n\n");

	auto& ct = this->m_pDuel_info.player[Team::CT];
	auto& ter = this->m_pDuel_info.player[Team::TER];

	g_SurfModMenu[EntityIndex].Create(title, true, reinterpret_cast<void*>(this->GiveRoundMenu_Handle));

	g_SurfModMenu[EntityIndex].AddItem(0, _TXT("%s \\d- ^rCT \\d- ^y%d", ct.name, ct.score), false, static_cast<int>(Team::CT));
	g_SurfModMenu[EntityIndex].AddItem(1, _TXT("%s \\d- ^rTT \\d- ^y%d", ter.name, ter.score), false, static_cast<int>(Team::TER));

	g_SurfModMenu[EntityIndex].Show(EntityIndex);
}

void surfmod::CDuel::GiveRoundMenu_Handle(int EntityIndex, P_MENU_ITEM Item)
{
	auto judge = INDEXENT(EntityIndex);
	Team winner = static_cast<Team>(Item.Extra);

	auto giveround = [=](Team winner, Team loser)
	{
		MDLL_ClientKill(g_SurfModDuel.m_pDuel_info.player[loser].base->edict());
		g_SurfModUtility.SayText(nullptr, PRINT_TEAM_GREY, "^3Judge ^1[^4%s^1] ^3gave round to ^1[^4%s^1]", STRING(judge->v.netname),
			g_SurfModDuel.m_pDuel_info.player[winner].name);

		static int iMsgSendAudio = gpMetaUtilFuncs->pfnGetUserMsgID(PLID, "SendAudio", NULL);

		g_engfuncs.pfnMessageBegin(MSG_BROADCAST, iMsgSendAudio, nullptr, nullptr);
		g_engfuncs.pfnWriteByte(0);
		g_engfuncs.pfnWriteString("%!MRAD_rounddraw");
		g_engfuncs.pfnWriteShort(100);
		g_engfuncs.pfnMessageEnd();

		CSGameRules()->EndRoundMessage("#Round_Draw", 10);
	};

	giveround(winner, winner == Team::CT ? Team::TER : Team::CT);
}

void surfmod::CDuel::TakeRoundMenu(int EntityIndex)
{
	std::string title = g_SurfModUtility.FormatString("^rTake Round\n\n");

	auto& ct = this->m_pDuel_info.player[Team::CT];
	auto& ter = this->m_pDuel_info.player[Team::TER];

	g_SurfModMenu[EntityIndex].Create(title, true, reinterpret_cast<void*>(this->TakeRoundMenu_Handle));

	if (ct.score > 0)
	{
		g_SurfModMenu[EntityIndex].AddItem(0, _TXT("%s \\d- ^rCT \\d- ^y%d", ct.name, ct.score), false, static_cast<int>(Team::CT));
	}
	else
	{
		g_SurfModMenu[EntityIndex].AddItem(-1, _TXT("%s - CT - %d", ct.name, ct.score), true);
	}

	if (ter.score > 0)
	{
		g_SurfModMenu[EntityIndex].AddItem(1, _TXT("%s \\d- ^rTT \\d- ^y%d", ter.name, ter.score), false, static_cast<int>(Team::TER));
	}
	else
	{
		g_SurfModMenu[EntityIndex].AddItem(-1, _TXT("%s - TT - %d", ter.name, ter.score), true);
	}

	g_SurfModMenu[EntityIndex].Show(EntityIndex);
}

void surfmod::CDuel::GiveWinMenu(int EntityIndex)
{
	std::string title = g_SurfModUtility.FormatString("^r-------- DANGER ZONE --------\n^rGive Win\n\n");

	auto& ct = this->m_pDuel_info.player[Team::CT];
	auto& ter = this->m_pDuel_info.player[Team::TER];

	g_SurfModMenu[EntityIndex].Create(title, true, reinterpret_cast<void*>(this->GiveWinMenu_Handle));
	g_SurfModMenu[EntityIndex].AddItem(0, _TXT("%s \\d- ^rCT \\d- ^y%d", ct.name, ct.score), false, static_cast<int>(Team::CT));
	g_SurfModMenu[EntityIndex].AddItem(1, _TXT("%s \\d- ^rTT \\d- ^y%d", ter.name, ter.score), false, static_cast<int>(Team::TER));

	g_SurfModMenu[EntityIndex].Show(EntityIndex);
}

void surfmod::CDuel::GiveWinMenu_Handle(int EntityIndex, P_MENU_ITEM Item)
{
	auto judge = INDEXENT(EntityIndex);
	auto& winner = g_SurfModDuel.m_pDuel_info.player[static_cast<Team>(Item.Extra)];

	static int iMsgSendAudio = gpMetaUtilFuncs->pfnGetUserMsgID(PLID, "SendAudio", NULL);

	g_engfuncs.pfnMessageBegin(MSG_BROADCAST, iMsgSendAudio, nullptr, nullptr);
	g_engfuncs.pfnWriteByte(0);
	g_engfuncs.pfnWriteString(winner.team == TeamName::CT ? "%!MRAD_ctwin" : "%!MRAD_terwin");
	g_engfuncs.pfnWriteShort(100);
	g_engfuncs.pfnMessageEnd();

	g_SurfModUtility.SayText(nullptr, PRINT_TEAM_GREY, "^3Judge ^1[^4%s^1] ^3gave ^4match win ^3to ^1[^4%s^1]", STRING(judge->v.netname), winner.name);

	g_SurfModDuel.DuelWon(&winner);
}

void surfmod::CDuel::TakeRoundMenu_Handle(int EntityIndex, P_MENU_ITEM Item)
{
	if (Item.Info == -1)
	{
		g_SurfModUtility.SayText(nullptr, PRINT_TEAM_RED, _TXT("^3You can't took round of this player!"));
		return;
	}

	auto judge = INDEXENT(EntityIndex);
	Team loser = static_cast<Team>(Item.Extra);

	auto takeround = [=](Team loser, Team winner)
	{
		g_SurfModDuel.m_pDuel_info.player[winner].score--;
		g_SurfModDuel.m_pDuel_info.player[loser].score--;

		MDLL_ClientKill(g_SurfModDuel.m_pDuel_info.player[loser].base->edict());

		g_SurfModUtility.SayText(nullptr, PRINT_TEAM_GREY, "^3Judge ^1[^4%s^1] ^3took round from ^1[^4%s^1]", STRING(judge->v.netname),
			g_SurfModDuel.m_pDuel_info.player[loser].name);

		static int iMsgSendAudio = gpMetaUtilFuncs->pfnGetUserMsgID(PLID, "SendAudio", NULL);

		g_engfuncs.pfnMessageBegin(MSG_BROADCAST, iMsgSendAudio, nullptr, nullptr);
		g_engfuncs.pfnWriteByte(0);
		g_engfuncs.pfnWriteString("%!MRAD_rounddraw");
		g_engfuncs.pfnWriteShort(100);
		g_engfuncs.pfnMessageEnd();

		CSGameRules()->EndRoundMessage("#Round_Draw", ScenarioEventEndRound::ROUND_END_DRAW);
	};

	takeround(loser, loser == Team::CT ? Team::TER : Team::CT);
}

void surfmod::CDuel::StartDuel()
{
	auto Players = g_SurfModUtility.GetPlayers(false, true);

	// everyone to spec
	for (auto const& Player : Players)
	{
		Player->CSPlayer()->JoinTeam(TeamName::SPECTATOR);
	}

	this->m_pDuel_info.player[Team::CT].base->CSPlayer()->JoinTeam(TeamName::CT);
	this->m_pDuel_info.player[Team::TER].base->CSPlayer()->JoinTeam(TeamName::TERRORIST);

	this->State(DUEL_STATE::PREPARING);
}

void surfmod::CDuel::DuelWon(duel_player_t* player)
{
	this->m_pDuel_info.player[Team::WINNER] = *player;

	this->State(DUEL_STATE::WINING);
}

void surfmod::CDuel::DuelPause(duel_player_t* player)
{
	this->m_pDuel_info.state = DUEL_STATE::PAUSE;

	if (this->m_iDuelists_out == 1)
	{
		g_SurfModUtility.SayText(nullptr, player->print_team, _TXT("^3%s ^1has been ^4disconnected^1! Duel on pause.", player->name));
		g_SurfModUtility.SayText(nullptr, PRINT_TEAM_GREY, _TXT("^3Waiting for the ^4judge ^3decision..."));

		g_SurfModDuel.JudgeMenuMain(g_SurfModDuel.m_iJudge_id);
	}
	else if (this->m_iDuelists_out == 2)
	{
		g_SurfModUtility.SayText(nullptr, PRINT_TEAM_GREY, _TXT("^3All of ^4duelists ^1has been ^4disconnected^1!"));
		this->AbortDuel();
	}
}

void surfmod::CDuel::DuelPause(int EntityIndex)
{
	this->m_pDuel_info.state = DUEL_STATE::PAUSE;
	g_SurfModUtility.SayText(nullptr, PRINT_TEAM_GREY, _TXT("^3Judge ^1[^4%s^1] ^3force duel pause."), STRING(INDEXENT(EntityIndex)->v.netname));
}

void surfmod::CDuel::PlayerResign(int EntityIndex)
{
	if (!this->m_pDuel_info.is_now_duel)
	{
		return;
	}

	if (this->m_pDuel_info.player[Team::CT].id != EntityIndex || this->m_pDuel_info.player[Team::TER].id != EntityIndex)
	{
		return;
	}

	auto loser = UTIL_PlayerByIndexSafe(EntityIndex);
	auto winner = loser->m_iTeam == TeamName::CT ? Team::TER : Team::CT;

	g_SurfModUtility.SayText(nullptr, PRINT_TEAM_RED, _TXT("^4%s ^3has resign!"), STRING(loser->edict()->v.netname));

	this->DuelWon(&this->m_pDuel_info.player[winner]);
}

void surfmod::CDuel::State(DUEL_STATE state)
{
	switch (this->m_pDuel_info.state = state)
	{
		case DUEL_STATE::NOTHING:
		{
			this->m_pDuel_info.player.clear();
			this->m_pDuel_info.round.clear();
			this->m_iRoundsCount = 0;
			this->m_pDuel_info.is_now_duel = false;
			break;
		}

		case DUEL_STATE::PREPARING:
		{
			this->StartRestarting();
			break;
		}

		case DUEL_STATE::PAUSE:
		{
			break;
		}

		case DUEL_STATE::UNPAUSE:
		{
			g_SurfModUtility.SayText(nullptr, PRINT_TEAM_DEFAULT, _TXT("^4Duel unpaused. ^3Get ready for restart."));

			this->m_pDuel_info.state = DUEL_STATE::GOING;
			g_SurfModUtility.ServerCommand("sv_restart %d", 7);
			CSGameRules()->m_bGameStarted = true;
			break;
		}

		case DUEL_STATE::GOING:
		{
			g_SurfModUtility.HudMessage(nullptr, g_SurfModUtility.HudParam(50, 200, 255, -1.0, 0.35, 1, 13.0, 10.0, 0.0f, 0.0f, 1),
				_TXT("%s", g_SurfModDuel.m_pDuel_info.player[Team::CT].name));

			g_SurfModUtility.HudMessage(nullptr, g_SurfModUtility.HudParam(255, 255, 255, -1.0, 0.38, 1, 13.0, 10.0, 0.0f, 0.0f, 2),
				_TXT("---------- VS ----------"));

			g_SurfModUtility.HudMessage(nullptr, g_SurfModUtility.HudParam(220, 40, 40, -1.0, 0.41, 1, 13.0, 10.0, 0.0f, 0.0f, 3),
				_TXT("%s", g_SurfModDuel.m_pDuel_info.player[Team::TER].name));

			this->m_pDuel_info.player[Team::CT].score = 0;
			this->m_pDuel_info.player[Team::TER].score = 0;
			this->m_pDuel_info.is_now_duel = true;

			this->UpdateGameInfo();

			break;
		}

		case DUEL_STATE::WINING:
		{
			auto print_team = this->m_pDuel_info.player[Team::WINNER].print_team;
			auto winner_team = this->m_pDuel_info.player[Team::WINNER].team;
			auto winner_name = this->m_pDuel_info.player[Team::WINNER].name;

			CSGameRules()->EndRoundMessage(_TXT("%s won the duel!", winner_name),
				winner_team == TeamName::CT ? ScenarioEventEndRound::ROUND_CTS_WIN : ScenarioEventEndRound::ROUND_TERRORISTS_WIN);

			g_SurfModUtility.SayText(nullptr, print_team, _TXT("^3%s ^4won the duel!", winner_name));

			this->State(DUEL_STATE::NOTHING);
			break;
		}

		default: break;
	}
}

void surfmod::CDuel::ClientDisconnect(edict_t* player)
{
	if (this->m_pDuel_info.is_now_duel)
	{
		auto steamid = g_engfuncs.pfnGetPlayerAuthId(player);

		if (!Q_strcmp(steamid, this->m_pDuel_info.player[Team::CT].auth_id.c_str()))
		{
			this->m_iDuelists_out++;
			g_SurfModDuel.DuelPause(&this->m_pDuel_info.player[Team::CT]);
		}
		else if (!Q_strcmp(steamid, this->m_pDuel_info.player[Team::TER].auth_id.c_str()))
		{
			this->m_iDuelists_out++;
			g_SurfModDuel.DuelPause(&this->m_pDuel_info.player[Team::TER]);
		}
	}
}

void surfmod::CDuel::DuelUnpause(int time)
{
	g_SurfModDuel.State(DUEL_STATE::UNPAUSE);
}

void surfmod::CDuel::DuelistComeback(int player_id, Team player_team)
{
	int score = this->m_pDuel_info.player[player_team].score;
	this->m_pDuel_info.player[player_team].init(player_id, player_team);
	this->m_pDuel_info.player[player_team].base->CSPlayer()->JoinTeam(static_cast<TeamName>(player_team));
	this->m_pDuel_info.player[player_team].score = score;

	g_SurfModUtility.SayText(nullptr, this->m_pDuel_info.player[player_team].print_team, "^3%s ^1is back! ^4Continue the duel.", this->m_pDuel_info.player[player_team].name);

	g_SurfModTask.Create(TASK_DUEL_UNPAUSE, 5.f, false, reinterpret_cast<void*>(this->DuelUnpause), 5);
}

void surfmod::CDuel::StartRestarting()
{
	this->m_iRestart_times = 3;

	g_SurfModUtility.ServerCommand("sv_restart %d", this->m_iRestart_times);
	
	CSGameRules()->m_bGameStarted = true;

	g_SurfModUtility.SayText(nullptr, PRINT_TEAM_DEFAULT, _TXT("^3Initialization of the duel. ^1Get ready for restart."));
}

void surfmod::CDuel::RoundRestart()
{
	if (this->m_pDuel_info.state == DUEL_STATE::PREPARING)
	{
		--this->m_iRestart_times;

		if (this->m_iRestart_times > 0)
		{
			g_SurfModUtility.ServerCommand("sv_restart %d", this->m_iRestart_times);
		}
		else if (this->m_iRestart_times == 0)
		{
			g_SurfModUtility.ServerCommand("sv_restart %d", 5);
			g_SurfModUtility.SayText(nullptr, PRINT_TEAM_DEFAULT, _TXT("^3Last restart. ^4gl & hf!"));
		}
		else if (this->m_iRestart_times == -1)
		{
			this->State(DUEL_STATE::GOING);
		}
	}
}

void surfmod::CDuel::UpdateGameInfo(bool isAddRound)
{
	this->m_pDuel_info.round.push_back({ this->m_pDuel_info.player[Team::CT].score, this->m_pDuel_info.player[Team::TER].score });

	this->m_iRoundsCount += isAddRound ? 1 : 0;
}