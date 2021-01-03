/////////////////////////////////////////////////////////////////////////////////////////////////////
//                                      DEBUG STUFF.                                               //
//                          If <#define pvezdebug is uncommented,                                  //
//           player will be flagged as a lawbreaker on killing zombies in PvE area                 //
//_________________________________________________________________________________________________//
#define pvezdebug;


class PVEZ_KillManager : Managed {

	static void OnPlayerKilled(PlayerBase victim, EntityAI killer, EntityAI weapon, int weaponType, bool killedInLaw) {

		if (killer == victim) return;

		if (killedInLaw) {
			if (g_Game.pvez_Bounties.Enabled) {
				if (victim.pvez_PlayerStatus.GetIsLawbreaker() && !PlayerBase.Cast(killer).pvez_PlayerStatus.GetIsLawbreaker())
					g_Game.pvez_Bounties.RewardPlayer(Man.Cast(killer), victim.GetDisplayName());
			}
			return;
		}

		// Both grenades and firearms return <IsWeapon()=true>, and we need grenades to be processed separately.
		// So, check if <weapon> is inherited from <Grenade_Base> first, and weapons will go later.
		switch (weaponType) {
			case PVEZ_DAMAGE_SOURCE_TYPE_EXPLOSIVE:
				if (g_Game.pvez_Config.LAWBREAKERS_SYSTEM.Declare_a_Lawbreaker_When_Killed_a_Player_in_PVE_Area.On_Explosive_Kill)
					RegisterMurder(killer, victim, weapon);
				break;
			case PVEZ_DAMAGE_SOURCE_TYPE_WEAPON:
				if (killer && weapon) {
					if (g_Game.pvez_Config.LAWBREAKERS_SYSTEM.Declare_a_Lawbreaker_When_Killed_a_Player_in_PVE_Area.On_Weaponary_Kill)
						RegisterMurder(killer, victim, weapon);
				}
				break;
			case PVEZ_DAMAGE_SOURCE_TYPE_VEHICLE:
				if (g_Game.pvez_Config.LAWBREAKERS_SYSTEM.Declare_a_Lawbreaker_When_Killed_a_Player_in_PVE_Area.On_Vehicle_Kill)
					RegisterMurder(killer, victim, weapon);
				break;
			case PVEZ_DAMAGE_SOURCE_TYPE_FISTS:
				if (g_Game.pvez_Config.LAWBREAKERS_SYSTEM.Declare_a_Lawbreaker_When_Killed_a_Player_in_PVE_Area.On_Fist_Fight_Kill)
					RegisterMurder(killer, victim, weapon);
				break;
#ifdef pvezdebug
			case PVEZ_DAMAGE_SOURCE_TYPE_CLAWS:
				RegisterMurder(killer, victim, weapon);
				break;
#endif
		}
	}

	static void RegisterMurder(EntityAI killerEntity, EntityAI victimEntity, EntityAI weapon) {
		autoptr PlayerBase killer = PlayerBase.Cast(killerEntity);
		if (killer) {
			autoptr PlayerBase victim = PlayerBase.Cast(victimEntity);
			if (victim)
				killer.pvez_PlayerStatus.SetLawbreaker(true);
		}
#ifdef pvezdebug
		autoptr ZombieBase killerZ = ZombieBase.Cast(killerEntity);
		if (killerZ)
			killerZ.SetLawbreaker(true);
		else {
			autoptr ZombieBase victimZ = ZombieBase.Cast(victimEntity);
			if (victimZ && killer)
				killer.pvez_PlayerStatus.SetLawbreaker(true);
		}
#endif
		// Show server wide notification
		string crimeNotificationText = "Killer:" + killerEntity.GetDisplayName() + " has killed player " + victimEntity.GetDisplayName() + ". Weapon: " + weapon.GetDisplayName();
		PVEZ_Notifications.NotificationToAll(killerEntity, victimEntity, weapon);
		// and print in server logs (with addition of date & time)
		string time = " (" + PVEZ_StaticFunctions.GetFormattedDateTime() + ").";
		Print("PVEZ :: " + crimeNotificationText + time);
	}

#ifdef pvezdebug
	static void OnZombieKilled(ZombieBase victim, EntityAI killer, EntityAI weapon, int weaponType, bool killedInLaw) {
		
		//if (killedInLaw)
		//	g_Game.pvez_Bounties.RewardPlayer(Man.Cast(killer), victim.GetDisplayName());

		switch (weaponType) {
			case PVEZ_DAMAGE_SOURCE_TYPE_EXPLOSIVE:
				if (g_Game.pvez_Config.LAWBREAKERS_SYSTEM.Declare_a_Lawbreaker_When_Killed_a_Player_in_PVE_Area.On_Explosive_Kill)
					RegisterMurder(killer, victim, weapon);
				break;
			case PVEZ_DAMAGE_SOURCE_TYPE_WEAPON:
				if (killer && weapon) {
					if (g_Game.pvez_Config.LAWBREAKERS_SYSTEM.Declare_a_Lawbreaker_When_Killed_a_Player_in_PVE_Area.On_Weaponary_Kill)
						RegisterMurder(killer, victim, weapon);
				}
				break;
			case PVEZ_DAMAGE_SOURCE_TYPE_VEHICLE:
				if (g_Game.pvez_Config.LAWBREAKERS_SYSTEM.Declare_a_Lawbreaker_When_Killed_a_Player_in_PVE_Area.On_Vehicle_Kill)
					RegisterMurder(killer, victim, weapon);
				break;
			case PVEZ_DAMAGE_SOURCE_TYPE_FISTS:
				if (g_Game.pvez_Config.LAWBREAKERS_SYSTEM.Declare_a_Lawbreaker_When_Killed_a_Player_in_PVE_Area.On_Fist_Fight_Kill)
					RegisterMurder(killer, victim, weapon);
				break;
			case PVEZ_DAMAGE_SOURCE_TYPE_CLAWS:
				break;
		}
	}
#endif
}