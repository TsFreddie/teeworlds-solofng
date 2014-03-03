/* (c) Magnus Auvinen. See licence.txt in the root of the distribution for more information. */
/* If you are missing that file, acquire a complete release at teeworlds.com.                */
#ifndef GAME_VARIABLES_H
#define GAME_VARIABLES_H
#undef GAME_VARIABLES_H // this file will be included several times

//Anticamp
MACRO_CONFIG_INT(SvAnticamper, sv_anticamper, 2, 0, 2, CFGFLAG_SERVER, "0 disables, 1 enables anticamper in all modes and 2 only in Instagib")
MACRO_CONFIG_INT(SvAnticamperFreeze, sv_anticamper_freeze, 7, 0, 15, CFGFLAG_SERVER, "If a player should freeze on camping (and how long) or die")
MACRO_CONFIG_INT(SvAnticamperTime, sv_anticamper_time, 10, 5, 120, CFGFLAG_SERVER, "How long to wait till the player dies/freezes")
MACRO_CONFIG_INT(SvAnticamperRange, sv_anticamper_range, 200, 0, 1000, CFGFLAG_SERVER, "Distance how far away the player must move to escape anticamper")

// client
MACRO_CONFIG_INT(ClPredict, cl_predict, 1, 0, 1, CFGFLAG_CLIENT|CFGFLAG_SAVE, "Predict client movements")
MACRO_CONFIG_INT(ClNameplates, cl_nameplates, 1, 0, 1, CFGFLAG_CLIENT|CFGFLAG_SAVE, "Show name plates")
MACRO_CONFIG_INT(ClNameplatesAlways, cl_nameplates_always, 1, 0, 1, CFGFLAG_CLIENT|CFGFLAG_SAVE, "Always show name plates disregarding of distance")
MACRO_CONFIG_INT(ClNameplatesTeamcolors, cl_nameplates_teamcolors, 1, 0, 1, CFGFLAG_CLIENT|CFGFLAG_SAVE, "Use team colors for name plates")
MACRO_CONFIG_INT(ClNameplatesSize, cl_nameplates_size, 50, 0, 100, CFGFLAG_CLIENT|CFGFLAG_SAVE, "Size of the name plates from 0 to 100%")
MACRO_CONFIG_INT(ClAutoswitchWeapons, cl_autoswitch_weapons, 0, 0, 1, CFGFLAG_CLIENT|CFGFLAG_SAVE, "Auto switch weapon on pickup")

MACRO_CONFIG_INT(ClShowhud, cl_showhud, 1, 0, 1, CFGFLAG_CLIENT|CFGFLAG_SAVE, "Show ingame HUD")
MACRO_CONFIG_INT(ClShowfps, cl_showfps, 0, 0, 1, CFGFLAG_CLIENT|CFGFLAG_SAVE, "Show ingame FPS counter")

MACRO_CONFIG_INT(ClAirjumpindicator, cl_airjumpindicator, 1, 0, 1, CFGFLAG_CLIENT|CFGFLAG_SAVE, "")
MACRO_CONFIG_INT(ClThreadsoundloading, cl_threadsoundloading, 0, 0, 1, CFGFLAG_CLIENT|CFGFLAG_SAVE, "Load sound files threaded")

MACRO_CONFIG_INT(ClWarningTeambalance, cl_warning_teambalance, 1, 0, 1, CFGFLAG_CLIENT|CFGFLAG_SAVE, "Warn about team balance")

MACRO_CONFIG_INT(ClMouseDeadzone, cl_mouse_deadzone, 300, 0, 0, CFGFLAG_CLIENT|CFGFLAG_SAVE, "")
MACRO_CONFIG_INT(ClMouseFollowfactor, cl_mouse_followfactor, 60, 0, 200, CFGFLAG_CLIENT|CFGFLAG_SAVE, "")
MACRO_CONFIG_INT(ClMouseMaxDistance, cl_mouse_max_distance, 800, 0, 0, CFGFLAG_CLIENT|CFGFLAG_SAVE, "")

MACRO_CONFIG_INT(EdShowkeys, ed_showkeys, 0, 0, 1, CFGFLAG_CLIENT|CFGFLAG_SAVE, "")

//MACRO_CONFIG_INT(ClFlow, cl_flow, 0, 0, 1, CFGFLAG_CLIENT|CFGFLAG_SAVE, "")

MACRO_CONFIG_INT(ClShowWelcome, cl_show_welcome, 1, 0, 1, CFGFLAG_CLIENT|CFGFLAG_SAVE, "")
MACRO_CONFIG_INT(ClMotdTime, cl_motd_time, 10, 0, 100, CFGFLAG_CLIENT|CFGFLAG_SAVE, "How long to show the server message of the day")

