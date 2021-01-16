#ifdef PVEZ_DEBUGMODE

modded class ZombieBase extends DayZInfected {
	private EntityAI weapon;
	
	private bool IsInPVP = false;
	private bool IsLawbreaker = false;
	private EntityAI DmgInitiator; //root entity (player, infected, animal) of the previous damage source
	private int weaponType;	
	private bool death;
	private autoptr PVEZ_DamageRedistributor pvez_DamageRedistributor;

	override void Init() {
		super.Init();
		pvez_DamageRedistributor = new PVEZ_DamageRedistributor(this);
	}

	override void EEHitBy(TotalDamageResult damageResult, int damageType, EntityAI source, int component, string dmgZone, string ammo, vector modelPos, float speedCoef) {
		super.EEHitBy(damageResult, damageType, source, component, dmgZone, ammo, modelPos, speedCoef);

		if (GetGame().IsServer() || !GetGame().IsMultiplayer()) {
			if (!IsAlive() || g_Game.pvez_Config.GENERAL.Mode == PVEZ_MODE_PVP)
				return;

			pvez_DamageRedistributor.RegisterHit(this, source, weaponType, false);
			if (!pvez_DamageRedistributor.LastHitWasAllowed() && damageResult) {
				if (g_Game.pvez_Config.DAMAGE.Restore_Target_Health) {
					pvez_DamageRedistributor.HealDamageReceived(damageResult, dmgZone, false);
				}
				pvez_DamageRedistributor.ProcessDamageReflection(weaponType, damageResult.GetDamage("", ""));
			}
		}
	}

	override bool EvaluateDeathAnimation(EntityAI pSource, string pComponent, string pAmmoType, out int pAnimType, out float pAnimHitDir) {
		bool result = super.EvaluateDeathAnimation(pSource, pComponent, pAmmoType, pAnimType, pAnimHitDir);

		if (GetGame().IsServer() || !GetGame().IsMultiplayer()) {
			if (!IsAlive() && !death) {
				EntityAI killerEntity = pSource;
				pvez_DamageRedistributor.RegisterDeath(this, killerEntity, weaponType);
				SetLawbreaker(false);
			}
		}
		death = true;
		return true;
	}

	bool GetIsInPVP() {
		if (GetGame().IsServer() || !GetGame().IsMultiplayer()) {
			int zoneIndex = g_Game.pvez_Zones.GetPlayerZoneIndex(GetPosition());
			switch (g_Game.pvez_Config.GENERAL.Mode) {
				case PVEZ_MODE_PVP:
					IsInPVP = true; //PVP always on
					break;
				case PVEZ_MODE_PVE:
					IsInPVP = false; //PVP always off
					//valueZone = (zone >= 0);
					//IsInPVP = valueZone; //PVP is on in zones (PVP still should work within Airdrop zones)
					break;
				case PVEZ_MODE_PVP_ZONES:
					IsInPVP = (zoneIndex >= 0);
					break;
				case PVEZ_MODE_PVE_ZONES:
					IsInPVP = (zoneIndex < 0);
					break;
			}
		}
		return IsInPVP;
	}

	void SetLawbreaker(bool value) {
		if (GetGame().IsServer() || !GetGame().IsMultiplayer()) {
			g_Game.pvez_LawbreakersRoster.Update(this, value, PVEZ_Date.Now());
			g_Game.pvez_LawbreakersMarkers.Update(this, value);
			IsLawbreaker = value;
		}
	}

	bool PVEZ_IsPvpAttackAllowed() {
		EntityAI source = pvez_DamageRedistributor.GetDirectDmgInitiator();
		// If this player is a lawbreaker:
		if (IsLawbreaker)
			return true;
		// if both fighters are in PVP zone:
		else if (GetIsInPVP() && PlayerBase.Cast(source).pvez_PlayerStatus.GetIsInPVP())
			return true;
		// if config allows PVP when at least one of them is in PVP zone:
		else if (g_Game.pvez_Config.DAMAGE.Allow_Damage_Between_PVP_and_PVE)
			return GetIsInPVP() || PlayerBase.Cast(source).pvez_PlayerStatus.GetIsInPVP();
		else
			return false;
	}
}
#endif