const string PVEZ_SERVER_ROOT = "$profile:";
const string PVEZ_LOG_PREFIX = "[PVEZ] :: ";
const string PVEZ_CONFIG_FOLDER = PVEZ_SERVER_ROOT + "PVEZ_Config";
const string PVEZ_CONFIG_JSON = PVEZ_CONFIG_FOLDER + "/" + "PVEZ_Config.json";
const string PVEZ_LAWBREAKERS_ROSTER_JSON = PVEZ_CONFIG_FOLDER + "/" + "PVEZ_Lawbreakers.json";
const string PVEZ_ZONES_JSON = PVEZ_CONFIG_FOLDER + "/" + "PVEZ_Zones.json";
const string PVEZ_ADMINS_TXT = PVEZ_CONFIG_FOLDER + "/" + "Admins.txt";

const int PVEZ_DAMAGE_SOURCE_TYPE_EXPLOSIVE = 0;
const int PVEZ_DAMAGE_SOURCE_TYPE_WEAPON = 1;
const int PVEZ_DAMAGE_SOURCE_TYPE_VEHICLE = 2;
const int PVEZ_DAMAGE_SOURCE_TYPE_FISTS = 3;
const int PVEZ_DAMAGE_SOURCE_TYPE_SELF = 4;
const int PVEZ_DAMAGE_SOURCE_TYPE_CLAWS = 5;

const string PVEZ_DEFAULT_ENTER_ZONE_MESSAGE = "You have entered PVP zone";
const string PVEZ_DEFAULT_EXIT_ZONE_MESSAGE = "Left PVP zone";
const int PVEZ_DEFAULT_EXIT_ZONE_COUNTDOWN = 10;
const string PVEZ_DEFAULT_EXIT_ZONE_COUNTDOWN_MESSAGE = "You are going to leave PVP zone in";

const int PVEZ_MODE_PVP_ZONES = 0;
const int PVEZ_MODE_PVE_ZONES = 1;
const int PVEZ_MODE_PVE = 2;
const int PVEZ_MODE_PVP = 3;