MACRO_CONFIG_STR(ClVersionServer, cl_version_server, 100, "version.teeworlds.com", CFGFLAG_CLIENT|CFGFLAG_SAVE, "Server to use to check for new versions")

MACRO_CONFIG_STR(ClLanguagefile, cl_languagefile, 255, "", CFGFLAG_CLIENT|CFGFLAG_SAVE, "What language file to use")

MACRO_CONFIG_INT(PlayerUseCustomColor, player_use_custom_color, 0, 0, 1, CFGFLAG_CLIENT|CFGFLAG_SAVE, "Toggles usage of custom colors")
MACRO_CONFIG_INT(PlayerColorBody, player_color_body, 65408, 0, 0xFFFFFF, CFGFLAG_CLIENT|CFGFLAG_SAVE, "Player body color")
MACRO_CONFIG_INT(PlayerColorFeet, player_color_feet, 65408, 0, 0xFFFFFF, CFGFLAG_CLIENT|CFGFLAG_SAVE, "Player feet color")
MACRO_CONFIG_STR(PlayerSkin, player_skin, 24, "default", CFGFLAG_CLIENT|CFGFLAG_SAVE, "Player skin")

MACRO_CONFIG_INT(UiPage, ui_page, 6, 0, 10, CFGFLAG_CLIENT|CFGFLAG_SAVE, "Interface page")
MACRO_CONFIG_INT(UiToolboxPage, ui_toolbox_page, 0, 0, 2, CFGFLAG_CLIENT|CFGFLAG_SAVE, "Toolbox page")
MACRO_CONFIG_STR(UiServerAddress, ui_server_address, 64, "localhost:8303", CFGFLAG_CLIENT|CFGFLAG_SAVE, "Interface server address")
MACRO_CONFIG_INT(UiScale, ui_scale, 100, 50, 150, CFGFLAG_CLIENT|CFGFLAG_SAVE, "Interface scale")
MACRO_CONFIG_INT(UiMousesens, ui_mousesens, 100, 5, 100000, CFGFLAG_SAVE|CFGFLAG_CLIENT, "Mouse sensitivity for menus/editor")

MACRO_CONFIG_INT(UiColorHue, ui_color_hue, 160, 0, 255, CFGFLAG_CLIENT|CFGFLAG_SAVE, "Interface color hue")
MACRO_CONFIG_INT(UiColorSat, ui_color_sat, 70, 0, 255, CFGFLAG_CLIENT|CFGFLAG_SAVE, "Interface color saturation")
MACRO_CONFIG_INT(UiColorLht, ui_color_lht, 175, 0, 255, CFGFLAG_CLIENT|CFGFLAG_SAVE, "Interface color lightness")
MACRO_CONFIG_INT(UiColorAlpha, ui_color_alpha, 228, 0, 255, CFGFLAG_CLIENT|CFGFLAG_SAVE, "Interface alpha")

MACRO_CONFIG_INT(GfxNoclip, gfx_noclip, 0, 0, 1, CFGFLAG_CLIENT|CFGFLAG_SAVE, "Disable clipping")

// server
MACRO_CONFIG_INT(SvWarmup, sv_warmup, 0, 0, 0, CFGFLAG_SERVER, "Number of seconds to do warmup before round starts")
MACRO_CONFIG_STR(SvMotd, sv_motd, 900, "This is OpenFNG by fisted\n\nGet it at:\nhttp://github.com/fisted/teeworlds\nBranch: openfng\n\nIRC:\n#OpenFNG on QuakeNet (irc.quakenet.org)\n\nReport problems at:\nhttp://github.com/fisted/teeworlds/issues\n\nHave fun and good luck", CFGFLAG_SERVER, "Message of the day to display for the clients")
MACRO_CONFIG_INT(SvTeamdamage, sv_teamdamage, 0, 0, 1, CFGFLAG_SERVER, "Team damage")
MACRO_CONFIG_STR(SvMaprotation, sv_maprotation, 768, "openfng3 openfng5", CFGFLAG_SERVER, "Maps to rotate between")
MACRO_CONFIG_INT(SvRoundsPerMap, sv_rounds_per_map, 1, 1, 100, CFGFLAG_SERVER, "Number of rounds on each map before rotating")
MACRO_CONFIG_INT(SvRoundSwap, sv_round_swap, 1, 0, 1, CFGFLAG_SERVER, "Swap teams between rounds")
MACRO_CONFIG_INT(SvPowerups, sv_powerups, 1, 0, 1, CFGFLAG_SERVER, "Allow powerups like ninja")
MACRO_CONFIG_INT(SvScorelimit, sv_scorelimit, 1000, 0, 1000, CFGFLAG_SERVER, "Score limit (0 disables)")
MACRO_CONFIG_INT(SvTimelimit, sv_timelimit, 0, 0, 1000, CFGFLAG_SERVER, "Time limit in minutes (0 disables)")
MACRO_CONFIG_STR(SvGametype, sv_gametype, 32, "openfng", CFGFLAG_SERVER, "Game type (dm, tdm, ctf)")
MACRO_CONFIG_INT(SvTournamentMode, sv_tournament_mode, 0, 0, 1, CFGFLAG_SERVER, "Tournament mode. When enabled, players joins the server as spectator")
MACRO_CONFIG_INT(SvSpamprotection, sv_spamprotection, 1, 0, 1, CFGFLAG_SERVER, "Spam protection")

