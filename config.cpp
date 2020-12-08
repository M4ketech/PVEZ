class CfgPatches
{
	class PVEZ
	{
		units[]={};
		weapons[]={};
		requiredVersion=0.1;
		requiredAddons[]=
		{
			"DZ_Data",
			"DZ_Gear_Navigation"
		};
	};
};
class CfgMods
{
	class PVEZ
	{
		dir="PVEZ";
		extra=0;
		type="mod";
		name="PVEZ";
		inputs="PVEZ/data/inputs.xml";
		overview="$STR_pvez_overview";
		dependencies[]=
		{
			"Game",
			"World",
			"Mission"
		};
		class defs
		{
			class gameScriptModule
			{
				value="";
				files[]=
				{
					"PVEZ/3_Game"
				};
			};
			class worldScriptModule
			{
				value="";
				files[]=
				{
					"PVEZ/4_World"
				};
			};
			class missionScriptModule
			{
				value="";
				files[]=
				{
					"PVEZ/5_Mission"
				};
			};
		};
	};
};
