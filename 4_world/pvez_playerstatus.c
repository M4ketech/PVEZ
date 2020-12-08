class PVEZ_PlayerStatus : Managed {
	
	protected PlayerBase player;
	// A flag for the status "is already leaving a zone but there's a countdown".
	protected bool exitDelayed;
	protected float exitCounter;

	protected bool IsInZone;
	protected bool IsInPVP;
	protected bool IsLawbreaker;
	bool GetIsInPVP() { return IsInPVP; }
	bool GetIsLawbreaker() { return IsLawbreaker; }
	bool GetIsInZone() { return IsInZone; }
	
	void PVEZ_PlayerStatus(PlayerBase p) {
		player = p;

		if (GetGame().IsServer()) {
			// Get and update player's lawbreaker status
			if (g_Game.pvez_LawbreakersRoster.Check(player.GetIdentity().GetId())) {
				SetLawbreaker(true);
			}
			// Set status by checking zone and mode
			else {
				int zone = g_Game.pvez_Zones.GetPlayerZoneIndex(player.GetPosition());
				Update(zone, true);
			}
		}
	}

	// bool forceUpdate - used to update status instantly (without a countdown) when the countdown is already over,
	// or when the player just connected to the server.
	void Update(int zone, bool forceUpdate) {
		if (IsLawbreaker) // for a lawbreaker it doesn't matter, they're in PVP all the time
			return;
		
		if (GetGame().IsClient())
			return;
		
		if (zone < 0 && !forceUpdate) {
			TryLeaveZone();
			return;
		}

		if (SetByMode(zone))
			PVEZ_Notifications.PersonalNotificationServer(player, PVEZ_NotificationTypes.NOTIF_ZONE_ENTER, 5, false, zone);
		else if (exitDelayed)
			PVEZ_Notifications.PersonalNotificationServer(player, PVEZ_NotificationTypes.NOTIF_ZONE_EXIT, 5, false);
		
		exitDelayed = false;
		exitCounter = 0;
		PVEZ_Notifications.IconUpdate(player, IsInPVP, IsLawbreaker, zone);
	}

	bool SetByMode(int zone) {
		bool valueZone;
		switch (g_Game.pvez_Config.GENERAL.Mode) {
			case PVEZ_MODE_PVP:
				valueZone = (zone >= 0);
				IsInPVP = true; //PVP always on
				break;
			case PVEZ_MODE_PVE:
				//valueZone = false;
				//IsInPVP = false; //PVP always off
				valueZone = (zone >= 0);
				IsInPVP = valueZone; //PVP is on in zones (PVP still should work within Airdrop zones)
				break;
			case PVEZ_MODE_PVP_ZONES:
				valueZone = (zone >= 0);
				IsInPVP = valueZone; //PVP is on in zones
				break;
			case PVEZ_MODE_PVE_ZONES:
				valueZone = (zone >= 0);
				IsInPVP = !valueZone; //PVP is on if not in a zone
				break;
		}
		if (valueZone && IsInZone != valueZone) {
			IsInZone = valueZone;
			return true;
		}
		return false;
	}

	// If a countdown timer is set in config, zone status change will be delayed, otherwise it will be changed instantly.
	void TryLeaveZone() {
		// Return if already not in the zone or already leaving zone (on countdown). Nothing to change.
		if (!IsInZone || exitDelayed)
			return;
		
		IsInZone = false;
		if (g_Game.pvez_Config.GENERAL.Exit_Zone_Countdown > 0) {
			exitDelayed = true;
			// RPC call to client to show UI countdown notification
			PVEZ_Notifications.PersonalNotificationServer(player, PVEZ_NotificationTypes.NOTIF_EXIT_COUNTDOWN, g_Game.pvez_Config.GENERAL.Exit_Zone_Countdown, true);
			
			// WRNG! The following approach doesn't work correctly after a long server life time (more than 4-5 hours without a restart).
			// Had to change it to a trigger update call from OnScheduledTick() in PlayerBase class.
			/*
			GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).CallLater(Update, g_Game.pvez_Config.GENERAL.Exit_Zone_Countdown * 1000, false, -1, true);
			GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).CallLater(PVEZ_Notifications.PersonalNotificationServer, g_Game.pvez_Config.GENERAL.Exit_Zone_Countdown * 1000, false, player, PVEZ_NotificationTypes.NOTIF_ZONE_EXIT, g_Game.pvez_Config.GENERAL.Exit_Zone_Countdown, true);
			*/
		}
		else {
			Update(-1, true);
		}
	}

	// This method is called from PlayerBase class.
	void OnScheduledTick(float deltaTime) {
		if (!exitDelayed)
			return;
		
		exitCounter += deltaTime;
		if (exitCounter >= g_Game.pvez_Config.GENERAL.Exit_Zone_Countdown) {
			Update(-1, true);
		}
	}

	void SetLawbreaker(bool value) {
		if (GetGame().IsServer()) {
			if (value) {
				// If the player is not in LB status yet, inform them
				if (IsLawbreaker != value) {
					PVEZ_Notifications.PersonalNotificationServer(player, PVEZ_NotificationTypes.NOTIF_LB_PERSONAL, 10, false);
				}
			}
			g_Game.pvez_LawbreakersRoster.Update(player.GetIdentity().GetId(), value);
			if (!value)
				g_Game.pvez_LawbreakersMarkers.RemoveMarker(player.GetIdentity().GetId());
			IsLawbreaker = value;
			
			// Update UI icon image & visibility on client
			GetGame().RPCSingleParam(player, PVEZ_RPC.UPDATE_ICON_ON_CLIENT, new Param3<bool, bool, PVEZ_Zone>(IsInPVP, value, NULL), true, player.GetIdentity());
		}
	}

	// @param EntityAI attacker - should be the root entity (player) returned from <PVEZ_DamageRedistributor.RegisterHit()>.
	bool PVEZ_IsPvpAttackAllowed(EntityAI attacker) {

#ifdef pvezdebug
		if (attacker && attacker.IsZombie()) {
			if (GetIsLawbreaker())
				return true;
			// if both fighters are in PVP zone:
			else if (GetIsInPVP() && ZombieBase.Cast(attacker).PVEZ_GetIsInPVP())
				return true;
			// if config allows PVP when at least one of them is in PVP zone:
			else if (g_Game.pvez_Config.DAMAGE.Allow_Damage_Between_PVP_and_PVE)
				return GetIsInPVP() || ZombieBase.Cast(attacker).PVEZ_GetIsInPVP();
			else
				return false;
		}
#endif
		// If this player is a lawbreaker:
		if (GetIsLawbreaker())
			return true;
		// if both fighters are in PVP zone:
		else if (GetIsInPVP() && PlayerBase.Cast(attacker).pvez_PlayerStatus.GetIsInPVP())
			return true;
		// if config allows PVP when at least one of them is in PVP zone:
		else if (g_Game.pvez_Config.DAMAGE.Allow_Damage_Between_PVP_and_PVE)
			return GetIsInPVP() || PlayerBase.Cast(attacker).pvez_PlayerStatus.GetIsInPVP();
		// if config unlocks PVP for a lawbreaker player:
		else if (PlayerBase.Cast(attacker).pvez_PlayerStatus.GetIsLawbreaker() && g_Game.pvez_Config.LAWBREAKERS_SYSTEM.Allow_Lawbreakers_To_Attack_Anywhere)
			return true;
		else
			return false;
	}
}