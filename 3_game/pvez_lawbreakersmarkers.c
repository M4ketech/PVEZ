class PVEZ_LawbreakerMarker : Managed {
	vector Position;
	string GUID;
	string LBName;

	void PVEZ_LawbreakerMarker(vector pos, PlayerIdentity identity, bool showName) {
		Position = pos;
		GUID = identity.GetId();
		if (showName) {
			LBName = identity.GetName();
		}
	}
}

class PVEZ_LawbreakersMarkers : Managed {
	autoptr array<ref PVEZ_LawbreakerMarker> markers;

	void PVEZ_LawbreakersMarkers() {
		markers = new array<ref PVEZ_LawbreakerMarker>;
	}

	void Get(out array<ref PVEZ_LawbreakerMarker> result) {
		result = markers;
	}

	void AddMarker(vector position, PlayerIdentity identity) {
		if (g_Game.pvez_Config.MAP.Lawbreakers_Markers.Update_Frequency == 0)
			return;
		
		autoptr PVEZ_LawbreakerMarker newMarker = new PVEZ_LawbreakerMarker(
			position,
			identity,
			g_Game.pvez_Config.MAP.Lawbreakers_Markers.Show_Name);

		if (markers.Count() == 0) {
			markers.Insert(newMarker);
		}
		else {
			// Check if marker for the given player ID already exist. If so, just update its position.
			bool found = false;
			foreach (PVEZ_LawbreakerMarker marker : markers) {
				if (marker.GUID == newMarker.GUID) {
					found = true;
					marker.Position = newMarker.Position;
					break;
				}
			}
			if (!found) {
				markers.Insert(newMarker);
			}
		}
		Sync();
	}

	void RemoveMarker(string uid) {
		if (markers.Count() == 0)
			return;
		
		for (int i = 0; i < markers.Count(); i++) {
			if (markers[i].GUID == uid) {
				markers.Remove(i);
				break;
			}
		}
		Sync();
	}

	void UpdateMarkersPositions(array<Man> activePlayers) {
		if (markers.Count() == 0 || activePlayers.Count() == 0)
			return;
		
		foreach (Man man : activePlayers) {
			foreach (PVEZ_LawbreakerMarker marker : markers) {
				if (marker.GUID == man.GetIdentity().GetId()) {
					float x = man.GetPosition()[0];
					float z = man.GetPosition()[2];
					if (g_Game.pvez_Config.MAP.Lawbreakers_Markers.Approximate_Location) {
						// Add approximate offset for the marker position:
						int maxOffset = g_Game.pvez_Config.MAP.Lawbreakers_Markers.Approximate_Location_Max_Offset;
						x += Math.RandomInt(-maxOffset, maxOffset);
						z += Math.RandomInt(-maxOffset, maxOffset);
					}
					marker.Position = Vector(x, 0, z);
				}
			}
		}
		Sync();
	}

	void Sync(array<ref PVEZ_LawbreakerMarker> newMarkers = NULL) {
		if (GetGame().IsServer()) {
			Param1<array<ref PVEZ_LawbreakerMarker>> markersData = new Param1<array<ref PVEZ_LawbreakerMarker>>(markers);
			g_Game.PVEZ_RPCForAllClients(PVEZ_RPC.UPDATE_LAWBREAKERS_MARKERS, markersData);
		}
		else {
			markers = newMarkers;
			if (markers.Count() > 0) {
				// Delete own marker from array if config doesn't allow a lawbreaker to see his own marker
				if (!g_Game.pvez_Config.MAP.Lawbreakers_Markers.Lawbreakers_Can_See_Their_Own_Markers) {
					string id = GetGame().GetPlayer().GetIdentity().GetId();
					for (int i = 0; i < markers.Count(); i++) {
						if (markers[i].GUID == id) {
							markers.Remove(i);
							break;
						}
					}
				}
			}
		}
	}
}