MACRO_CONFIG_INT(SvRespawnDelayTDM, sv_respawn_delay_tdm, 3, 0, 10, CFGFLAG_SERVER, "Time needed to respawn after death in tdm gametype")

// these might need some fine tuning
MACRO_CONFIG_INT(SvChatPenalty, sv_chat_penalty, 250, 50, 1000, CFGFLAG_SERVER, "chatscore will be increased by this on every message, and decremented by 1 on every tick.")
MACRO_CONFIG_INT(SvChatThreshold, sv_chat_threshold, 1000, 50, 10000 , CFGFLAG_SERVER, "if chatscore exceeds this, the player will be muted for sv_spam_mute_duration seconds")
MACRO_CONFIG_INT(SvSpamMuteDuration, sv_spam_mute_duration, 60, 0, 3600 , CFGFLAG_SERVER, "how many seconds to mute, if player triggers mute on spam. 0 = off")

MACRO_CONFIG_INT(SvSpectatorSlots, sv_spectator_slots, 0, 0, MAX_CLIENTS, CFGFLAG_SERVER, "Number of slots to reserve for spectators")
MACRO_CONFIG_INT(SvTeambalanceTime, sv_teambalance_time, 1, 0, 1000, CFGFLAG_SERVER, "How many minutes to wait before autobalancing teams")
MACRO_CONFIG_INT(SvInactiveKickTime, sv_inactivekick_time, 3, 0, 1000, CFGFLAG_SERVER, "How many minutes to wait before taking care of inactive players")
MACRO_CONFIG_INT(SvInactiveKick, sv_inactivekick, 0, 0, 2, CFGFLAG_SERVER, "How to deal with inactive players (0=move to spectator, 1=move to free spectator slot/kick, 2=kick)")

MACRO_CONFIG_INT(SvEmotionalTees, sv_emotional_tees, 1, 0, 1, CFGFLAG_SERVER, "eye emote on emoticons")

MACRO_CONFIG_INT(SvStrictSpectateMode, sv_strict_spectate_mode, 0, 0, 1, CFGFLAG_SERVER, "Restricts information in spectator mode")
MACRO_CONFIG_INT(SvVoteSpectate, sv_vote_spectate, 1, 0, 1, CFGFLAG_SERVER, "Allow voting to move players to spectators")
MACRO_CONFIG_INT(SvVoteSpectateRejoindelay, sv_vote_spectate_rejoindelay, 3, 0, 1000, CFGFLAG_SERVER, "How many minutes to wait before a player can rejoin after being moved to spectators by vote")
MACRO_CONFIG_INT(SvVoteKick, sv_vote_kick, 1, 0, 1, CFGFLAG_SERVER, "Allow voting to kick players")
MACRO_CONFIG_INT(SvVoteKickMin, sv_vote_kick_min, 0, 0, MAX_CLIENTS, CFGFLAG_SERVER, "Minimum number of players required to start a kick vote")
MACRO_CONFIG_INT(SvVoteKickBantime, sv_vote_kick_bantime, 5, 0, 1440, CFGFLAG_SERVER, "The time to ban a player if kicked by vote. 0 makes it just use kick")

MACRO_CONFIG_INT(SvDamage, sv_damage, 0, 0, 1, CFGFLAG_SERVER, "allow damage")
MACRO_CONFIG_INT(SvUnlimitedAmmo, sv_unlimited_ammo, 0, 0, 1, CFGFLAG_SERVER, "guess what")
MACRO_CONFIG_INT(SvNinja, sv_ninja, 0, 0, 1, CFGFLAG_SERVER, "allow ninja")

