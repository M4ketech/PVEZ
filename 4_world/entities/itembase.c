modded class ItemBase extends InventoryItem {

	// Can't make this private, DayZ yells at me if I try.
	PlayerBase pvez_owner;
	
	// When the damage source is a player <isDamageAllowed> value will depend on the owner PVP status.
	private bool isDamageAllowed = true;

	// Used by <PVEZ_DamageRedistributor.RegisterHit(source, DmgInitiator, weaponType))>
	private autoptr PVEZ_DamageRedistributor pvez_DamageRedistributor;
	private int weaponType;

	void ItemBase() { }

	// This method is called on item when someone takes or drops the item.
	// We use it to update <configProtectClothing> when some player takes the item.
	override void OnItemLocationChanged(EntityAI old_owner, EntityAI new_owner) {
		super.OnItemLocationChanged(old_owner, new_owner);

		if (new_owner) {
			if (new_owner.IsMan())
				pvez_owner = PlayerBase.Cast(new_owner);
			else
				pvez_owner = PlayerBase.Cast(new_owner.GetHierarchyRootPlayer());
			
			pvez_DamageRedistributor = new PVEZ_DamageRedistributor(pvez_owner);
		}
		else {
			pvez_owner = NULL;
			pvez_DamageRedistributor = NULL;
			isDamageAllowed = true;
		}
	}

	override void EEHitBy(TotalDamageResult damageResult, int damageType, EntityAI source, int component, string dmgZone, string ammo, vector modelPos, float speedCoef) {
		
		isDamageAllowed = true;

		if (pvez_owner && pvez_owner.PVEZ_ShouldProtectClothing()) {
			if (IsClothing() || IsContainer()) {
				// Just like in <PlayerBase>, check the source root.
				// We do it before the base method call to set <isDamageAllowed> first, because the base method executes <DamageItemsInCargo()>.
				pvez_DamageRedistributor.RegisterHit(pvez_owner, source, weaponType, false);
				if (!pvez_DamageRedistributor.LastHitWasAllowed()) {
					isDamageAllowed = false;
					pvez_DamageRedistributor.ReflectDamageBack(weaponType, damageResult.GetDamage("", ""));
				}
			}

			super.EEHitBy(damageResult, damageType, source, component, dmgZone, ammo, modelPos, speedCoef);

			// Restore item health after the hit.
			if (!isDamageAllowed)
				this.AddHealth("", "", damageResult.GetDamage("", ""));
		}
		else
			// If <Protect_Clothing_And_Cargo> in PVEZ_Config is false (or the item has no player owner), then just call the base method.
			super.EEHitBy(damageResult, damageType, source, component, dmgZone, ammo, modelPos, speedCoef);
	}

	override bool DamageItemInCargo(float damage) {
		// Call the base method only when damage is allowed.
		// This could cause incompatibility with mods which use this base method too.
		// If PVEZ is loaded on server before those other mods, they could not work.
		// If PVEZ is loaded after them, items will receive damage.
		if (isDamageAllowed)
			return super.DamageItemInCargo(damage);
		else
			return false;
	}

	/*
	    Just an example of getting a certain inventory slot:
	    EntityAI item = (PlayerBase)owner.GetInventory().FindAttachment(InventorySlots.HIPS);

	    InventorySlots are: "Slot_Shoulder",
					        "Slot_Melee",
					        "Slot_Vest",
					        "Slot_Body",
					        "Slot_Hips",
					        "Slot_Legs",
					        "Slot_Back",
					        "Slot_Headgear",
					        "Slot_Mask",
					        "Slot_Eyewear",
					        "Slot_Gloves",
					        "Slot_Feet",
					        "Slot_Armband"
	    */
	/*
	EntityAI class has the method that returns all equipments with cargo:
	    array<EntityAI> GetAttachmentsWithCargo()
	    {
	        return m_AttachmentsWithCargo;
	    }
	*/
}