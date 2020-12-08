class PVEZ_Notifications : Managed {

//////////////////////////////////////////////////////////////////////////
////                            ON SERVER                             ////
	static void PersonalNotificationServer(PlayerBase recipient, int type, int duration, bool isCountdown, int zoneIndex = -1) {
		Param4<int, int, bool, int> data = new Param4<int, int, bool, int>(type, duration, isCountdown, zoneIndex);
		GetGame().RPCSingleParam(recipient, PVEZ_RPC.NOTIFICATION_PERSONAL, data, true, recipient.GetIdentity());
	}

	static void ServerWideNotificationServer(EntityAI killer, EntityAI victim, EntityAI weapon) {
		if (!g_Game.pvez_Config.LAWBREAKERS_SYSTEM.Server_Wide_Message_About_Lawbreaker)
			return;
		
		string weaponDisplayName;
		if (weapon == killer)
			weaponDisplayName = "#pvez_bare_hands";
		else
			weaponDisplayName = weapon.GetDisplayName();
		string serverMessage = FormatMessage(PVEZ_NotificationTypes.NOTIF_LB_SERVERWIDE, PlayerBase.Cast(killer).GetIdentity().GetName(), victim.GetDisplayName(), weaponDisplayName);

		autoptr ref array<Man> players = new array<Man>;
		GetGame().GetPlayers(players);
		if (players.Count() > 0) {
			foreach (Man man : players) {
				if (!man.IsAlive() || man == killer)
					continue;

				if (g_Game.pvez_Config.GENERAL.Use_UI_Notifications)
					GetGame().RPCSingleParam(PlayerBase.Cast(man), PVEZ_RPC.NOTIFICATION_SERVERWIDE, new Param1<string>(serverMessage), true, PlayerBase.Cast(man).GetIdentity());
				else
					GetGame().RPCSingleParam(PlayerBase.Cast(man), ERPCs.RPC_USER_ACTION_MESSAGE, new Param1<string>(serverMessage), true, PlayerBase.Cast(man).GetIdentity());
			}
		}
		delete players;
	}
////___________________________________________________________________////


///////////////////////////////////////////////////////////////////////////
////                            ON CLIENT                              ////
	static void NotificationFromType(PlayerBase recipient, int type, int duration, bool isCountdown, int zoneIndex = -1) {
		switch (type) {
			case PVEZ_NotificationTypes.NOTIF_ZONE_ENTER:
				ZoneNotification(recipient, 5, false, zoneIndex);
				break;
			case PVEZ_NotificationTypes.NOTIF_ZONE_EXIT:
				ZoneNotification(recipient, 5, false);
				break;
			case PVEZ_NotificationTypes.NOTIF_EXIT_COUNTDOWN:
				CountdownNotification(recipient);
				break;
			case PVEZ_NotificationTypes.NOTIF_LB_PERSONAL:
				LawbreakerPersonalNotification(recipient);
				break;
		}
	}

	static void NotificationFromString(PlayerBase player, string msg) {
		player.m_Hud.ShowPVEZNotification(msg, 10, false);
	}

	static void ZoneNotification(PlayerBase player, int duration, bool isCountdown, int zoneIndex = -1) {
		if (!player)
			return;
		
		string msg = "";
		autoptr PVEZ_Zone zone = g_Game.pvez_Zones.GetByIndex(zoneIndex);

		//On entering the zone
		if (zone) {
			msg = FormatMessage(PVEZ_NotificationTypes.NOTIF_ZONE_ENTER, zone.Name);
		}
		//On leaving the zone
		else {
			msg = FormatMessage(PVEZ_NotificationTypes.NOTIF_ZONE_EXIT);
		}
		
		if (g_Game.pvez_Config.GENERAL.Use_UI_Notifications && player.m_Hud)
			player.m_Hud.ShowPVEZNotification(msg, duration, isCountdown);
		else
			player.MessageStatus(msg);
	}

	static void CountdownNotification(PlayerBase player) {
		if (g_Game.pvez_Config.GENERAL.Use_UI_Notifications) {
			string msg = FormatMessage(PVEZ_NotificationTypes.NOTIF_EXIT_COUNTDOWN);
			int countdownTime = g_Game.pvez_Config.GENERAL.Exit_Zone_Countdown;
			player.m_Hud.ShowPVEZNotification(msg, countdownTime, true);
		}
	}

	static void LawbreakerPersonalNotification(PlayerBase lb) {
		string msg = FormatMessage(PVEZ_NotificationTypes.NOTIF_LB_PERSONAL);
		if (g_Game.pvez_Config.GENERAL.Use_UI_Notifications)
			lb.m_Hud.ShowPVEZNotification(msg, 10, false);
		else
			lb.MessageImportant(msg);
	}

	static string FormatMessage(int type, string str1 = "", string str2 = "", string str3 = "") {
		string result = "";
		switch (type) {
			case PVEZ_NotificationTypes.NOTIF_ZONE_ENTER:
				if (g_Game.pvez_Config.GENERAL.Custom_Enter_Zone_Message != string.Empty)
					result = g_Game.pvez_Config.GENERAL.Custom_Enter_Zone_Message;
				else {
					if (g_Game.pvez_Config.GENERAL.Mode == PVEZ_MODE_PVE_ZONES)
						result = "#pvez_entr_msg_pve";
					else
						result = "#pvez_entr_msg_pvp";
				}
				if (g_Game.pvez_Config.GENERAL.Add_Zone_Name_To_Message && str1 != string.Empty)
					result = result + " " + str1;
				break;
			case PVEZ_NotificationTypes.NOTIF_ZONE_EXIT:
				if (g_Game.pvez_Config.GENERAL.Custom_Exit_Zone_Message != string.Empty)
					result = g_Game.pvez_Config.GENERAL.Custom_Exit_Zone_Message;
				else {
					if (g_Game.pvez_Config.GENERAL.Mode == PVEZ_MODE_PVE_ZONES)
						result = "#pvez_exit_msg_pve";
					else
						result = "#pvez_exit_msg_pvp";
				}
				break;
			case PVEZ_NotificationTypes.NOTIF_EXIT_COUNTDOWN:
				if (g_Game.pvez_Config.GENERAL.Custom_Exit_Zone_Countdown_Message != string.Empty)
					result = g_Game.pvez_Config.GENERAL.Custom_Exit_Zone_Countdown_Message;
				else {
					if (g_Game.pvez_Config.GENERAL.Mode == PVEZ_MODE_PVE_ZONES)
						result = "#pvez_cdwn_msg_pve";
					else
						result = "#pvez_cdwn_msg_pvp";
				}
				break;
			case PVEZ_NotificationTypes.NOTIF_LB_PERSONAL:
				if (g_Game.pvez_Config.LAWBREAKERS_SYSTEM.Custom_Message_for_the_Lawbreaker != string.Empty)
					result = g_Game.pvez_Config.LAWBREAKERS_SYSTEM.Custom_Message_for_the_Lawbreaker;
				else
					result = "#pvez_lb_personal";
				break;
			case PVEZ_NotificationTypes.NOTIF_LB_SERVERWIDE:
				if (g_Game.pvez_Config.LAWBREAKERS_SYSTEM.Custom_Crime_Notification_Message_Format != string.Empty) {
					result = g_Game.pvez_Config.LAWBREAKERS_SYSTEM.Custom_Crime_Notification_Message_Format;
					result.Replace("1", str1);
					result.Replace("2", str2);
					result.Replace("3", str3);
				}
				else
					result = str1 + " " + "#pvez_lb_serverwide1" + " " + str2 + "#pvez_lb_serverwide2" + " " + str3;
				break;
		}
		return result;
	}

	static void IconUpdate(PlayerBase player, bool isInPVP, bool isLawbreaker, int zone) {
		GetGame().RPCSingleParam(player, PVEZ_RPC.UPDATE_ICON_ON_CLIENT, new Param3<bool, bool, int>(isInPVP, isLawbreaker, zone), true, player.GetIdentity());
	}
////__________________________________________________________________////
}