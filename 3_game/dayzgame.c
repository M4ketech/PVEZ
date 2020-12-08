modded class DayZGame extends CGame {

	autoptr PVEZ_Config pvez_Config;
	autoptr PVEZ_Zones pvez_Zones;
	autoptr PVEZ_LawbreakersMarkers pvez_LawbreakersMarkers;
	autoptr PVEZ_LawbreakersRoster pvez_LawbreakersRoster;

	override void OnAfterCreate() {
		super.OnAfterCreate();

		GetCallQueue(CALL_CATEGORY_GAMEPLAY).CallLater(this.PVEZ_Init, 5000, false);
	}

	void PVEZ_Init() {
		pvez_Config = new PVEZ_Config;
		pvez_Zones = new PVEZ_Zones;
		pvez_LawbreakersMarkers = new PVEZ_LawbreakersMarkers;
		pvez_LawbreakersRoster = new PVEZ_LawbreakersRoster(pvez_Config);

		if (IsServer()) {
			pvez_Zones.Init();
		}
	}

	void PVEZ_SendConfigToClient(DayZPlayer dzp = NULL) {
		if (IsServer()) {
			// Send PVEZ config to the client via RPC
			Param1<ref PVEZ_Config> configData = new Param1<ref PVEZ_Config>(pvez_Config);
			if (dzp)
				RPCSingleParam(dzp, PVEZ_RPC.UPDATE_CONFIG_ON_CLIENT, configData, true, dzp.GetIdentity()); // to the given player
			else
				PVEZ_RPCForAllClients(PVEZ_RPC.UPDATE_CONFIG_ON_CLIENT, configData); // to all players
		}
	}

	void PVEZ_SendActiveZonesToClient(DayZPlayer dzp = NULL) {
		if (IsServer()) {
			Param1<array<ref PVEZ_Zone>> zonesData = new Param1<array<ref PVEZ_Zone>>(pvez_Zones.activeZones);
			if (dzp)
				RPCSingleParam(dzp, PVEZ_RPC.UPDATE_ZONES_ON_CLIENT, zonesData, true, dzp.GetIdentity());
			else
				PVEZ_RPCForAllClients(PVEZ_RPC.UPDATE_ZONES_ON_CLIENT, zonesData);
		}
	}

	void PVEZ_GetAdminStatus(DayZPlayer dzp) {
		if (IsServer()) {
			bool isAdmin = false;
			string uid = dzp.GetIdentity().GetId();

			FileHandle file_handle = OpenFile(PVEZ_ADMINS_TXT, FileMode.READ);
			string line_content;
			while (FGets(file_handle, line_content) > 0) {
				if (line_content == uid) {
					isAdmin = true;
					break;
				}
			}
			CloseFile(file_handle);
			
			Param1<bool> data = new Param1<bool>(isAdmin);
			RPCSingleParam(dzp, PVEZ_RPC.ADMIN_ACCESS_REQUEST, data, true, dzp.GetIdentity());
		}
	}

	void PVEZ_RPCForAllClients(int type, Param data) {
		if (GetGame().GetWorld()) {
			autoptr ref array<Man> players = new array<Man>;
			GetGame().GetWorld().GetPlayerList(players);
			if (players.Count() > 0) {
				foreach (Man p : players) {
					RPCSingleParam(p, type, data, true, p.GetIdentity());
				}
			}
		}
	}

	override void OnRPC(PlayerIdentity sender, Object target, int rpc_type, ParamsReadContext ctx) {
		super.OnRPC(sender, target, rpc_type, ctx);

		if (IsClient()) {
			switch (rpc_type) {
				case PVEZ_RPC.UPDATE_MARKERS_ON_CLIENT:
					// Assign markers array with the new data sent by server.
					Param1<array<ref PVEZ_LawbreakerMarker>> data2 = new Param1<array<ref PVEZ_LawbreakerMarker>>(NULL);
					ctx.Read(data2);
					pvez_LawbreakersMarkers.Sync(data2.param1);
					break;
			}
		}
		else {
			switch (rpc_type) {
				case PVEZ_RPC.ADMIN_UPDATE_CONFIG_ON_SERVER:
					Param1<ref PVEZ_Config> data4 = new Param1<ref PVEZ_Config>(NULL);
					ctx.Read(data4);
					pvez_Config = data4.param1;
					pvez_Config.SaveToJson();
					pvez_Zones.Init(); // Re-init zones to reactivate them if the week mode setting has been changed
					PVEZ_SendConfigToClient();
					break;
				case PVEZ_RPC.UPDATE_ZONES_ON_SERVER:
					Param1<array<ref PVEZ_Zone>> data5 = new Param1<array<ref PVEZ_Zone>>(NULL);
					ctx.Read(data5);
					pvez_Zones.allZones = data5.param1;
					pvez_Zones.SaveToJson();
					pvez_Zones.Init();
					break;
				case PVEZ_RPC.ADMIN_ZONES_DATA_REQUEST:
					Param1<array<ref PVEZ_Zone>> data6 = new Param1<array<ref PVEZ_Zone>>(pvez_Zones.allZones);
					RPCSingleParam(DayZPlayer.Cast(target), PVEZ_RPC.ADMIN_ZONES_DATA_REQUEST, data6, true, DayZPlayer.Cast(target).GetIdentity());
					break;
				case PVEZ_RPC.ADMIN_LAWBREAKERS_DATA_REQUEST:
					autoptr array<Man> players = new array<Man>;
					GetGame().GetWorld().GetPlayerList(players);
					Param2<array<ref PVEZ_Lawbreaker>, array<Man>> data7 = new Param2<array<ref PVEZ_Lawbreaker>, array<Man>>(pvez_LawbreakersRoster.lbDataBase, players);
					RPCSingleParam(DayZPlayer.Cast(target), PVEZ_RPC.ADMIN_LAWBREAKERS_DATA_REQUEST, data7, true, DayZPlayer.Cast(target).GetIdentity());
					break;
				case PVEZ_RPC.ADMIN_UPDATE_LAWBREAKERS_ON_SERVER:
					Param1<array<ref PVEZ_Lawbreaker>> data8 = new Param1<array<ref PVEZ_Lawbreaker>>(NULL);
					ctx.Read(data8);
					pvez_LawbreakersRoster.lbDataBase = data8.param1;
					pvez_LawbreakersRoster.SaveToJson();
					pvez_LawbreakersRoster = new PVEZ_LawbreakersRoster(pvez_Config);
					// Send updated database back to admin client (the sender of this RPC call):
					autoptr array<Man> players2 = new array<Man>;
					GetGame().GetWorld().GetPlayerList(players2);
					Param2<array<ref PVEZ_Lawbreaker>, array<Man>> data9 = new Param2<array<ref PVEZ_Lawbreaker>, array<Man>>(pvez_LawbreakersRoster.lbDataBase, players2);
					RPCSingleParam(DayZPlayer.Cast(target), PVEZ_RPC.ADMIN_LAWBREAKERS_DATA_REQUEST, data9, true, DayZPlayer.Cast(target).GetIdentity());
					break;
			}
		}
	}
}