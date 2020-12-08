//////////////////////////////////////////////////////////////////////////
//---------------------- Config management class -----------------------//
class PVEZ_Config : Managed {
	
	//--------------------PVEZ_Config.JSON sections:--------------------//
	autoptr ref PVEZ_GeneralSettings GENERAL;
	autoptr ref PVEZ_DamageSettings DAMAGE;
	autoptr ref PVEZ_LawbreakersSettings LAWBREAKERS_SYSTEM;
	autoptr ref PVEZ_MapSettings MAP;
	autoptr ref PVEZ_AirdropSettings AIRDROP_ZONES;
	int version = 0;
	//__________________________________________________________________//

	void PVEZ_Config() {
		if (GetGame().IsServer()) {
			Init();
			LoadFromJson();
		}
		else {
			if (FileExist(PVEZ_CONFIG_FOLDER)) {
				DeleteFile(PVEZ_ADMINS_TXT);
				DeleteFile(PVEZ_CONFIG_JSON);
				DeleteFile(PVEZ_LAWBREAKERS_ROSTER_JSON);
				DeleteFile(PVEZ_ZONES_JSON);
				DeleteFile(PVEZ_CONFIG_FOLDER);
			}
		}
	}

	void Init() {
		// Create mod folder in the server profile folder (if it doesn't exist yet).
		MakeDirectory(PVEZ_CONFIG_FOLDER);
		// Create admins.txt if doesn't exist.
		if (!FileExist(PVEZ_ADMINS_TXT)) {
			FileHandle file = OpenFile(PVEZ_ADMINS_TXT, FileMode.WRITE);
			if (file != 0) {
				FPrintln(file, "# Write admins UIDs (44 digits, not the SteamIDs) below. Each one at a new line.");
				CloseFile(file);
			}
		}

		GENERAL = new ref PVEZ_GeneralSettings;
		DAMAGE = new ref PVEZ_DamageSettings;
		LAWBREAKERS_SYSTEM = new ref PVEZ_LawbreakersSettings;
		MAP = new ref PVEZ_MapSettings;
		AIRDROP_ZONES = new ref PVEZ_AirdropSettings;
	}

	void LoadFromJson() {
		if (FileExist(PVEZ_CONFIG_JSON)) {
			// Read Json file and fullfill this instance with the data stored in the Json
			if (!PVEZ_JsonFileLoader<PVEZ_Config>.JsonLoadFile(PVEZ_CONFIG_JSON, this)) {
				// Read process failed.
				// Try to read Json as an old config.
				UpgradeToGUIVersion();
			}
		}

		// Update the Json to add new properties to the config file.
		// The existing Json contains old properties only, while this instance could be initialized with new properties,
		// and then during the file read process mutual properties get values from the file, but the file doesn't know about new properties added through the code here.
		// In the save file process, new properties will be added to the file along with mutual ones.
		SaveToJson();
	}

	void SaveToJson() {
		JsonFileLoader<PVEZ_Config>.JsonSaveFile(PVEZ_CONFIG_JSON, this);
	}

