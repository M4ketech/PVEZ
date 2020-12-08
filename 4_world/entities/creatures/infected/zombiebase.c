/////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                 //
//                                      DEBUG STUFF.                                               //
//                 To test PvP damage and lawbreakers system on infected.                          //
//          They are treated as players if the <#define pvezdebug> is uncommented.                 //
//_________________________________________________________________________________________________//
#define pvezdebug;


#ifdef pvezdebug
modded class ZombieBase extends DayZInfected {
	private EntityAI weapon;

	private bool IsInPVP = false;
	bool PVEZ_GetIsInPVP() { return IsInPVP; }
	private bool IsLawbreaker = false;
	private EntityAI DmgInitiator; //root entity (player, infected, animal) of the previous damage source
	private int weaponType;	
	private bool death;
	private autoptr PVEZ_DamageRedistributor pvez_DamageRedistributor;

	override void Init() {
		super.Init();
		pvez_DamageRedistributor = new PVEZ_DamageRedistributor(this);
	}

	override bool EvaluateDeathAnimation(EntityAI pSource, string pComponent, string pAmmoType, out int pAnimType, out float pAnimHitDir) {
		bool result = super.EvaluateDeathAnimation(pSource, pComponent, pAmmoType, pAnimType, pAnimHitDir);

		if (!IsAlive() && !death) {
			EntityAI killerEntity = pSource;
			pvez_DamageRedistributor.RegisterDeath(this, killerEntity, weaponType);
		}
		death = true;
		return true;
	}

	bool PVEZ_IsPvpAttackAllowed() {
		EntityAI source = pvez_DamageRedistributor.GetDirectDmgInitiator();
		// If this player is a lawbreaker:
		if (IsLawbreaker)
			return true;
		// if both fighters are in PVP zone:
		else if (IsInPVP && PlayerBase.Cast(source).pvez_PlayerStatus.GetIsInPVP())
			return true;
		// if config allows PVP when at least one of them is in PVP zone:
		else if (g_Game.pvez_Config.DAMAGE.Allow_Damage_Between_PVP_and_PVE)
			return IsInPVP || PlayerBase.Cast(source).pvez_PlayerStatus.GetIsInPVP();
		else
			return false;
	}
}
#endif