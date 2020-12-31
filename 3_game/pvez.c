class PVEZ : Managed {

	autoptr ref PVEZ_Config config;
	autoptr ref PVEZ_Zones zones;
	autoptr ref PVEZ_LawbreakersRoster lbRoster;
	autoptr ref PVEZ_Bounties bounties;

	void PVEZ() {
		config = new ref PVEZ_Config;
		zones = new ref PVEZ_Zones;
		lbRoster = new ref PVEZ_LawbreakersRoster(config);
		bounties = new ref PVEZ_Bounties;
	}
}