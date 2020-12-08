////////////////////////////////////////////////////////////////////////////////
//   The follwing classes will be sections and settings in PVEZ_Config.json.  //
//    The PVEZ_ConfigManager class will use these to initialize the config.   //
////////////////////////////////////////////////////////////////////////////////

//____________________________________________________________________________//
//----------------------------------GENERAL-----------------------------------//
class PVEZ_GeneralSettings : Managed {
	int Mode = 0; // Other options are: "PVE_ZONES", "PVP", "PVE".
	float Update_Frequency = 5;
	bool Show_Notifications = true;
	bool Use_UI_Notifications = true;
	bool Add_Zone_Name_To_Message = true;
	float Exit_Zone_Countdown = 10;
	bool Week_Starts_On_Sunday = false;
	string Custom_Enter_Zone_Message = "";
	string Custom_Exit_Zone_Message = "";
	string Custom_Exit_Zone_Countdown_Message = "";

	void PVEZ_GeneralSettings() { }
}

class PVEZ_GeneralSettings_Old : Managed {
	string Mode = "PVP_ZONES"; // Other options are: "PVE_ZONES", "PVP", "PVE".
	float Update_Frequency = 5;
	bool Show_Notifications = true;
	bool Use_UI_Notifications = true;
	bool Add_Zone_Name_To_Message = true;
	float Exit_Zone_Countdown = 10;
	bool Week_Starts_On_Sunday = false;
	string Custom_Enter_Zone_Message = "";
	string Custom_Exit_Zone_Message = "";
	string Custom_Exit_Zone_Countdown_Message = "";

	void PVEZ_GeneralSettings() { }
}

//____________________________________________________________________________//
//--------------------------DAMAGE PROTECTION RULES---------------------------//
class PVEZ_DamageSettings : Managed {
	bool Restore_Target_Health = true;
	bool Protect_Clothing_And_Cargo = true;
	bool Allow_Damage_Between_PVP_and_PVE = false;
	autoptr ref PVEZ_ReflectDamageRules Damage_Types_Sent_Back_To_The_Attacker;

	void PVEZ_DamageSettings() {
		Damage_Types_Sent_Back_To_The_Attacker = new PVEZ_ReflectDamageRules;
	}
}

class PVEZ_ReflectDamageRules : Managed {
	bool Weapon_Damage = false;
	bool Explosive_Damage = false;
	bool Vehicle_Damage = false;
	bool Fist_Fight_Damage = false;

	void PVEZ_ReflectDamageRules() { }
}

//_____________________________________________________________________________//
//-------------------------------LAWBREAKERS-----------------------------------//
class PVEZ_LawbreakersSettings : Managed {
	autoptr ref PVEZ_LawbreakersRules Declare_a_Lawbreaker_When_Killed_a_Player_in_PVE_Area;

	bool Server_Wide_Message_About_Lawbreaker = true;
	string Custom_Crime_Notification_Message_Format = "";
	string Custom_Message_for_the_Lawbreaker = "";
	
	bool Auto_Clear_Lawbreakers_Data = false;
	int Autoclear_Period_Amount = 14;
	int Autoclear_Period_Mode = 2;
	bool Allow_Lawbreakers_To_Attack_Anywhere = false;
	bool Pardon_On_Death_From_Any_Source = false;
	
	void PVEZ_LawbreakersSettings() {
		Declare_a_Lawbreaker_When_Killed_a_Player_in_PVE_Area = new PVEZ_LawbreakersRules;
	}
}

class PVEZ_LawbreakersRules : Managed {
	bool On_Weaponary_Kill = true;
	bool On_Explosive_Kill = true;
	bool On_Vehicle_Kill = false;
	bool On_Fist_Fight_Kill = false;

	void PVEZ_LawbreakersRules() { }
}

//__________________________________________________________________________//
//-------------------------------MAP MARKERS--------------------------------//
class PVEZ_ZoneColor : Managed {
	int A = 200;
	int R = 255;
	int G = 0;
	int B = 0;
	
	void PVEZ_ZoneColor() { }
}

class PVEZ_MapSettings : Managed {
	bool Show_Player_Marker = true;
	string Custom_Player_Marker_Text = "";
	autoptr ref PVEZ_ZoneColor Zones_Border_Color;

	autoptr ref PVEZ_LawbreakersMapMarkers Lawbreakers_Markers;

	void PVEZ_MapSettings() {
		Zones_Border_Color = new PVEZ_ZoneColor;
		Lawbreakers_Markers = new PVEZ_LawbreakersMapMarkers;
	}
}

class PVEZ_LawbreakersMapMarkers : Managed {
	bool Show_Markers_On_Map = true;
	float Update_Frequency = 800; // 800 seconds = 15 minutes
	string Custom_Lawbreaker_Label = "";
	bool Show_Name = true;
	bool Lawbreakers_Can_See_Their_Own_Markers = false;
	bool Approximate_Location = true;
	int Approximate_Location_Max_Offset = 200;

	void PVEZ_LawbreakersMapMarkers() { }
}

// Settings for Airdrop mod compatibility submod (PVEZAirdropZones)
class PVEZ_AirdropSettings : Managed {
	float Radius = 150;
	string Zone_Name = "Airdrop";
	bool Show_On_Map = true;
	bool Show_Name_On_Map = true;
}