class PVEZ_BountyItemData : Managed {
	string className;
	string displayName;
	int amount;

	void PVEZ_BountyItemData(string cName, string dName, int a) {
		className = cName;
		displayName = dName;
		amount = a;
	}
}

class PVEZ_Bounties : Managed {

	bool Enabled;
	autoptr array<ref PVEZ_BountyItemData> Items;

	void PVEZ_Bounties() {
		if (GetGame().IsServer()) {
			LoadFromJson();
			SaveToJson();
		}
	}

	void LoadFromJson() {
		if (FileExist(PVEZ_BOUNTIES_JSON)) {
			JsonFileLoader<PVEZ_Bounties>.JsonLoadFile(PVEZ_BOUNTIES_JSON, this);
		}
	}

	void SaveToJson() {
		JsonFileLoader<PVEZ_Bounties>.JsonSaveFile(PVEZ_BOUNTIES_JSON, this);
	}

	void RewardPlayer(Man player, string lbName) {
		EntityAI item;
		bool failedToPutInInventory;
		string msg = "#pvez_bnty_msg1" + " " + lbName + " " + "#pvez_bnty_msg2";

		for (int i = 0; i < Items.Count(); i++) {
			msg = msg + "\n" + Items[i].displayName + " x" + Items[i].amount;
			for (int j = 0; j < Items[i].amount; j++) {
				item = player.GetInventory().CreateInInventory(Items[i].className);
				if (!item) {
					failedToPutInInventory = true;
					player.SpawnEntityOnGroundPos(Items[i].className, player.GetPosition());
				}
			}
		}
		if (failedToPutInInventory)
			msg = msg + "\n" + "#pvez_bnty_msg3";
		PVEZ_Notifications.PersonalNotificationServer(DayZPlayer.Cast(player), PVEZ_NotificationType.NOTIF_GENERIC, 15, false, msg);
	}
}