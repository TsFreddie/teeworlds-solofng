/* (c) Timo Buhrmester. See licence.txt in the root of the distribution   */
/* for more information. If you are missing that file, acquire a complete */
/* release at teeworlds.com.                                              */

#include <base/system.h>

#include <engine/shared/config.h>
//#include <engine/server/server.h>

#include <game/server/gamecontext.h>
#include <game/server/entities/character.h>

#include "boomfng.h"

#define TS Server()->TickSpeed()
#define TICK Server()->Tick()
#define GS GameServer()
#define CHAR(C) (((C) < 0 || (C) >= MAX_CLIENTS) ? 0 : GS->GetPlayerChar(C))
#define PLAYER(C) (((C) < 0 || (C) >= MAX_CLIENTS) ? 0 : GS->m_apPlayers[C])
#define TPLAYER(C) (((C) < 0 || (C) >= MAX_CLIENTS) ? 0 : (GS->IsClientReady(C) && GS->IsClientPlayer(C)) ? GS->m_apPlayers[C] : 0)
#define CFG(A) g_Config.m_Sv ## A
#define FORTEAMS(T) for(int T = TEAM_RED; T != -1; T = (T==TEAM_RED?TEAM_BLUE:-1))

#if defined(CONF_FAMILY_WINDOWS)
 #define D(F, ...) dbg_msg("BoomFNG", "%s:%i:%s(): " F, __FILE__, __LINE__, \
                                                            __FUNCTION__, __VA_ARGS__)
#elif defined(CONF_FAMILY_UNIX)
 #define D(F, ARGS...) dbg_msg("BoomFNG", "%s:%i:%s(): " F, __FILE__, __LINE__, \
                                                            __func__,##ARGS)
#endif


CGameControllerBoomFNG::CGameControllerBoomFNG(class CGameContext *pGameServer)
: IGameController(pGameServer), m_ScoreDisplay(pGameServer), m_Broadcast(pGameServer)
{
	m_pGameType = "boomfng";
	m_GameFlags = GAMEFLAG_TEAMS;
	m_aCltMask[0] = m_aCltMask[1] = 0;

	Reset();
}

CGameControllerBoomFNG::~CGameControllerBoomFNG()
{
	Reset(true);
}

void CGameControllerBoomFNG::Reset(bool Destruct)
{
	for(int i = 0; i < MAX_CLIENTS; i++)
		m_aFrozenBy[i] = m_aMoltenBy[i] = m_aLastInteraction[i] = -1;

	m_Broadcast.Reset();

	*m_aRagequitAddr = '\0';

	m_ScoreDisplay.Reset(Destruct);
	m_aCltMask[0] = m_aCltMask[1] = 0;
}

void CGameControllerBoomFNG::Tick()
{
	IGameController::Tick();

	if (m_GameOverTick != -1 || m_Warmup)
		return;

	if (DoEmpty())
		return;

	DoInteractions();

	DoScoreDisplays();

	DoBroadcasts();

	DoRagequit();
}

bool CGameControllerBoomFNG::DoEmpty()
{
	bool Empty = true;

	for(int i = 0; Empty && i < MAX_CLIENTS; i++)
		if (TPLAYER(i))
			Empty = false;

	if (Empty)
		m_aTeamscore[0] = m_aTeamscore[1] = 0;
	
	return Empty;
}

void CGameControllerBoomFNG::DoHookers()
{
	for(int i = 0; i < MAX_CLIENTS; i++)
	{
		CCharacter *pChr = CHAR(i);
		if (!pChr)
			continue;
		
		int Hooking = pChr->GetHookedPlayer();

		if (Hooking >= 0)
		{
			CCharacter *pVic = CHAR(Hooking);
			if (!pVic || (pVic->GetPlayer()->GetTeam() != pChr->GetPlayer()->GetTeam() && pChr->GetHookTick() < CFG(HookRegisterDelay)))
				Hooking = -1;
		}

		int HammeredBy = pChr->LastHammeredBy();
		pChr->ClearLastHammeredBy();

		if (Hooking >= 0)
		{
			CCharacter *pVic = CHAR(Hooking);
			if (pVic)
			{
				bool SameTeam = pChr->GetPlayer()->GetTeam() == pVic->GetPlayer()->GetTeam();
				m_aLastInteraction[Hooking] = SameTeam ? -1 : i;
			}
		}

		if (HammeredBy >= 0)
		{	
			CCharacter *pHam = CHAR(HammeredBy);
			if (pHam)
			{
				bool SameTeam = pChr->GetPlayer()->GetTeam() == pHam->GetPlayer()->GetTeam();
				m_aLastInteraction[i] = SameTeam ? -1 : HammeredBy;
				if (!SameTeam && CFG(HammerFreeze) && pChr->GetFreezeTicks() <= 0)
				{
					pChr->Freeze(CFG(HammerFreeze) * TS, HammeredBy);
					m_aFrozenBy[i] = HammeredBy; //suppress kill event being generated
				}
			}
			else
				m_aLastInteraction[i] = -1;
		}
	}
}

