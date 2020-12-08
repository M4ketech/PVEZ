class PVEZ_MapMarkersDrawer : Managed {

	private static autoptr array<ref PVEZ_LawbreakerMarker> markers;
	private static autoptr array<ref PVEZ_Zone> zones;
	private static autoptr array<vector> zoneCirclePoints = new array<vector>;

	static void LoadPVEZMarkers(MapWidget mapWidget) {

		// mapWidget could be NULL when this method is called from VPP's very first Init(), so, make sure it's initialized before doing anything.
		if (!mapWidget)
			return;

		g_Game.pvez_LawbreakersMarkers.Get(markers);
		zones = g_Game.pvez_Zones.activeZones;
		
		// Drawing zones
		zoneCirclePoints = new array<vector>;
		// Get color from config
		auto color = ARGB(
				g_Game.pvez_Config.MAP.Zones_Border_Color.A,
				g_Game.pvez_Config.MAP.Zones_Border_Color.R,
				g_Game.pvez_Config.MAP.Zones_Border_Color.G,
				g_Game.pvez_Config.MAP.Zones_Border_Color.B);
		
		if (zones && zones.Count() > 0) {
			for (int j = 0; j < zones.Count(); j++) {
				vector zonePos = zones[j].GetVectorPos();
				// Border circle
				if (zones[j].ShowBorderOnMap) {
					zoneCirclePoints.Clear();
					GetCirclePoints(zones[j].Radius, zonePos, zoneCirclePoints);
					for (int k = 0; k < zoneCirclePoints.Count(); k++) {
						mapWidget.AddUserMark(zoneCirclePoints[k], "", color, "\\dz\\gear\\navigation\\data\\map_bush_ca.paa");
					}
				}
				// Name
				if (zones[j].ShowNameOnMap) {
					mapWidget.AddUserMark(zonePos, zones[j].Name, color, "");
				}
			}
		}

		// Draw lawbreakers markers
		if (markers.Count() > 0) {
			string label = "";
			if (g_Game.pvez_Config.MAP.Lawbreakers_Markers.Custom_Lawbreaker_Label != string.Empty)
				label = g_Game.pvez_Config.MAP.Lawbreakers_Markers.Custom_Lawbreaker_Label;
			else
				label = Widget.TranslateString("#pvez_lb_map_label");

			for (int i = 0; i < markers.Count(); i++) {
				mapWidget.AddUserMark(markers[i].Position, label + " " + markers[i].LBName, ARGB(255, 255, 90, 0), "\\PVEZ\\GUI\\Textures\\skull_ca.paa");
			}
		}
	}

	static void GetCirclePoints(float radius, vector center, out array<vector> result) {
		// How many points we gotta draw. Depends on the circle radius. The bigger the circle, the more points.
		float L = 2 * Math.PI * radius; // circle lenght
		L = L / 10; // how much points to draww if we draw a point for every 10 meters on zone border line

		float slice = 2 * Math.PI / L;
		for (int i = 0; i < L; i++) {
			float angle = slice * i;
			float newX = center[0] + radius * Math.Cos(angle);
			float newY = center[2] + radius * Math.Sin(angle);
			vector p = Vector(newX, 0, newY);
			if (!IntersectsWithOtherCircles(p, center))
				result.Insert(p);
		}
	}

	static bool IntersectsWithOtherCircles(vector v, vector center) {
		autoptr PVEZ_Zone zoneToCheck;
		for (int i = 0; i < zones.Count(); i++) {
			zoneToCheck = zones[i];
			if (zoneToCheck.GetVectorPos() == center)
				continue;
			
			if (vector.DistanceSq(zoneToCheck.GetVectorPos(), v) <= zoneToCheck.Radius * zoneToCheck.Radius)
				return true;
		}
		return false;
	}
}