MACRO_CONFIG_INT(SvHammerMelt, sv_hammer_melt, 3, 0, 60, CFGFLAG_SERVER, "how many ticks to subtract from freezetime, when hammering frozen teammates")
MACRO_CONFIG_INT(SvMeltSafeticks, sv_melt_safeticks, 20, 0, 300, CFGFLAG_SERVER, "how many seconds to be unfreezable after melting (not when being molten per hammer)")

MACRO_CONFIG_INT(SvFreezeScore, sv_freeze_score, 1, 0, 10, CFGFLAG_SERVER, "player score for freezing")
MACRO_CONFIG_INT(SvFreezeTeamscore, sv_freeze_teamscore, 1, 0, 10, CFGFLAG_SERVER, "team score for freezing")
MACRO_CONFIG_INT(SvSacrScore, sv_sacr_score, 3, 0, 10, CFGFLAG_SERVER, "player score for sacrificing")
MACRO_CONFIG_INT(SvSacrTeamscore, sv_sacr_teamscore, 5, 0, 10, CFGFLAG_SERVER, "team score for sacrificing")
MACRO_CONFIG_INT(SvWrongSacrScore, sv_wrong_sacr_score, -5, -10, 0, CFGFLAG_SERVER, "player score for sacrificing in wrong shrine")
MACRO_CONFIG_INT(SvWrongSacrTeamscore, sv_wrong_sacr_teamscore, -1, -10, 0, CFGFLAG_SERVER, "team score for sacrificing in wrong shrine")
MACRO_CONFIG_INT(SvRightSacrScore, sv_right_sacr_score, 5, 0, 20, CFGFLAG_SERVER, "player score for sacrificing in wrong shrine")
MACRO_CONFIG_INT(SvRightSacrTeamscore, sv_right_sacr_teamscore, 10, 0, 20, CFGFLAG_SERVER, "team score for sacrificing in wrong shrine")
MACRO_CONFIG_INT(SvMeltScore, sv_melt_score, 1, 0, 10, CFGFLAG_SERVER, "player score for melting")
MACRO_CONFIG_INT(SvMeltTeamscore, sv_melt_teamscore, 0, 0, 10, CFGFLAG_SERVER, "team score for melting")

MACRO_CONFIG_INT(SvSuppressEntities, sv_suppress_entities, 1, 0, 1, CFGFLAG_SERVER, "for non fng maps, disables spawning of pickups")
MACRO_CONFIG_INT(SvBroadcasts, sv_broadcasts, 1, 0, 1, CFGFLAG_SERVER, "allow broadcasts")
MACRO_CONFIG_STR(SvDefBroadcast, sv_def_broadcast, 128, "", CFGFLAG_SERVER, "default broadcast to display")
MACRO_CONFIG_INT(SvBroadcastTime, sv_broadcast_time, 3, 1, 10, CFGFLAG_SERVER, "default time for broadcasts to appear")

MACRO_CONFIG_INT(SvHammerScaleX, sv_hammer_scale_x, 320, 1, 1000, CFGFLAG_SERVER, "linearly scale up hammer x power, percentage, for hammering enemies and unfrozen teammates")
MACRO_CONFIG_INT(SvHammerScaleY, sv_hammer_scale_y, 120, 1, 1000, CFGFLAG_SERVER, "linearly scale up hammer y power, percentage, for hammering enemies and unfrozen teammates")

MACRO_CONFIG_INT(SvMeltHammerScaleX, sv_melt_hammer_scale_x, 50, 1, 1000, CFGFLAG_SERVER, "linearly scale up hammer x power, percentage, for hammering frozen teammates")
MACRO_CONFIG_INT(SvMeltHammerScaleY, sv_melt_hammer_scale_y, 50, 1, 1000, CFGFLAG_SERVER, "linearly scale up hammer y power, percentage, for hammering frozen teammates")

MACRO_CONFIG_INT(SvLoltextHspace, sv_loltext_hspace, 14, 10, 25, CFGFLAG_SERVER, "horizontal offset between loltext 'pixels'")
MACRO_CONFIG_INT(SvLoltextVspace, sv_loltext_vspace, 14, 10, 25, CFGFLAG_SERVER, "vertical offset between loltext 'pixels'")

MACRO_CONFIG_INT(SvSacrLoltext, sv_sacr_loltext, 0, 0, 1, CFGFLAG_SERVER, "display a loltext when sacrificing")
MACRO_CONFIG_INT(SvFreezeLoltext, sv_freeze_loltext, 0, 0, 1, CFGFLAG_SERVER, "display a loltext when freezing")
MACRO_CONFIG_INT(SvMeltLoltext, sv_melt_loltext, 0, 0, 1, CFGFLAG_SERVER, "display a loltext when melting")