void CGameControllerBoomFNG::DoInteractions()
{
	DoHookers();

	for(int i = 0; i < MAX_CLIENTS; i++)
	{
		CCharacter *pChr = CHAR(i);
		if (!pChr)
			continue;

		int FrzTicks = pChr->GetFreezeTicks();
		int Col = GameServer()->Collision()->GetCollisionAt(pChr->m_Pos.x, pChr->m_Pos.y);
		if (Col == TILE_SHRINE_ALL || Col == TILE_SHRINE_RED || Col == TILE_SHRINE_BLUE)
		{
			if (FrzTicks > 0 && m_aLastInteraction[i] < 0)
				pChr->Freeze(FrzTicks = 0);

			bool WasSacrificed = false;
			if (FrzTicks > 0)
			{
				int ShrineTeam = Col == TILE_SHRINE_RED ? TEAM_RED : Col == TILE_SHRINE_BLUE ? TEAM_BLUE : -1;
				HandleSacr(m_aLastInteraction[i], i, ShrineTeam);
				WasSacrificed = true;
			}

			pChr->Die(pChr->GetPlayer()->GetCID(), WEAPON_WORLD, WasSacrificed);
		}


		if (FrzTicks > 0)
		{
			if ((FrzTicks+1) % TS == 0)
			{
				if (pChr->GetPlayer()->GetTeam() != -1)
					GS->CreateDamageInd(pChr->m_Pos, 0, (FrzTicks+1) / TS, CmaskOne(pChr->GetPlayer()->GetCID()));
				else
					GS->CreateDamageInd(pChr->m_Pos, 0, (FrzTicks+1) / TS, m_aCltMask[pChr->GetPlayer()->GetTeam()&1]);
					
				if (g_Config.m_SvTickSound == 1) GS->CreateSound(GS->m_apPlayers[i]->m_ViewPos, SOUND_WEAPON_NOAMMO, CmaskOne(i));
				if (g_Config.m_SvTickSound == 2) GS->CreateSound(pChr->m_Pos, SOUND_WEAPON_NOAMMO);
			}
			if (FrzTicks == 1 && CFG(MeltSafeticks))
				pChr->SetEmote(EMOTE_SURPRISE, TICK + CFG(MeltSafeticks) + 1);

			m_aMoltenBy[i] = -1;

			if (m_aFrozenBy[i] != -1)
				continue;

			int Killer = pChr->WasFrozenBy();
			if (Killer < 0) //may happen then the gods are not pleased
			{
				m_aFrozenBy[i] = -1;
				continue;
			}

			m_aFrozenBy[i] = Killer;
			
			HandleFreeze(Killer, i);
		}
		else
		{
			m_aFrozenBy[i] = -1;

			int Melter = pChr->WasMoltenBy();
			if (Melter < 0)
				m_aMoltenBy[i] = -1;
			
			if (Melter == m_aMoltenBy[i])
				continue;

			m_aMoltenBy[i] = Melter;

			HandleMelt(Melter, i);
		}
	}
}

void CGameControllerBoomFNG::DoScoreDisplays()
{
	FORTEAMS(i)
		m_ScoreDisplay.Update(i, m_aTeamscore[i]);

	m_ScoreDisplay.Operate();
}

void CGameControllerBoomFNG::DoBroadcasts(bool ForceSend)
{
	if (m_GameOverTick != -1)
		return;

	if (max(m_aTeamscore[0], m_aTeamscore[1]) + CFG(AllYourBase) >= CFG(Scorelimit))
		m_Broadcast.Update(-1, "ALL YOUR BASE ARE BELONG TO US.", -1);

	m_Broadcast.SetDef(CFG(DefBroadcast));

	m_Broadcast.Operate();
}

