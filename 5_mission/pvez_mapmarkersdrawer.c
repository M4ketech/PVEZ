class PVEZ_MapMarkersDrawer : Managed {

	private static autoptr array<ref PVEZ_LawbreakerMarker> markers;
	private static autoptr array<ref PVEZ_Zone> zones;
	private static autoptr array<vector> zoneCirclePoints = new array<vector>;

	static void LoadPVEZMarkers(MapWidget mapWidget) {

		// mapWidget could be NULL when this method is called from VPP's very first Init(), so, make sure it's initialized before doing anything.
		if (!mapWidget)
			return;

		markers = g_Game.pvez_LawbreakersMarkers.markers;
		zones = g_Game.pvez_Zones.activeZones;
		
		// Drawing zones
		zoneCirclePoints = new array<vector>;
		// Get color from config
		auto color = ARGB(
				255,
				g_Game.pvez_Config.MAP.Zones_Border_Color.R,
				g_Game.pvez_Config.MAP.Zones_Border_Color.G,
				g_Game.pvez_Config.MAP.Zones_Border_Color.B);
		
		if (zones && zones.Count() > 0) {
			for (int j = 0; j < zones.Count(); j++) {
				vector zonePos = zones[j].GetVectorPos();
				// Border circle
				if (zones[j].ShowBorderOnMap) {
					zoneCirclePoints.Clear();
					GetCirclePoints(zones[j], zoneCirclePoints);
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
				mapWidget.AddUserMark(markers[i].Position, label + " " + markers[i].Name, ARGB(255, 255, 90, 0), "\\PVEZ\\GUI\\Textures\\skull_ca.paa");
			}
		}
	}

	static void GetCirclePoints(PVEZ_Zone zone, out array<vector> result) {
		// How many points we gotta draw. Depends on the circle radius. The bigger the circle, the more points.
		float L = 2 * Math.PI * zone.Radius; // circle lenght
		L = L / 10; // how much points to draw if we draw one point for every 10 meters on the zone border line

		float slice = 2 * Math.PI / L;
		for (int i = 0; i < L; i++) {
			float angle = slice * i;
			float newX = zone.GetVectorPos()[0] + zone.Radius * Math.Cos(angle);
			float newY = zone.GetVectorPos()[2] + zone.Radius * Math.Sin(angle);
			vector point = Vector(newX, 0, newY);
			if (!IntersectsWithOtherCircles(point, zone))
				result.Insert(point);
		}
	}

	static bool IntersectsWithOtherCircles(vector point, PVEZ_Zone zone) {
		autoptr PVEZ_Zone zoneToCheck;
		for (int i = 0; i < zones.Count(); i++) {
			zoneToCheck = zones[i];
			// Ignore the zones with disabled borders and if it's the same zone, also if the zones have the same center and radius
			if (!zoneToCheck.ShowBorderOnMap || zoneToCheck == zone || (zoneToCheck.GetVectorPos() == zone.GetVectorPos() && zoneToCheck.Radius == zone.Radius))
				continue;
			
			if (vector.DistanceSq(zoneToCheck.GetVectorPos(), point) <= zoneToCheck.Radius * zoneToCheck.Radius)
				return true;
		}
		return false;
	}
}