MACRO_CONFIG_INT(SvSacrBroadcast, sv_sacr_broadcast, 1, 0, 1, CFGFLAG_SERVER, "display a broadcast when sacrificing")
MACRO_CONFIG_INT(SvFreezeBroadcast, sv_freeze_broadcast, 0, 0, 1, CFGFLAG_SERVER, "display a broadcast when freezing")
MACRO_CONFIG_INT(SvMeltBroadcast, sv_melt_broadcast, 0, 0, 1, CFGFLAG_SERVER, "display a broadcast when melting by hammer")

MACRO_CONFIG_INT(SvEmoticonDelay, sv_emoticon_delay, 2, 0, 5, CFGFLAG_SERVER, "be careful with 0 as it allows for emoticon spam")

MACRO_CONFIG_INT(SvLaserSkipFrozen, sv_laser_skip_frozen, 0, 0, 1, CFGFLAG_SERVER, "allow/disallow shooting through frozen tees")
MACRO_CONFIG_INT(SvLaserSkipTeammates, sv_laser_skip_teammates, 0, 0, 1, CFGFLAG_SERVER, "allow/disallow shooting through teammates")

MACRO_CONFIG_INT(SvSacrSound, sv_sacr_sound, 1, 0, 2, CFGFLAG_SERVER, "play ctf capture sound on sacrification (0 = off, 1 = global, 2 = local")

MACRO_CONFIG_INT(SvAllYourBase, sv_all_your_base, 50, 0, 200, CFGFLAG_SERVER, "display AYB if one team does only need this many score in order to win")

MACRO_CONFIG_INT(SvHookRegisterDelay, sv_hook_register_delay, 10, 0, 100, CFGFLAG_SERVER, "require this many ticks to keep a player hooked until it counts as an interaction (sacr)")

MACRO_CONFIG_INT(SvBloodInterval, sv_blood_interval, 1, 1, 300, CFGFLAG_SERVER, "should stay at 1 for openfng (as we bleed for only 1 tick)")
MACRO_CONFIG_INT(SvBleedOnFreeze, sv_bleed_on_freeze, 1, 0, 1, CFGFLAG_SERVER, "'blood' splash + sound on freezing someone")

MACRO_CONFIG_INT(SvPunishRagequit, sv_punish_ragequit, 30, 0, 120, CFGFLAG_SERVER, "number of seconds to ban for forcefully leaving the game while frozen. 0 = off.")
MACRO_CONFIG_INT(SvPunishWrongSacr, sv_punish_wrong_sacr, 6, 0, 30, CFGFLAG_SERVER, "number of seconds to freeze who is sacrificing in a wrong-colored shrine.")

MACRO_CONFIG_INT(SvHammerFreeze, sv_hammer_freeze, 0, 0, 10, CFGFLAG_SERVER, "number of seconds to freeze when hit with a hammer")

MACRO_CONFIG_INT(SvEndvoteTime, sv_endvote_time, 60, 0, 180, CFGFLAG_SERVER, "cause vote on extending the map as soon as there are this many seconds left to hit the timelimit")
MACRO_CONFIG_INT(SvEndvoteScore, sv_endvote_score, 50, 0, 100, CFGFLAG_SERVER, "cause vote on extending the map as soon as there is this much score left to hit the scorelimit")
MACRO_CONFIG_INT(SvExtend, sv_extend, 0, 0, 0, CFGFLAG_SERVER, "when set to 1 before round ends, another round on the same map is enforced and this is reset to 0")

//Old-School feature
MACRO_CONFIG_INT(SvTickSound, sv_tick_sound, 0, 0, 2, CFGFLAG_SERVER, "Ticking sound 0-disable 1-local 2-global")
MACRO_CONFIG_INT(SvSuperHammer, sv_super_hammer, 0, 0, 1, CFGFLAG_SERVER, "Super Hammer")
MACRO_CONFIG_INT(SvVivid, sv_vivid, 0, 0, 1, CFGFLAG_SERVER, "Jumpup and vivid animation when unfreeze")

// debug
#ifdef CONF_DEBUG // this one can crash the server if not used correctly
	MACRO_CONFIG_INT(DbgDummies, dbg_dummies, 0, 0, 15, CFGFLAG_SERVER, "")
#endif

MACRO_CONFIG_INT(DbgFocus, dbg_focus, 0, 0, 1, CFGFLAG_CLIENT, "")
MACRO_CONFIG_INT(DbgTuning, dbg_tuning, 0, 0, 1, CFGFLAG_CLIENT, "")
#endif