void CGameControllerBoomFNG::DoRagequit()
{
	if (*m_aRagequitAddr)
	{
		char aBuf[128];
		str_format(aBuf, sizeof(aBuf), "ban %s %d %s", m_aRagequitAddr, g_Config.m_SvPunishRagequit, "Forcefully left the server while being frozen.");
		GameServer()->Console()->ExecuteLine(aBuf);
		*m_aRagequitAddr = '\0';
		/*
		NETADDR Addr;
		if (net_addr_from_str(&Addr, m_aRagequitAddr) == 0)
		{
			Addr.port = 0;
			((CServer*)Server())->m_ServerBan.BanAddr(Addr, CFG(PunishRagequit), "Forcefully left the server while being frozen.");
		}
		*m_aRagequitAddr = '\0';
		*/
	}
}

void CGameControllerBoomFNG::HandleFreeze(int Killer, int Victim)
{
	CCharacter *pVictim = CHAR(Victim);
	if (!pVictim) // for odd reasons, this can happen (confirmed by segfault). didn't yet track down why 
	{
		D("no pVictim in HandleFreeze(%d, %d)", Killer, Victim);
		return;
	}

	if (CFG(BleedOnFreeze))
	{
		pVictim->Bleed(1);
		GS->CreateSound(pVictim->m_Pos, SOUND_CTF_RETURN);
	}

	int FailTeam = pVictim->GetPlayer()->GetTeam() & 1;
	m_aTeamscore[1 - FailTeam] += CFG(FreezeTeamscore);

	if (CFG(FreezeTeamscore) && CFG(FreezeBroadcast)) //probably of no real use but for completeness...
	{
		char aBuf[64];
		str_format(aBuf, sizeof aBuf, "%s froze (%+d)", GetTeamName(1-FailTeam), CFG(FreezeTeamscore));
		m_Broadcast.Update(-1, aBuf, CFG(BroadcastTime) * TS);
	}

	CPlayer *pPlKiller = TPLAYER(Killer);

	if (!pPlKiller)
		return;

	//freezing counts as a hostile interaction
	m_aLastInteraction[pVictim->GetPlayer()->GetCID()] = pPlKiller->GetCID();

	pPlKiller->m_Score += CFG(FreezeScore);
	SendFreezeKill(Killer, Victim, WEAPON_GRENADE);

	if (pPlKiller->GetCharacter())
	{
		GS->CreateSound(pPlKiller->GetCharacter()->m_Pos, SOUND_HIT, (1<<pPlKiller->GetCID()));
		if (CFG(FreezeLoltext) && CFG(FreezeScore))
		{
			char aBuf[64];
			str_format(aBuf, sizeof aBuf, "%+d", CFG(FreezeScore));
			GS->CreateLolText(pPlKiller->GetCharacter(), false, vec2(0.f, -50.f), vec2(0.f, 0.f), 50, aBuf);
		}
	}
}

void CGameControllerBoomFNG::HandleMelt(int Melter, int Meltee)
{
	CCharacter *pMeltee = CHAR(Meltee);
	if (!pMeltee) //due to HandleFreeze, i suspect this COULD also possibly happen. 
	{
		D("no pMeltee in HandleMelt(%d, %d)", Melter, Meltee);
		return;
	}

	int MeltTeam = pMeltee->GetPlayer()->GetTeam()&1;
	m_aTeamscore[MeltTeam] += CFG(MeltTeamscore);

	if (CFG(MeltTeamscore) && CFG(MeltBroadcast))
	{
		char aBuf[64];
		str_format(aBuf, sizeof aBuf, "%s melted (%+d)", GetTeamName(MeltTeam), CFG(MeltTeamscore));
		m_Broadcast.Update(-1, aBuf, CFG(BroadcastTime) * TS);
	}

	CPlayer *pPlMelter = TPLAYER(Melter);

	if (!pPlMelter)
		return;

	pPlMelter->m_Score += CFG(MeltScore);
	SendFreezeKill(Melter, Meltee, WEAPON_HAMMER);

	if (pPlMelter->GetCharacter() && CFG(MeltLoltext) && CFG(MeltScore))
	{
		char aBuf[64];
		str_format(aBuf, sizeof aBuf, "%+d", CFG(MeltScore));
		GS->CreateLolText(pPlMelter->GetCharacter(), false, vec2(0.f, -50.f), vec2(0.f, 0.f), 50, aBuf);
	}
}

