class PVEZ_Lawbreaker : Managed {
	string UID;
	autoptr array<string> Recent_Character_Names;
	int Murder_Count;
	autoptr ref PVEZ_DateTime Latest_Murder_Time;
	bool Is_Currently_Outlaw;

	void PVEZ_Lawbreaker(string uid, string name, int crimesCount, bool outlaw = true, PVEZ_DateTime lastMurderTime = NULL) {
		UID = uid;
		Recent_Character_Names = new array<string>;
		Recent_Character_Names.Insert(name);
		Murder_Count = crimesCount;
		Latest_Murder_Time = lastMurderTime;
		Is_Currently_Outlaw = outlaw;
	}
}

class PVEZ_LawbreakersRoster : Managed {

	autoptr array<ref PVEZ_Lawbreaker> lbDataBase;

	// This is monitored by MissionServer to update labreaker statuses on players when this roster is re-initiated.
	bool updated;

	void PVEZ_LawbreakersRoster(PVEZ_Config c) {
		lbDataBase = new array<ref PVEZ_Lawbreaker>;
		if (GetGame().IsServer()) {
			LoadFromJson();
			RemoveOutdated(c);
			SaveToJson();
			updated = true;
		}
	}

	void SaveToJson() {
		JsonFileLoader<array<ref PVEZ_Lawbreaker>>.JsonSaveFile(PVEZ_LAWBREAKERS_ROSTER_JSON, lbDataBase);
	}
	
	void LoadFromJson() {
		// Read Json file and fullfill the <roster> array with the data stored in the Json
		if (FileExist(PVEZ_LAWBREAKERS_ROSTER_JSON)) {
			JsonFileLoader<array<ref PVEZ_Lawbreaker>>.JsonLoadFile(PVEZ_LAWBREAKERS_ROSTER_JSON, lbDataBase);
		}
	}

	void RemoveOutdated(PVEZ_Config config) {
		// Call it again after 60 secs to update LBs on server.
		GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).CallLater(this.RemoveOutdated, 60000, false, g_Game.pvez_Config);
		
		if (lbDataBase.Count() == 0)
			return;
		
		autoptr PVEZ_DateTime lastMurderTime;
		for (int i = 0; i < lbDataBase.Count(); i++) {
			lastMurderTime = lbDataBase[i].Latest_Murder_Time;
			
			if (!lastMurderTime) {
				lbDataBase.Remove(i);
				continue;
			}

			if (config.LAWBREAKERS_SYSTEM.Auto_Clear_Lawbreakers_Data) {
				// Check the player's last murder date. If it was long ago, forgive the player.
				int count;
				switch (config.LAWBREAKERS_SYSTEM.Autoclear_Period_Mode) {
					case 0: // Mode 0 counts minutes
						count = PVEZ_TimeHelper.CountMinutesBetweenDates(lastMurderTime, PVEZ_DateTime.Now());
						break;
					case 1: // Mode 1 counts hours
						count = PVEZ_TimeHelper.CountHoursBetweenDates(lastMurderTime, PVEZ_DateTime.Now());
						break;
					case 2: // Mode 2 counts days
						count = PVEZ_TimeHelper.CountDaysBetweenDates(lastMurderTime, PVEZ_DateTime.Now());
						break;
				}
				if (count >= config.LAWBREAKERS_SYSTEM.Autoclear_Period_Amount)
					lbDataBase.Remove(i);
			}
		}
		updated = true;
	}

	// This can be called by other scripts on lawful kills
	void Save(string murdererUID, string murdererName, PVEZ_DateTime time) {
		// Initialize the roster first
		if (!lbDataBase)
			LoadFromJson();
		
		if (lbDataBase.Count() == 0) {
			lbDataBase.Insert(new ref PVEZ_Lawbreaker(murdererUID, murdererName, 1, true, time));
			return;
		}

		// Looking for the murderer in the roster
		bool lawbreakerFound;
		foreach (PVEZ_Lawbreaker lb : lbDataBase) {
			if (lb.UID == murdererUID) {
				lawbreakerFound = true;

				// Checking if the murderer's current character name is already stored in roster
				bool nameAlreadyKnown;
				for (int i = 0; i < lb.Recent_Character_Names.Count(); i++) {
					if (lb.Recent_Character_Names[i] == murdererName) {
						nameAlreadyKnown = true;
						break;
					}
				}
				// if the name is not in the roster yet
				if (!nameAlreadyKnown) {
					// Putting new name in the murderer's data (only 3 the most recent names are stored)
					if (lb.Recent_Character_Names.Count() < 3)
						lb.Recent_Character_Names.Insert(murdererName);
					else {
						lb.Recent_Character_Names.Remove(0);
						lb.Recent_Character_Names.Insert(murdererName);
					}
				}
				lb.Murder_Count++;
				lb.Latest_Murder_Time = time;
				lb.Is_Currently_Outlaw = true;
				break;
			}
		}
		if (!lawbreakerFound) {
			// Adding new lawbreaker data to the roster
			lbDataBase.Insert(new ref PVEZ_Lawbreaker(murdererUID, murdererName, 1, true, time));
		}

		// Update Json because of the array changes
		//SaveToJson();
	}

	void Update(string murdererUID, bool isLawbreaker) {
		// Initialize the roster first
		if (!lbDataBase)
			LoadFromJson();

		if (lbDataBase.Count() == 0)
			return;
		
		// Looking for the murderer in the roster
		foreach (PVEZ_Lawbreaker lb : lbDataBase) {
			if (lb.UID == murdererUID) {
				lb.Is_Currently_Outlaw = isLawbreaker;
				break;
			}
		}
		// Update Json because of the array changes
		//SaveToJson();
	}

	bool Check(string playerId) {
		// Initialize the roster first
		if (!lbDataBase)
			LoadFromJson();
		
		if (lbDataBase.Count() == 0)
			return false;

		foreach (PVEZ_Lawbreaker lb : lbDataBase) {
			if (lb.UID == playerId) {
				return lb.Is_Currently_Outlaw;
			}
		}
		return false;
	}
}