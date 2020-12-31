modded class MissionGameplay {

	void MissionGameplay() {
		g_Game.PVEZ_Init();
	}

	override void OnUpdate(float timeslice) {
		super.OnUpdate(timeslice);

		if (GetUApi().GetInputByName("UAPVEZOpenAdminMenu").LocalPress()) {
			UIScriptedMenu menu = GetUIManager().GetMenu();

			if (!PlayerBase.Cast(GetGame().GetPlayer()).IsPVEZAdmin()) {
				//PlayerBase.Cast(GetGame().GetPlayer()).MessageStatus("PVEZ: You're not an admin. Don't touch this.");
				return;
			}
			if (!menu && !PlayerBase.Cast(GetGame().GetPlayer()).GetPVEZAdminMenu().GetLayoutRoot().IsVisible()) {
				GetUIManager().ShowScriptedMenu(PlayerBase.Cast(GetGame().GetPlayer()).GetPVEZAdminMenu(), NULL);
				PlayerControlDisable(INPUT_EXCLUDE_ALL);
			} else if (PlayerBase.Cast(GetGame().GetPlayer()).GetPVEZAdminMenu().GetLayoutRoot().IsVisible()) {
				// If the cursor is on some edit box then the user is typing some text, don't close the GUI
				if (!PlayerBase.Cast(GetGame().GetPlayer()).GetPVEZAdminMenu().IsTypingText()) {
					GetUIManager().HideScriptedMenu(PlayerBase.Cast(GetGame().GetPlayer()).GetPVEZAdminMenu());
					PlayerControlEnable(false);
				}
			}
		}

		if (GetUApi().GetInputByName("UAUIBack").LocalPress() && GetGame().GetPlayer() && PlayerBase.Cast(GetGame().GetPlayer()).GetPVEZAdminMenu().GetLayoutRoot().IsVisible()) {
			GetUIManager().HideScriptedMenu(PlayerBase.Cast(GetGame().GetPlayer()).GetPVEZAdminMenu());
			PlayerControlEnable(false);
		}
	}
}