void CGameControllerBoomFNG::HandleSacr(int Killer, int Victim, int ShrineTeam)
{//assertion: Killer >= 0, victim anyways
	CCharacter *pVictim = CHAR(Victim);

	if (!pVictim) //due to HandleFreeze, i suspect this COULD also possibly happen. 
	{
		D("no pVictim in HandleSacr(%d, %d, %d)", Killer, Victim, ShrineTeam);
		return;
	}

	int FailTeam = pVictim->GetPlayer()->GetTeam();
	bool Wrong = ShrineTeam != -1 && FailTeam == ShrineTeam;

	m_aTeamscore[1-FailTeam] += Wrong?CFG(WrongSacrTeamscore):(ShrineTeam == -1 ? CFG(SacrTeamscore) : CFG(RightSacrTeamscore));

	if (!Wrong)
	{
		if (CFG(SacrSound) == 1)
			GameServer()->CreateSoundGlobal(SOUND_CTF_CAPTURE);
		else if (CFG(SacrSound) == 2)
			GameServer()->CreateSound(pVictim->m_Pos, SOUND_CTF_CAPTURE);
	}

	if (((Wrong && CFG(WrongSacrTeamscore)) || (!Wrong && (ShrineTeam == -1 ? CFG(SacrTeamscore) : CFG(RightSacrTeamscore)))) && CFG(SacrBroadcast))
	{
		char aBuf[64];
		str_format(aBuf, sizeof aBuf, "%s sacrificed%s (%+d)", GetTeamName(1-FailTeam), Wrong?" in wrong shrine":(ShrineTeam == -1 ? "" : ", pleasing their gods"),
		                                                                           Wrong?CFG(WrongSacrTeamscore):(ShrineTeam == -1 ? CFG(SacrTeamscore):CFG(RightSacrTeamscore)));
		m_Broadcast.Update(-1, aBuf, CFG(BroadcastTime) * TS);
	}

	CPlayer *pPlKiller = TPLAYER(Killer);
	if (!pPlKiller)
		return;

	pPlKiller->m_Score += Wrong?CFG(WrongSacrScore):(ShrineTeam == -1 ? CFG(SacrScore) : CFG(RightSacrScore));
	SendFreezeKill(Killer, Victim, WEAPON_NINJA);

	if (Wrong && pPlKiller->GetCharacter() && CFG(PunishWrongSacr))
	{
		pPlKiller->GetCharacter()->Freeze(CFG(PunishWrongSacr) * TS);
		GS->CreateSound(pPlKiller->GetCharacter()->m_Pos, SOUND_PLAYER_PAIN_LONG);
		GS->SendChatTarget(pPlKiller->GetCID(), "You made gods grr!");
	}

	if (!Wrong && pPlKiller->GetCharacter())
		pPlKiller->GetCharacter()->SetEmote(EMOTE_HAPPY, TICK + TS * 2);

	if (pPlKiller->GetCharacter() && CFG(SacrLoltext) && ((!Wrong && CFG(SacrScore)) || (Wrong && CFG(WrongSacrScore))))
	{
		char aBuf[64];
		str_format(aBuf, sizeof aBuf, "%+d", Wrong?CFG(WrongSacrScore):(ShrineTeam == -1 ? CFG(SacrScore) : CFG(RightSacrScore)));
		GS->CreateLolText(pPlKiller->GetCharacter(), false, vec2(0.f, -50.f), vec2(0.f, 0.f), 50, aBuf);
	}
}

void CGameControllerBoomFNG::SendFreezeKill(int Killer, int Victim, int Weapon)
{
	char aBuf[256];
	str_format(aBuf, sizeof(aBuf), "frzkill k:%d:'%s' v:%d:'%s' w:%d",
	                      Killer, Server()->ClientName(Killer),
	                      Victim, Server()->ClientName(Victim), Weapon);

	GS->Console()->Print(IConsole::OUTPUT_LEVEL_DEBUG, "game", aBuf);

	// send the kill message
	CNetMsg_Sv_KillMsg Msg;
	Msg.m_Killer = Killer;
	Msg.m_Victim = Victim;
	Msg.m_Weapon = Weapon;
	Msg.m_ModeSpecial = 0;
	Server()->SendPackMsg(&Msg, MSGFLAG_VITAL, -1);
}

bool CGameControllerBoomFNG::CanBeMovedOnBalance(int ClientID)
{
	if (!IGameController::CanBeMovedOnBalance(ClientID))
		return false;
	if (CHAR(ClientID) && CHAR(ClientID)->GetFreezeTicks() > 0)
		return false;
	return true;
}