	void UpgradeToGUIVersion() {
		ref PVEZ_Config_Old oldConfig = new PVEZ_Config_Old;
		if (PVEZ_JsonFileLoader<PVEZ_Config_Old>.JsonLoadFile(PVEZ_CONFIG_JSON, oldConfig)) {
			// If read process success, then it's an old config.
			// Do upgrade.
			DeleteFile(PVEZ_LAWBREAKERS_ROSTER_JSON);
			if (oldConfig.version < 9) {
				Print("[PVEZ] :: Upgrading config to new version for Admin GUI update. Moving zones into a separate file.");
				if (!g_Game.pvez_Zones) {
					g_Game.pvez_Zones = new PVEZ_Zones;
					g_Game.pvez_Zones.Init();
					g_Game.pvez_Zones.allZones = oldConfig.ZONES;
					g_Game.pvez_Zones.SaveToJson();
				}

				switch (oldConfig.GENERAL.Mode) {
					case "PVP_ZONES":
						GENERAL.Mode = PVEZ_MODE_PVP_ZONES;
						break;
					case "PVE_ZONES":
						GENERAL.Mode = PVEZ_MODE_PVE_ZONES;
						break;
					case "PVE":
						GENERAL.Mode = PVEZ_MODE_PVE;
						break;
					case "PVP":
						GENERAL.Mode = PVEZ_MODE_PVP;
						break;
				}

				GENERAL.Update_Frequency = oldConfig.GENERAL.Update_Frequency;
				GENERAL.Show_Notifications = oldConfig.GENERAL.Show_Notifications;
				GENERAL.Use_UI_Notifications = oldConfig.GENERAL.Use_UI_Notifications;
				GENERAL.Add_Zone_Name_To_Message = oldConfig.GENERAL.Add_Zone_Name_To_Message;
				GENERAL.Exit_Zone_Countdown = oldConfig.GENERAL.Exit_Zone_Countdown;
				GENERAL.Week_Starts_On_Sunday = oldConfig.GENERAL.Week_Starts_On_Sunday;
				GENERAL.Custom_Enter_Zone_Message = oldConfig.GENERAL.Custom_Enter_Zone_Message;
				GENERAL.Custom_Exit_Zone_Message = oldConfig.GENERAL.Custom_Exit_Zone_Message;
				GENERAL.Custom_Exit_Zone_Countdown_Message = oldConfig.GENERAL.Custom_Exit_Zone_Countdown_Message;

				DAMAGE = oldConfig.DAMAGE;
				LAWBREAKERS_SYSTEM = oldConfig.LAWBREAKERS_SYSTEM;
				MAP = oldConfig.MAP;
				AIRDROP_ZONES = oldConfig.AIRDROP_ZONES;

				LAWBREAKERS_SYSTEM.Autoclear_Period_Amount = 14;
				LAWBREAKERS_SYSTEM.Autoclear_Period_Mode = 2;

				MAP.Lawbreakers_Markers.Show_Markers_On_Map = true;
						
				version = 0;
				Print("[PVEZ] :: Update is done.");
			}
		}
	}
}

class PVEZ_Config_Old : Managed {
	
	//--------------------PVEZ_Config.JSON sections:--------------------//
	autoptr ref PVEZ_GeneralSettings_Old GENERAL;
	autoptr ref PVEZ_DamageSettings DAMAGE;
	autoptr ref PVEZ_LawbreakersSettings LAWBREAKERS_SYSTEM;
	autoptr ref PVEZ_MapSettings MAP;
	autoptr ref PVEZ_AirdropSettings AIRDROP_ZONES;
	autoptr ref array<ref PVEZ_Zone> ZONES;
	int version = 8;
	//__________________________________________________________________//

	void PVEZ_Config_Old() {
		// On server only. Clients will receive a config ref via RPC when needed.
		Init();
		if (GetGame().IsServer())
			LoadFromJson();
	}

	void Init() {
		GENERAL = new ref PVEZ_GeneralSettings_Old;
		DAMAGE = new ref PVEZ_DamageSettings;
		LAWBREAKERS_SYSTEM = new ref PVEZ_LawbreakersSettings;
		MAP = new ref PVEZ_MapSettings;
		AIRDROP_ZONES = new ref PVEZ_AirdropSettings;
	}

	void LoadFromJson() {
		if (FileExist(PVEZ_CONFIG_JSON)) {
			// Read Json file and fullfill this instance with the data stored in the Json
			JsonFileLoader<PVEZ_Config_Old>.JsonLoadFile(PVEZ_CONFIG_JSON, this);
		}
	}
}

class PVEZ_JsonFileLoader<Class T> {
	protected static ref JsonSerializer m_Serializer = new JsonSerializer;

	static bool JsonLoadFile( string filename, out T data ) {
		if( FileExist( filename ) ) {
			string file_content;
			string line_content;
			string error;
			
			FileHandle handle = OpenFile( filename, FileMode.READ );
			if ( handle == 0 )
				return true;
			
			while ( FGets( handle,  line_content ) >= 0 )
				file_content += line_content;
			
			CloseFile( handle );
			
			if( !m_Serializer )
				m_Serializer = new JsonSerializer;
			
			if( !m_Serializer.ReadFromString( data, file_content, error ) )
				return false;
		}
		return true;
	}
}