int CGameControllerBoomFNG::OnCharacterDeath(class CCharacter *pVictim, class CPlayer *pUnusedKiller, int Weapon)
{
	m_aCltMask[pVictim->GetPlayer()->GetTeam()&1] &= ~(1<<pVictim->GetPlayer()->GetCID());

	//IGameController::OnCharacterDeath(pVictim, pKiller, Weapon);

	int Cid = pVictim->GetPlayer()->GetCID();
	if (pVictim->GetFreezeTicks() > 0 && m_aLastInteraction[Cid] != -1  && Weapon == WEAPON_GAME && CFG(PunishRagequit)) //ragequit
		Server()->GetClientAddr(Cid, m_aRagequitAddr, sizeof m_aRagequitAddr); //directly adding the ban here causes deadly trouble

	return 0;
}


void CGameControllerBoomFNG::OnCharacterSpawn(class CCharacter *pChr)
{
	m_aCltMask[pChr->GetPlayer()->GetTeam()&1] |= (1<<pChr->GetPlayer()->GetCID());
	
	IGameController::OnCharacterSpawn(pChr);

	pChr->TakeWeapon(WEAPON_GUN);
	pChr->GiveWeapon(WEAPON_GRENADE, 10);
	pChr->SetWeapon(WEAPON_GRENADE);

	int Cid = pChr->GetPlayer()->GetCID();
	for(int i = 0; i < MAX_CLIENTS; ++i)
		if (m_aLastInteraction[i] == Cid)
			m_aLastInteraction[i] = -1;

	m_aLastInteraction[pChr->GetPlayer()->GetCID()] = -1;
}

void CGameControllerBoomFNG::PostReset()
{
	IGameController::PostReset();
	Reset();
}

void CGameControllerBoomFNG::Snap(int SnappingClient)
{
	IGameController::Snap(SnappingClient);

	CNetObj_GameData *pGameDataObj = (CNetObj_GameData*)Server()->
	      SnapNewItem(NETOBJTYPE_GAMEDATA, 0, sizeof(CNetObj_GameData));

	if(!pGameDataObj)
		return;

	pGameDataObj->m_TeamscoreRed = m_aTeamscore[TEAM_RED];
	pGameDataObj->m_TeamscoreBlue = m_aTeamscore[TEAM_BLUE];

	pGameDataObj->m_FlagCarrierRed = 0;
	pGameDataObj->m_FlagCarrierBlue = 0;
}

bool CGameControllerBoomFNG::OnEntity(int Index, vec2 Pos)
{
	switch(Index)
	{
	case ENTITY_SPAWN:
	case ENTITY_SPAWN_RED:
	case ENTITY_SPAWN_BLUE:
		return IGameController::OnEntity(Index, Pos);

	default:
		if (!CFG(SuppressEntities))
			return IGameController::OnEntity(Index, Pos);
	}

	return false;
}

bool CGameControllerBoomFNG::CanJoinTeam(int Team, int NotThisID)
{
	int Can = IGameController::CanJoinTeam(Team, NotThisID);
	if (!Can)
		return false;

	CCharacter *pChr = CHAR(NotThisID);
	return !pChr || pChr->GetFreezeTicks() <= 0;
}

#undef TS
#undef TICK
#undef GS

#define TS m_pGS->Server()->TickSpeed()
#define TICK m_pGS->Server()->Tick()
#define GS m_pGS

CScoreDisplayBoom::CScoreDisplayBoom(class CGameContext *pGameServer)
: m_pGS(pGameServer)
{
	FORTEAMS(i)
		for(int j = 0; j < MAX_SCOREDISPLAYS; ++j)
			m_aScoreDisplayTextIDs[i][j] = -1;
	Reset();
}

CScoreDisplayBoom::~CScoreDisplayBoom()
{
	Reset(true);
}

void CScoreDisplayBoom::Reset(bool Destruct)
{
	FORTEAMS(i)
	{
		for(int j = 0; j < MAX_SCOREDISPLAYS; ++j)
		{
			if (m_aScoreDisplayTextIDs[i][j] != -1)
				GS->DestroyLolText(m_aScoreDisplayTextIDs[i][j]);
			m_aScoreDisplayTextIDs[i][j] = -1;
		}
		m_aScoreDisplayCount[i] = 0;
		m_aScoreDisplayValue[i] = -1;
	}
	m_Changed = 0;
	if (!Destruct)
		FindMarkers();
}

void CScoreDisplayBoom::FindMarkers()
{
	CMapItemLayerTilemap *pTMap = GS->Collision()->Layers()->GameLayer();
	CTile *pTiles = (CTile *)GS->Collision()->Layers()->Map()->GetData(pTMap->m_Data);
	for(int y = 0; y < pTMap->m_Height; y++)
	{
		for(int x = 0; x < pTMap->m_Width; x++)
		{
			int Index = pTiles[y * pTMap->m_Width + x].m_Index;
			if (Index == TILE_REDSCORE || Index == TILE_BLUESCORE)
			{
				int Team = Index - TILE_REDSCORE;
				Add(Team, vec2(x*32.f, y*32.f));
			}
				
		}
	}
}

void CScoreDisplayBoom::Add(int Team, vec2 Pos)
{
	if (m_aScoreDisplayCount[Team&1] >= MAX_SCOREDISPLAYS)
		return;
	m_aScoreDisplays[Team&1][m_aScoreDisplayCount[Team&1]++] = Pos;
}

void CScoreDisplayBoom::Update(int Team, int Score)
{
	if (m_aScoreDisplayValue[Team&1] == Score)
		return;

	m_aScoreDisplayValue[Team&1] = Score;
	m_Changed |= (1<<(Team&1));
}

void CScoreDisplayBoom::Operate()
{
	FORTEAMS(Team)
	{
		if (m_Changed & (1<<Team))
		{
			char aBuf[16];
			str_format(aBuf, sizeof aBuf, "%d", m_aScoreDisplayValue[Team]);
			for(int i = 0; i < m_aScoreDisplayCount[Team]; i++)
			{
				if (m_aScoreDisplayTextIDs[Team][i] != -1)
					GS->DestroyLolText(m_aScoreDisplayTextIDs[Team][i]);
				m_aScoreDisplayTextIDs[Team][i] = GS->CreateLolText(0, false, m_aScoreDisplays[Team][i], vec2(0.f, 0.f), 3600 * TS, aBuf);
			}
		}
	}
	
	m_Changed = 0;
}



CBroadcasterBoom::CBroadcasterBoom(class CGameContext *pGameServer)
: m_pGS(pGameServer)
{
	Reset();
}

CBroadcasterBoom::~CBroadcasterBoom()
{
	Reset();
}

void CBroadcasterBoom::SetDef(const char *pText)
{
	if (str_comp(m_aDefBroadcast, pText) != 0)
	{
		str_copy(m_aDefBroadcast, pText, sizeof m_aDefBroadcast);
		for(int i = 0; i < MAX_CLIENTS; ++i)
			if (m_aBroadcastStop[i] < 0)
				str_copy(m_aBroadcast[i], pText, sizeof m_aBroadcast[i]); //this is unfortunately required
		m_Changed = ~0;
	}
}

void CBroadcasterBoom::Update(int Cid, const char *pText, int Lifespan)
{
	if (Cid < 0) // all
	{
		for(int i = 0; i < MAX_CLIENTS; ++i)
			Update(i, pText, Lifespan);
		return;
	}

	m_aBroadcastStop[Cid] = Lifespan < 0 ? -1 : (TICK + Lifespan);
	bool Changed = str_comp(m_aBroadcast[Cid], pText) != 0;
	if (Changed)
	{
		str_copy(m_aBroadcast[Cid], pText, sizeof m_aBroadcast[Cid]);
		m_Changed |= (1<<Cid);
	}
}

void CBroadcasterBoom::Reset()
{
	for(int i = 0; i < MAX_CLIENTS; ++i)
	{
		m_aBroadcast[i][0] = '\0';
		m_aNextBroadcast[i] = m_aBroadcastStop[i] = -1;
		m_Changed = ~0;
	}
	m_aDefBroadcast[0] = '\0';
}

void CBroadcasterBoom::Operate()
{
	for(int i = 0; i < MAX_CLIENTS; ++i)
	{
		if (!GS->IsClientReady(i))
			continue;
		
		if (m_aBroadcastStop[i] >= 0 && m_aBroadcastStop[i] < TICK)
		{
			str_copy(m_aBroadcast[i], m_aDefBroadcast, sizeof m_aBroadcast[i]);
			if (!*m_aBroadcast[i])
			{
				GS->SendBroadcast(" ", i);
				m_Changed &= ~(1<<i);
			}
			else
			{
				m_Changed |= (1<<i);
			}
			m_aBroadcastStop[i] = -1;
		}

		if (((m_Changed & (1<<i)) || m_aNextBroadcast[i] < TICK) && *m_aBroadcast[i])
		{
			GS->SendBroadcast(m_aBroadcast[i], i);
			m_aNextBroadcast[i] = TICK + TS * 3;
		}
	}
	m_Changed = 0;
}

