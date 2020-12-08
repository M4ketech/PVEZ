class PVEZ_AdminConsoleGUI extends UIScriptedMenu {

	autoptr ref PVEZ_Zone selectedZone;
	autoptr ref PVEZ_Lawbreaker selectedLawbreaker;
	autoptr ref array<Man> playersOnServer;

	protected bool isTypingText;

	// header buttons
	protected ButtonWidget btnGeneral;
	protected ButtonWidget btnZones;
	protected ButtonWidget btnLawbreakers;

	protected Widget descriptionFrame;
	protected RichTextWidget descriptionTitle;

	/// SETTINGS page
	protected Widget SettingsRootPanel;
	// general settings
	protected XComboBoxWidget modeValue;
	protected Widget modePanel;
	protected EditBoxWidget freqValue;
	protected Widget freqPanel;
	protected CheckBoxWidget showNotifsValue;
	protected Widget showNotifsPanel;
	protected CheckBoxWidget showUIValue;
	protected Widget showUIPanel;
	protected CheckBoxWidget showNameInNotifsValue;
	protected Widget showNameInNotifsPanel;
	protected EditBoxWidget exitDelayValue;
	protected Widget exitDelayPanel;
	protected EditBoxWidget customMsgEnterValue;
	protected Widget customMsgEnterPanel;
	protected EditBoxWidget customMsgExitValue;
	protected Widget customMsgExitPanel;
	protected EditBoxWidget customMsgExitCountdownValue;
	protected Widget customMsgExitCountdownPanel;
	// damage settings
	protected CheckBoxWidget healTargetValue;
	protected Widget healTargetPanel;
	protected CheckBoxWidget healCargoValue;
	protected Widget healCargoPanel;
	protected CheckBoxWidget allowDmgBeyondZoneValue;
	protected Widget allowDmgBeyondZonePanel;
	protected CheckBoxWidget reflectDmgWeaponsValue;
	protected CheckBoxWidget reflectDmgExplosivesValue;
	protected CheckBoxWidget reflectDmgVehiclesValue;
	protected CheckBoxWidget reflectDmgFistsValue;
	protected Widget reflectDmgPanel;
	// lawbreakers settings
	protected CheckBoxWidget lbKillTypeWeaponsValue;
	protected CheckBoxWidget lbKillTypeExplosivesValue;
	protected CheckBoxWidget lbKillTypeVehiclesValue;
	protected CheckBoxWidget lbKillTypeFistsValue;
	protected Widget lbKillTypesPanel;
	protected CheckBoxWidget lbServerMessageValue;
	protected Widget lbServerMessagePanel;
	protected CheckBoxWidget lbAttackAnywhereValue;
	protected Widget lbAttackAnywherePanel;
	protected CheckBoxWidget lbPardonOnAnyDeathValue;
	protected Widget lbPardonOnAnyDeathPanel;
	protected CheckBoxWidget lbAutoclearValue;
	protected Widget lbAutoclearPanel;
	protected EditBoxWidget lbAutoclearPeriodValue;
	protected XComboBoxWidget lbAutoclearPeriodMode;
	protected Widget lbAutoclearPeriodPanel;
	protected EditBoxWidget customMsgLbServerWideValue;
	protected Widget customMsgLbServerWidePanel;
	protected EditBoxWidget customMsgLbPersonalValue;
	protected Widget customMsgLbPersonalPanel;
	// map settings
	protected CheckBoxWidget mapShowLbMarkersValue;
	protected Widget mapShowLbMarkersPanel;
	protected EditBoxWidget mapLbMarkersUpdateFreqValue;
	protected Widget mapLbMarkersUpdateFreqPanel;
	protected CheckBoxWidget mapLbMarkersUseApproximateValue;
	protected Widget mapLbMarkersUseApproximatePanel;
	protected EditBoxWidget mapLbMarkersApproximateOffsetValue;
	protected Widget mapLbMarkersApproximateOffsetPanel;
	protected CheckBoxWidget mapLbCanSeeTheirOwnMarkerValue;
	protected Widget mapLbCanSeeTheirOwnMarkerPanel;
	// apply button
	protected ButtonWidget btnSettingsApply;

	/// ZONES page
	protected Widget ZonesRootPanel;
	protected TextListboxWidget zonesList;
	protected ButtonWidget btnCreateNewZone;
	// selected zone side panel
	protected Widget ZoneSettingsPanel;
	protected EditBoxWidget zoneNameValue;
	protected EditBoxWidget zoneCoordXValue;
	protected EditBoxWidget zoneCoordZValue;
	protected ButtonWidget btnZoneSetCurrentPosition;
	protected EditBoxWidget zoneRadiusValue;
	protected CheckBoxWidget zoneShowBorderValue;
	protected CheckBoxWidget zoneShowNameValue;
	protected EditBoxWidget zoneDaysValue;
	protected CheckBoxWidget zoneDaysModeValue;
	protected EditBoxWidget zoneHourStartValue;
	protected EditBoxWidget zoneHourEndValue;
	protected TextWidget serverTimeValue;
	// selected zone actions
	protected ButtonWidget btnZoneApply;
	protected ButtonWidget btnZoneDelete;

	/// LAWBREAKERS page
	protected Widget LawbreakersRootPanel;
	protected TextListboxWidget lbPlayersList;
	// selected LB side panel
	protected Widget lbDataPanel;
	protected RichTextWidget lbUIDValue;
	protected RichTextWidget lbNamesValue;
	protected TextWidget lbCountValue;
	protected TextWidget lbLatestMurderTimeValue;
	protected CheckBoxWidget lbStatusValue;
	protected PlayerPreviewWidget lbPreview;
	protected ButtonWidget btnLbApply;
	protected ButtonWidget btnLbDelete;

	bool IsTypingText() {
		return isTypingText;
	}

	override Widget Init() {
		layoutRoot = GetGame().GetWorkspace().CreateWidgets("PVEZ\\GUI\\layouts\\AdminConsole.layout");

		/// SETTINGS, ZONES, LAWBREAKERS header buttons
		btnGeneral = ButtonWidget.Cast(layoutRoot.FindAnyWidget("btnGeneral"));
		btnZones = ButtonWidget.Cast(layoutRoot.FindAnyWidget("btnZones"));
		btnLawbreakers = ButtonWidget.Cast(layoutRoot.FindAnyWidget("btnLawbreakers"));

		descriptionFrame = Widget.Cast(layoutRoot.FindAnyWidget("descriptionFrame"));
		descriptionTitle = RichTextWidget.Cast(layoutRoot.FindAnyWidget("descriptionTitle"));

		/// SETTINGS page
		SettingsRootPanel = Widget.Cast(layoutRoot.FindAnyWidget("SettingsRootPanel"));
		SettingsRootPanel.Show(true);
		// general
		modeValue = XComboBoxWidget.Cast(layoutRoot.FindAnyWidget("modeValue"));
		modePanel = Widget.Cast(layoutRoot.FindAnyWidget("modePanel"));
		freqValue = EditBoxWidget.Cast(layoutRoot.FindAnyWidget("freqValue"));
		freqPanel = Widget.Cast(layoutRoot.FindAnyWidget("freqPanel"));
		showNotifsValue = CheckBoxWidget.Cast(layoutRoot.FindAnyWidget("showNotifsValue"));
		showNotifsPanel = Widget.Cast(layoutRoot.FindAnyWidget("showNotifsPanel"));
		showUIValue = CheckBoxWidget.Cast(layoutRoot.FindAnyWidget("showUIValue"));
		showUIPanel = Widget.Cast(layoutRoot.FindAnyWidget("showUIPanel"));
		showNameInNotifsValue = CheckBoxWidget.Cast(layoutRoot.FindAnyWidget("showNameInNotifsValue"));
		showNameInNotifsPanel = Widget.Cast(layoutRoot.FindAnyWidget("showNameInNotifsPanel"));
		exitDelayValue = EditBoxWidget.Cast(layoutRoot.FindAnyWidget("exitDelayValue"));
		exitDelayPanel = Widget.Cast(layoutRoot.FindAnyWidget("exitDelayPanel"));
		customMsgEnterValue = EditBoxWidget.Cast(layoutRoot.FindAnyWidget("customMsgEnterValue"));
		customMsgEnterPanel = Widget.Cast(layoutRoot.FindAnyWidget("customMsgEnterPanel"));
		customMsgExitValue = EditBoxWidget.Cast(layoutRoot.FindAnyWidget("customMsgExitValue"));
		customMsgExitPanel = Widget.Cast(layoutRoot.FindAnyWidget("customMsgExitPanel"));
		customMsgExitCountdownValue = EditBoxWidget.Cast(layoutRoot.FindAnyWidget("customMsgExitCountdownValue"));
		customMsgExitCountdownPanel = Widget.Cast(layoutRoot.FindAnyWidget("customMsgExitCountdownPanel"));
		// damage
		healTargetValue = CheckBoxWidget.Cast(layoutRoot.FindAnyWidget("healTargetValue"));
		healTargetPanel = Widget.Cast(layoutRoot.FindAnyWidget("healTargetPanel"));
		healCargoValue = CheckBoxWidget.Cast(layoutRoot.FindAnyWidget("healCargoValue"));
		healCargoPanel = Widget.Cast(layoutRoot.FindAnyWidget("healCargoPanel"));
		allowDmgBeyondZoneValue = CheckBoxWidget.Cast(layoutRoot.FindAnyWidget("allowDmgBeyondZoneValue"));
		allowDmgBeyondZonePanel = Widget.Cast(layoutRoot.FindAnyWidget("allowDmgBeyondZonePanel"));
		reflectDmgWeaponsValue = CheckBoxWidget.Cast(layoutRoot.FindAnyWidget("reflectDmgWeaponsValue"));
		reflectDmgExplosivesValue = CheckBoxWidget.Cast(layoutRoot.FindAnyWidget("reflectDmgExplosivesValue"));
		reflectDmgVehiclesValue = CheckBoxWidget.Cast(layoutRoot.FindAnyWidget("reflectDmgVehiclesValue"));
		reflectDmgFistsValue = CheckBoxWidget.Cast(layoutRoot.FindAnyWidget("reflectDmgFistsValue"));
		reflectDmgPanel = Widget.Cast(layoutRoot.FindAnyWidget("reflectDmgPanel"));
		// lawbreakers
		lbKillTypeWeaponsValue = CheckBoxWidget.Cast(layoutRoot.FindAnyWidget("lbKillTypeWeaponsValue"));
		lbKillTypeExplosivesValue = CheckBoxWidget.Cast(layoutRoot.FindAnyWidget("lbKillTypeExplosivesValue"));
		lbKillTypeVehiclesValue = CheckBoxWidget.Cast(layoutRoot.FindAnyWidget("lbKillTypeVehiclesValue"));
		lbKillTypeFistsValue = CheckBoxWidget.Cast(layoutRoot.FindAnyWidget("lbKillTypeFistsValue"));
		lbKillTypesPanel = Widget.Cast(layoutRoot.FindAnyWidget("lbKillTypesPanel"));
		lbServerMessageValue = CheckBoxWidget.Cast(layoutRoot.FindAnyWidget("lbServerMessageValue"));
		lbServerMessagePanel = Widget.Cast(layoutRoot.FindAnyWidget("lbServerMessagePanel"));
		lbAttackAnywhereValue = CheckBoxWidget.Cast(layoutRoot.FindAnyWidget("lbAttackAnywhereValue"));
		lbAttackAnywherePanel = Widget.Cast(layoutRoot.FindAnyWidget("lbAttackAnywherePanel"));
		lbPardonOnAnyDeathValue = CheckBoxWidget.Cast(layoutRoot.FindAnyWidget("lbPardonOnAnyDeathValue"));
		lbPardonOnAnyDeathPanel = Widget.Cast(layoutRoot.FindAnyWidget("lbPardonOnAnyDeathPanel"));
		lbAutoclearValue = CheckBoxWidget.Cast(layoutRoot.FindAnyWidget("lbAutoclearValue"));
		lbAutoclearPanel = Widget.Cast(layoutRoot.FindAnyWidget("lbAutoclearPanel"));
		lbAutoclearPeriodValue = EditBoxWidget.Cast(layoutRoot.FindAnyWidget("lbAutoclearPeriodValue"));
		lbAutoclearPeriodMode = XComboBoxWidget.Cast(layoutRoot.FindAnyWidget("lbAutoclearPeriodMode"));
		lbAutoclearPeriodPanel = Widget.Cast(layoutRoot.FindAnyWidget("lbAutoclearPeriodPanel"));
		customMsgLbServerWideValue = EditBoxWidget.Cast(layoutRoot.FindAnyWidget("customMsgLbServerWideValue"));
		customMsgLbServerWidePanel = Widget.Cast(layoutRoot.FindAnyWidget("customMsgLbServerWidePanel"));
		customMsgLbPersonalValue = EditBoxWidget.Cast(layoutRoot.FindAnyWidget("customMsgLbPersonalValue"));
		customMsgLbPersonalPanel = Widget.Cast(layoutRoot.FindAnyWidget("customMsgLbPersonalPanel"));
		// map
		mapShowLbMarkersValue = CheckBoxWidget.Cast(layoutRoot.FindAnyWidget("mapShowLbMarkersValue"));
		mapShowLbMarkersPanel = Widget.Cast(layoutRoot.FindAnyWidget("mapShowLbMarkersPanel"));
		mapLbMarkersUpdateFreqValue = EditBoxWidget.Cast(layoutRoot.FindAnyWidget("mapLbMarkersUpdateFreqValue"));
		mapLbMarkersUpdateFreqPanel = Widget.Cast(layoutRoot.FindAnyWidget("mapLbMarkersUpdateFreqPanel"));
		mapLbMarkersUseApproximateValue = CheckBoxWidget.Cast(layoutRoot.FindAnyWidget("mapLbMarkersUseApproximateValue"));
		mapLbMarkersUseApproximatePanel = Widget.Cast(layoutRoot.FindAnyWidget("mapLbMarkersUseApproximatePanel"));
		mapLbMarkersApproximateOffsetValue = EditBoxWidget.Cast(layoutRoot.FindAnyWidget("mapLbMarkersApproximateOffsetValue"));
		mapLbMarkersApproximateOffsetPanel = Widget.Cast(layoutRoot.FindAnyWidget("mapLbMarkersApproximateOffsetPanel"));
		mapLbCanSeeTheirOwnMarkerValue = CheckBoxWidget.Cast(layoutRoot.FindAnyWidget("mapLbCanSeeTheirOwnMarkerValue"));
		mapLbCanSeeTheirOwnMarkerPanel = Widget.Cast(layoutRoot.FindAnyWidget("mapLbCanSeeTheirOwnMarkerPanel"));
		// settings Apply button
		btnSettingsApply = ButtonWidget.Cast(layoutRoot.FindAnyWidget("btnSettingsApply"));

		/// ZONES page
		ZonesRootPanel = Widget.Cast(layoutRoot.FindAnyWidget("ZonesRootPanel"));
		ZonesRootPanel.Show(false);
		// list of zones
		zonesList = TextListboxWidget.Cast(layoutRoot.FindAnyWidget("zonesList"));
		btnCreateNewZone = ButtonWidget.Cast(layoutRoot.FindAnyWidget("btnCreateNewZone"));
		// selected zone side panel
		ZoneSettingsPanel = Widget.Cast(layoutRoot.FindAnyWidget("ZoneSettingsPanel"));
		ZoneSettingsPanel.Show(false);
		zoneNameValue = EditBoxWidget.Cast(layoutRoot.FindAnyWidget("zoneNameValue"));
		zoneCoordXValue = EditBoxWidget.Cast(layoutRoot.FindAnyWidget("zoneCoordXValue"));
		zoneCoordZValue = EditBoxWidget.Cast(layoutRoot.FindAnyWidget("zoneCoordZValue"));
		btnZoneSetCurrentPosition = ButtonWidget.Cast(layoutRoot.FindAnyWidget("btnZoneSetCurrentPosition"));
		zoneRadiusValue = EditBoxWidget.Cast(layoutRoot.FindAnyWidget("zoneRadiusValue"));
		zoneShowBorderValue = CheckBoxWidget.Cast(layoutRoot.FindAnyWidget("zoneShowBorderValue"));
		zoneShowNameValue = CheckBoxWidget.Cast(layoutRoot.FindAnyWidget("zoneShowNameValue"));
		zoneDaysValue = EditBoxWidget.Cast(layoutRoot.FindAnyWidget("zoneDaysValue"));
		zoneDaysModeValue = CheckBoxWidget.Cast(layoutRoot.FindAnyWidget("zoneDaysModeValue"));
		zoneHourStartValue = EditBoxWidget.Cast(layoutRoot.FindAnyWidget("zoneHourStartValue"));
		zoneHourEndValue = EditBoxWidget.Cast(layoutRoot.FindAnyWidget("zoneHourEndValue"));
		serverTimeValue = TextWidget.Cast(layoutRoot.FindAnyWidget("serverTimeValue"));
		// selected zone actions
		btnZoneApply = ButtonWidget.Cast(layoutRoot.FindAnyWidget("btnZoneApply"));
		btnZoneDelete = ButtonWidget.Cast(layoutRoot.FindAnyWidget("btnZoneDelete"));

		/// LAWBREAKERS page
		LawbreakersRootPanel = Widget.Cast(layoutRoot.FindAnyWidget("LawbreakersRootPanel"));
		LawbreakersRootPanel.Show(false);
		// list of LBs
		lbPlayersList = TextListboxWidget.Cast(layoutRoot.FindAnyWidget("lbPlayersList"));
		// selected LB side panel
		lbDataPanel = Widget.Cast(layoutRoot.FindAnyWidget("lbDataPanel"));
		lbDataPanel.Show(false);
		lbUIDValue = RichTextWidget.Cast(layoutRoot.FindAnyWidget("lbUIDValue"));
		lbNamesValue = RichTextWidget.Cast(layoutRoot.FindAnyWidget("lbNamesValue"));
		lbCountValue = TextWidget.Cast(layoutRoot.FindAnyWidget("lbCountValue"));
		lbLatestMurderTimeValue = TextWidget.Cast(layoutRoot.FindAnyWidget("lbLatestMurderTimeValue"));
		lbStatusValue = CheckBoxWidget.Cast(layoutRoot.FindAnyWidget("lbStatusValue"));
		lbPreview = PlayerPreviewWidget.Cast(layoutRoot.FindAnyWidget("lbPreview"));
		// selected LB actions
		btnLbApply = ButtonWidget.Cast(layoutRoot.FindAnyWidget("btnLbApply"));
		btnLbDelete = ButtonWidget.Cast(layoutRoot.FindAnyWidget("btnLbDelete"));

		layoutRoot.Show(false);
		return layoutRoot;
	}

	override void OnShow() {
		super.OnShow();
		GetGame().GetInput().ChangeGameFocus( 1 );

		modeValue.SetCurrentItem(g_Game.pvez_Config.GENERAL.Mode);
		freqValue.SetText(g_Game.pvez_Config.GENERAL.Update_Frequency.ToString());
		showNotifsValue.SetChecked(g_Game.pvez_Config.GENERAL.Show_Notifications);
		showUIValue.SetChecked(g_Game.pvez_Config.GENERAL.Show_Notifications && g_Game.pvez_Config.GENERAL.Use_UI_Notifications);
		showNameInNotifsValue.SetChecked(g_Game.pvez_Config.GENERAL.Add_Zone_Name_To_Message);
		exitDelayValue.SetText(g_Game.pvez_Config.GENERAL.Exit_Zone_Countdown.ToString());
		customMsgEnterValue.SetText(g_Game.pvez_Config.GENERAL.Custom_Enter_Zone_Message);
		customMsgExitValue.SetText(g_Game.pvez_Config.GENERAL.Custom_Exit_Zone_Message);
		customMsgExitCountdownValue.SetText(g_Game.pvez_Config.GENERAL.Custom_Exit_Zone_Countdown_Message);

		healTargetValue.SetChecked(g_Game.pvez_Config.DAMAGE.Restore_Target_Health);
		healCargoValue.SetChecked(g_Game.pvez_Config.DAMAGE.Protect_Clothing_And_Cargo);
		allowDmgBeyondZoneValue.SetChecked(g_Game.pvez_Config.DAMAGE.Allow_Damage_Between_PVP_and_PVE);
		reflectDmgWeaponsValue.SetChecked(g_Game.pvez_Config.DAMAGE.Damage_Types_Sent_Back_To_The_Attacker.Weapon_Damage);
		reflectDmgExplosivesValue.SetChecked(g_Game.pvez_Config.DAMAGE.Damage_Types_Sent_Back_To_The_Attacker.Explosive_Damage);
		reflectDmgVehiclesValue.SetChecked(g_Game.pvez_Config.DAMAGE.Damage_Types_Sent_Back_To_The_Attacker.Vehicle_Damage);
		reflectDmgFistsValue.SetChecked(g_Game.pvez_Config.DAMAGE.Damage_Types_Sent_Back_To_The_Attacker.Fist_Fight_Damage);

		lbKillTypeWeaponsValue.SetChecked(g_Game.pvez_Config.LAWBREAKERS_SYSTEM.Declare_a_Lawbreaker_When_Killed_a_Player_in_PVE_Area.On_Weaponary_Kill);
		lbKillTypeExplosivesValue.SetChecked(g_Game.pvez_Config.LAWBREAKERS_SYSTEM.Declare_a_Lawbreaker_When_Killed_a_Player_in_PVE_Area.On_Explosive_Kill);
		lbKillTypeVehiclesValue.SetChecked(g_Game.pvez_Config.LAWBREAKERS_SYSTEM.Declare_a_Lawbreaker_When_Killed_a_Player_in_PVE_Area.On_Vehicle_Kill);
		lbKillTypeFistsValue.SetChecked(g_Game.pvez_Config.LAWBREAKERS_SYSTEM.Declare_a_Lawbreaker_When_Killed_a_Player_in_PVE_Area.On_Fist_Fight_Kill);
		lbServerMessageValue.SetChecked(g_Game.pvez_Config.LAWBREAKERS_SYSTEM.Server_Wide_Message_About_Lawbreaker);
		lbAttackAnywhereValue.SetChecked(g_Game.pvez_Config.LAWBREAKERS_SYSTEM.Allow_Lawbreakers_To_Attack_Anywhere);
		lbPardonOnAnyDeathValue.SetChecked(g_Game.pvez_Config.LAWBREAKERS_SYSTEM.Pardon_On_Death_From_Any_Source);
		lbAutoclearValue.SetChecked(g_Game.pvez_Config.LAWBREAKERS_SYSTEM.Auto_Clear_Lawbreakers_Data);
		lbAutoclearPeriodValue.SetText(g_Game.pvez_Config.LAWBREAKERS_SYSTEM.Autoclear_Period_Amount.ToString());
		lbAutoclearPeriodMode.SetCurrentItem(g_Game.pvez_Config.LAWBREAKERS_SYSTEM.Autoclear_Period_Mode);
		customMsgLbServerWideValue.SetText(g_Game.pvez_Config.LAWBREAKERS_SYSTEM.Custom_Crime_Notification_Message_Format);
		customMsgLbPersonalValue.SetText(g_Game.pvez_Config.LAWBREAKERS_SYSTEM.Custom_Message_for_the_Lawbreaker);

		mapShowLbMarkersValue.SetChecked(g_Game.pvez_Config.MAP.Lawbreakers_Markers.Show_Markers_On_Map);
		mapLbMarkersUpdateFreqValue.SetText(g_Game.pvez_Config.MAP.Lawbreakers_Markers.Update_Frequency.ToString());
		mapLbMarkersUseApproximateValue.SetChecked(g_Game.pvez_Config.MAP.Lawbreakers_Markers.Approximate_Location);
		mapLbMarkersApproximateOffsetValue.SetText(g_Game.pvez_Config.MAP.Lawbreakers_Markers.Approximate_Location_Max_Offset.ToString());
		mapLbCanSeeTheirOwnMarkerValue.SetChecked(g_Game.pvez_Config.MAP.Lawbreakers_Markers.Lawbreakers_Can_See_Their_Own_Markers);
	}

	override void OnHide() {
		super.OnHide();
		GetGame().GetInput().ResetGameFocus();
		// Free some memory, we don't need these anymore
		playersOnServer = NULL;
		selectedZone = NULL;
		selectedLawbreaker = NULL;
	}

	override bool OnChange( Widget w, int x, int y, bool finished ) {
		if (w == showNotifsValue) {
			if (!showNotifsValue.IsChecked())
				showUIValue.SetChecked(false);
			return true;
		}
		if (w == showUIValue) {
			if (showUIValue.IsChecked())
				showNotifsValue.SetChecked(true);
			return true;
		}
		return false;
	}

	override bool OnClick( Widget w, int x, int y, int button ) {
		if (w == btnGeneral) {
			SettingsRootPanel.Show(true);
			ZonesRootPanel.Show(false);
			ZoneSettingsPanel.Show(false);
			LawbreakersRootPanel.Show(false);
			lbDataPanel.Show(false);
			return true;
		}
		if (w == btnZones) {
			SettingsRootPanel.Show(false);
			ZonesRootPanel.Show(true);
			ZoneSettingsPanel.Show(false);
			LawbreakersRootPanel.Show(false);
			lbDataPanel.Show(false);
			GetGame().RPCSingleParam(GetGame().GetPlayer(), PVEZ_RPC.ADMIN_ZONES_DATA_REQUEST, NULL, true);
			return true;
		}
		if (w == btnLawbreakers) {
			SettingsRootPanel.Show(false);
			ZonesRootPanel.Show(false);
			ZoneSettingsPanel.Show(false);
			LawbreakersRootPanel.Show(true);
			lbDataPanel.Show(false);
			GetGame().RPCSingleParam(GetGame().GetPlayer(), PVEZ_RPC.ADMIN_LAWBREAKERS_DATA_REQUEST, NULL, true);
			return true;
		}
		if (w == btnSettingsApply) {
			g_Game.pvez_Config.GENERAL.Mode = modeValue.GetCurrentItem();
			
			g_Game.pvez_Config.GENERAL.Update_Frequency = freqValue.GetText().ToInt();
			g_Game.pvez_Config.GENERAL.Show_Notifications = showNotifsValue.IsChecked();
			g_Game.pvez_Config.GENERAL.Use_UI_Notifications = showUIValue.IsChecked();
			g_Game.pvez_Config.GENERAL.Add_Zone_Name_To_Message = showNameInNotifsValue.IsChecked();
			g_Game.pvez_Config.GENERAL.Exit_Zone_Countdown = exitDelayValue.GetText().ToInt();
			g_Game.pvez_Config.GENERAL.Custom_Enter_Zone_Message = customMsgEnterValue.GetText();
			g_Game.pvez_Config.GENERAL.Custom_Exit_Zone_Message = customMsgExitValue.GetText();
			g_Game.pvez_Config.GENERAL.Custom_Exit_Zone_Countdown_Message = customMsgExitCountdownValue.GetText();
			
			g_Game.pvez_Config.DAMAGE.Restore_Target_Health = healTargetValue.IsChecked();
			g_Game.pvez_Config.DAMAGE.Protect_Clothing_And_Cargo = healCargoValue.IsChecked();
			g_Game.pvez_Config.DAMAGE.Allow_Damage_Between_PVP_and_PVE = allowDmgBeyondZoneValue.IsChecked();
			g_Game.pvez_Config.DAMAGE.Damage_Types_Sent_Back_To_The_Attacker.Weapon_Damage = reflectDmgWeaponsValue.IsChecked();
			g_Game.pvez_Config.DAMAGE.Damage_Types_Sent_Back_To_The_Attacker.Explosive_Damage = reflectDmgExplosivesValue.IsChecked();
			g_Game.pvez_Config.DAMAGE.Damage_Types_Sent_Back_To_The_Attacker.Vehicle_Damage = reflectDmgVehiclesValue.IsChecked();
			g_Game.pvez_Config.DAMAGE.Damage_Types_Sent_Back_To_The_Attacker.Fist_Fight_Damage = reflectDmgFistsValue.IsChecked();

			g_Game.pvez_Config.LAWBREAKERS_SYSTEM.Declare_a_Lawbreaker_When_Killed_a_Player_in_PVE_Area.On_Weaponary_Kill = lbKillTypeWeaponsValue.IsChecked();
			g_Game.pvez_Config.LAWBREAKERS_SYSTEM.Declare_a_Lawbreaker_When_Killed_a_Player_in_PVE_Area.On_Explosive_Kill = lbKillTypeExplosivesValue.IsChecked();
			g_Game.pvez_Config.LAWBREAKERS_SYSTEM.Declare_a_Lawbreaker_When_Killed_a_Player_in_PVE_Area.On_Vehicle_Kill = lbKillTypeVehiclesValue.IsChecked();
			g_Game.pvez_Config.LAWBREAKERS_SYSTEM.Declare_a_Lawbreaker_When_Killed_a_Player_in_PVE_Area.On_Fist_Fight_Kill = lbKillTypeFistsValue.IsChecked();
			g_Game.pvez_Config.LAWBREAKERS_SYSTEM.Server_Wide_Message_About_Lawbreaker = lbServerMessageValue.IsChecked();
			g_Game.pvez_Config.LAWBREAKERS_SYSTEM.Allow_Lawbreakers_To_Attack_Anywhere = lbAttackAnywhereValue.IsChecked();
			g_Game.pvez_Config.LAWBREAKERS_SYSTEM.Pardon_On_Death_From_Any_Source = lbPardonOnAnyDeathValue.IsChecked();
			g_Game.pvez_Config.LAWBREAKERS_SYSTEM.Auto_Clear_Lawbreakers_Data = lbAutoclearValue.IsChecked();
			g_Game.pvez_Config.LAWBREAKERS_SYSTEM.Autoclear_Period_Amount = lbAutoclearPeriodValue.GetText().ToInt();
			g_Game.pvez_Config.LAWBREAKERS_SYSTEM.Autoclear_Period_Mode = lbAutoclearPeriodMode.GetCurrentItem();
			g_Game.pvez_Config.LAWBREAKERS_SYSTEM.Custom_Crime_Notification_Message_Format = customMsgLbServerWideValue.GetText();
			g_Game.pvez_Config.LAWBREAKERS_SYSTEM.Custom_Message_for_the_Lawbreaker = customMsgLbPersonalValue.GetText();

			g_Game.pvez_Config.MAP.Lawbreakers_Markers.Show_Markers_On_Map = mapShowLbMarkersValue.IsChecked();
			g_Game.pvez_Config.MAP.Lawbreakers_Markers.Update_Frequency = mapLbMarkersUpdateFreqValue.GetText().ToInt();
			g_Game.pvez_Config.MAP.Lawbreakers_Markers.Approximate_Location = mapLbMarkersUseApproximateValue.IsChecked();
			g_Game.pvez_Config.MAP.Lawbreakers_Markers.Approximate_Location_Max_Offset = mapLbMarkersApproximateOffsetValue.GetText().ToInt();
			g_Game.pvez_Config.MAP.Lawbreakers_Markers.Lawbreakers_Can_See_Their_Own_Markers = mapLbCanSeeTheirOwnMarkerValue.IsChecked();
			PlayerBase.Cast(GetGame().GetPlayer()).MessageStatus("PVEZ: Applying changes to config");
			GetGame().RPCSingleParam(GetGame().GetPlayer(), PVEZ_RPC.ADMIN_UPDATE_CONFIG_ON_SERVER, new Param1<PVEZ_Config>(g_Game.pvez_Config), true);
			return true;
		}
		if (w == zonesList) {
			int selectedZoneRow = zonesList.GetSelectedRow();
			if (selectedZoneRow == -1) return true;

			Param1<ref PVEZ_Zone> selectedZoneParam = new Param1<ref PVEZ_Zone>(NULL);
			zonesList.GetItemData(selectedZoneRow, 0, selectedZoneParam);
			selectedZone = selectedZoneParam.param1;

			if (selectedZone)
				OnZoneSelected(selectedZone);
			else
				zoneNameValue.SetText("No zone");
			return true;
		}
		if (w == btnCreateNewZone) {
			selectedZone = new ref PVEZ_Zone(0, 0, "New zone", 100, false, false);
			OnZoneSelected(selectedZone);
			return true;
		}
		if (w == btnZoneSetCurrentPosition) {
			vector curPos = GetGame().GetPlayer().GetPosition();
			zoneCoordXValue.SetText(curPos[0].ToString());
			zoneCoordZValue.SetText(curPos[2].ToString());
			return true;
		}
		if (w == btnZoneApply) {
			int zoneindex = g_Game.pvez_Zones.allZones.Find(selectedZone);
			if (zoneindex >= 0) {
				PlayerBase.Cast(GetGame().GetPlayer()).MessageStatus("PVEZ: Applying changes to the zone " + selectedZone.Name);
				selectedZone = g_Game.pvez_Zones.allZones[zoneindex];
			}
			else {
				PlayerBase.Cast(GetGame().GetPlayer()).MessageStatus("PVEZ: Creating a new zone...");
				g_Game.pvez_Zones.allZones.Insert(selectedZone);
			}
			selectedZone.Name = zoneNameValue.GetText();
			selectedZone.X = zoneCoordXValue.GetText().ToInt();
			selectedZone.Z = zoneCoordZValue.GetText().ToInt();
			selectedZone.Radius = zoneRadiusValue.GetText().ToInt();
			selectedZone.ShowBorderOnMap = zoneShowBorderValue.IsChecked();
			selectedZone.ShowNameOnMap = zoneShowNameValue.IsChecked();
			selectedZone.Activity_Schedule.Days = zoneDaysValue.GetText();
			selectedZone.Activity_Schedule.StartHour = zoneHourStartValue.GetText().ToInt();
			selectedZone.Activity_Schedule.EndHour = zoneHourEndValue.GetText().ToInt();
			GetGame().RPCSingleParam(GetGame().GetPlayer(), PVEZ_RPC.UPDATE_ZONES_ON_SERVER, new Param1<array<ref PVEZ_Zone>>(g_Game.pvez_Zones.allZones), true);
			// If the week mode has been toggled, then push new config update to server
			if (zoneDaysModeValue.IsChecked() != g_Game.pvez_Config.GENERAL.Week_Starts_On_Sunday) {
				g_Game.pvez_Config.GENERAL.Week_Starts_On_Sunday = zoneDaysModeValue.IsChecked();
				GetGame().RPCSingleParam(GetGame().GetPlayer(), PVEZ_RPC.ADMIN_UPDATE_CONFIG_ON_SERVER, new Param1<PVEZ_Config>(g_Game.pvez_Config), true);
			}
			return true;
		}
		if (w == btnZoneDelete) {
			PlayerBase.Cast(GetGame().GetPlayer()).MessageStatus("PVEZ: Removing the zone " + selectedZone.Name + "...");
			g_Game.pvez_Zones.allZones.RemoveItem(selectedZone);
			GetGame().RPCSingleParam(GetGame().GetPlayer(), PVEZ_RPC.UPDATE_ZONES_ON_SERVER, new Param1<array<ref PVEZ_Zone>>(g_Game.pvez_Zones.allZones), true);
			return true;
		}
		if (w == lbPlayersList) {
			int selectedLBRow = lbPlayersList.GetSelectedRow();
			if (selectedLBRow == -1) return true;

			Param1<ref PVEZ_Lawbreaker> selectedLBParam = new Param1<ref PVEZ_Lawbreaker>(NULL);
			lbPlayersList.GetItemData(selectedLBRow, 0, selectedLBParam);
			selectedLawbreaker = selectedLBParam.param1;

			if (selectedLawbreaker)
				OnLawbreakerSelected(selectedLawbreaker);
			else
				lbNamesValue.SetText("No data");
			return true;
		}
		if (w == btnLbApply) {
			int lbindex = g_Game.pvez_LawbreakersRoster.lbDataBase.Find(selectedLawbreaker);
			if (lbindex >= 0)
				selectedLawbreaker = g_Game.pvez_LawbreakersRoster.lbDataBase[lbindex];
			else {
				PlayerBase.Cast(GetGame().GetPlayer()).MessageStatus("PVEZ: Can't find the lawbreaker's data. Try to re-open this page.");
				return true;
			}
			PlayerBase.Cast(GetGame().GetPlayer()).MessageStatus("PVEZ: Changing the lawbreaker status. The player need to wait a minute.");
			selectedLawbreaker.Is_Currently_Outlaw = lbStatusValue.IsChecked();
			GetGame().RPCSingleParam(GetGame().GetPlayer(), PVEZ_RPC.ADMIN_UPDATE_LAWBREAKERS_ON_SERVER, new Param1<array<ref PVEZ_Lawbreaker>>(g_Game.pvez_LawbreakersRoster.lbDataBase), true);
			return true;
		}
		if (w == btnLbDelete) {
			PlayerBase.Cast(GetGame().GetPlayer()).MessageStatus("PVEZ: Deleting the lawbreaker data. The player need to wait a minute.");
			g_Game.pvez_LawbreakersRoster.lbDataBase.RemoveItem(selectedLawbreaker);
			GetGame().RPCSingleParam(GetGame().GetPlayer(), PVEZ_RPC.ADMIN_UPDATE_LAWBREAKERS_ON_SERVER, new Param1<array<ref PVEZ_Lawbreaker>>(g_Game.pvez_LawbreakersRoster.lbDataBase), true);
			return true;
		}
		return false;
	}

	void UpdateZonesList() {
		zonesList.ClearItems();
		for (int i = 0; i < g_Game.pvez_Zones.allZones.Count(); i++) {
			ref PVEZ_Zone zone = g_Game.pvez_Zones.allZones.Get(i);
			Param1<ref PVEZ_Zone> data = new Param1<ref PVEZ_Zone>(zone);
			zonesList.AddItem((i + 1).ToString() + ". " + g_Game.pvez_Zones.allZones[i].Name, data, 0, i);
		}
		if (g_Game.pvez_Zones.allZones.Find(selectedZone) < 0)
			ZoneSettingsPanel.Show(false);
	}

	void OnZoneSelected(PVEZ_Zone selectedZone) {
		if (!ZoneSettingsPanel.IsVisible())
			ZoneSettingsPanel.Show(true);
		zoneNameValue.SetText(selectedZone.Name);
		zoneCoordXValue.SetText(selectedZone.X.ToString());
		zoneCoordZValue.SetText(selectedZone.Z.ToString());
		zoneRadiusValue.SetText(selectedZone.Radius.ToString());
		zoneShowBorderValue.SetChecked(selectedZone.ShowBorderOnMap);
		zoneShowNameValue.SetChecked(selectedZone.ShowNameOnMap);
		zoneDaysValue.SetText(selectedZone.Activity_Schedule.Days);
		zoneDaysModeValue.SetChecked(g_Game.pvez_Config.GENERAL.Week_Starts_On_Sunday);
		zoneHourStartValue.SetText(selectedZone.Activity_Schedule.StartHour.ToString());
		zoneHourEndValue.SetText(selectedZone.Activity_Schedule.EndHour.ToString());
		// Display server time
		int day, hour, min, sec;
		GetHourMinuteSecond(hour, min, sec);
		day = PVEZ_TimeHelper.GetDayOfWeek(g_Game.pvez_Config.GENERAL.Week_Starts_On_Sunday);
		serverTimeValue.SetText("[Day]" + day.ToString() + ", [Time]" + hour.ToString() + ":" + min.ToString());
	}

	void UpdateLawbreakersList(array<Man> players) {
		playersOnServer = players;
		lbPlayersList.ClearItems();
		for (int i = 0; i < g_Game.pvez_LawbreakersRoster.lbDataBase.Count(); i++) {
			ref PVEZ_Lawbreaker lb = g_Game.pvez_LawbreakersRoster.lbDataBase.Get(i);
			string uid = lb.UID;
			string name = lb.Recent_Character_Names[0];
			Param1<ref PVEZ_Lawbreaker> data = new Param1<ref PVEZ_Lawbreaker>(lb);
			lbPlayersList.AddItem((i + 1).ToString() + ". " + name + " (" + uid + ")", data, 0, i);
		}
		if (g_Game.pvez_LawbreakersRoster.lbDataBase.Find(selectedLawbreaker) < 0)
			lbDataPanel.Show(false);
	}

	void OnLawbreakerSelected(PVEZ_Lawbreaker selectedLawbreaker) {
		if (!lbDataPanel.IsVisible())
			lbDataPanel.Show(true);
		lbUIDValue.SetText(selectedLawbreaker.UID);
		string names = "";
		for (int i = 0; i < selectedLawbreaker.Recent_Character_Names.Count(); i++) {
			names = names + selectedLawbreaker.Recent_Character_Names[i] + ", ";
		}
		lbNamesValue.SetText(names);
		lbCountValue.SetText(selectedLawbreaker.Murder_Count.ToString());
		lbLatestMurderTimeValue.SetText(PVEZ_TimeHelper.GetFormattedDateTimeFromPVEZ_DateTime(selectedLawbreaker.Latest_Murder_Time));
		lbStatusValue.SetChecked(selectedLawbreaker.Is_Currently_Outlaw);

		if (playersOnServer && playersOnServer.Count() > 0) {
			foreach (Man p : playersOnServer) {
				if (p.GetIdentity().GetId() == selectedLawbreaker.UID) {
					lbPreview.SetPlayer(DayZPlayer.Cast(p));
					lbPreview.SetModelPosition( "0 0 0.605" );
					lbPreview.SetSize( 0.5, 0.5 );  // default scale
					break;
				}
			}
		}
	}
	/*
The mode:
PVP zones - the zones are PvP areas, the rest of the map is PvE.
PVE zones - the zones are PvE safe zones, the rest of the map is PvP area.
PVE - the whole map is PvE area, PvP is not allowed (except Airdrop zones if you use one of the Airdrop(Upgraded)+PVEZAirdropZones or DayZ-Expansion+PVEZExpansionPlugin.)
PVP - just like the vanilla game, PvP works anywwhere, but you can use the zones to show your players where PvP is legal.
*/

	override bool OnMouseEnter(Widget w, int x, int y) {
		if (w.GetTypeName() == "EditBoxWidget") {
			isTypingText = true;
		}
		
		switch (w.GetParent()) {
			case modePanel:
				descriptionTitle.SetText("The mode:\n* PVP zones: the zones are PvP areas, the rest of the map is PvE.\n* PVE zones: the zones are PvE safe zones, the rest of the map is PvP area.\n* PVE: the whole map is PvE area, PvP is not allowed (except Airdrop zones if you use one of the Airdrop(Upgraded)+PVEZAirdropZones or DayZ-Expansion+PVEZExpansionPlugin.)\n* PVP: just like the vanilla game, PvP works anywwhere, but you can use the zones to show your players where PvP is legal.");
				ResizeDescriptionWidget();
				descriptionFrame.Show(true);
				return true;
			case freqPanel:
				descriptionTitle.SetText("Update frequency.\nThe period in seconds that determines how often the mod will check players current positions to update their PvP status (is the player in a zone or not).");
				ResizeDescriptionWidget();
				descriptionFrame.Show(true);
				return true;
			case showNotifsPanel:
				descriptionTitle.SetText("Show notifications to players when they enter or leave a zone.");
				ResizeDescriptionWidget();
				descriptionFrame.Show(true);
				return true;
			case showUIPanel:
				descriptionTitle.SetText("Use GUI notifications. When this is OFF, players will receive chat messages on entering and leaving zones.");
				ResizeDescriptionWidget();
				descriptionFrame.Show(true);
				return true;
			case showNameInNotifsPanel:
				descriptionTitle.SetText("Show the zone name in notifications.");
				ResizeDescriptionWidget();
				descriptionFrame.Show(true);
				return true;
			case exitDelayPanel:
				descriptionTitle.SetText("Countdown in seconds before leaving a zone. When player leaves a zone, their status will be changed only after the countdown is over.");
				ResizeDescriptionWidget();
				descriptionFrame.Show(true);
				return true;
			case customMsgEnterPanel:
				descriptionTitle.SetText("Print your own text here and it will be shown in notifications replacing the localized text.\nBy default it's the localized text that is determined by the selected Mode.\nIf the mode is 'PvP zones' or 'PvP', then it will be localized variations of 'You entered the open fire zone'.\nIf the Mode is 'PvE zones', the message will be 'You entered the armistice zone'.");
				ResizeDescriptionWidget();
				descriptionFrame.Show(true);
				return true;
			case customMsgExitPanel:
				descriptionTitle.SetText("Print your own text here and it will be shown in notifications replacing the localized text.\nBy default it's the localized text that is determined by the selected Mode.\nIf the mode is 'PvP zones' or 'PvP', then it will be localized variations of 'You left the open fire zone'.\nIf the Mode is 'PvE zones', the message will be 'You left the armistice zone'.");
				ResizeDescriptionWidget();
				descriptionFrame.Show(true);
				return true;
			case customMsgExitCountdownPanel:
				descriptionTitle.SetText("Print your own text here and it will be shown in notifications replacing the localized text.\nBy default it's the localized text that is determined by the selected Mode.\nIf the mode is 'PvP zones' or 'PvP', then it will be localized variations of 'You will leave the open fire zone in' followed by the countdown timer.\nIf the Mode is 'PvE zones', the message will be 'You will leave the armistice zone in' followed by the countdown timer.");
				ResizeDescriptionWidget();
				descriptionFrame.Show(true);
				return true;
			case healTargetPanel:
				descriptionTitle.SetText("Heal target.\nRestore the health of the player that has been hit by another player in PvE area.\nNOTE: as mentioned in the mod description, we can't disable damage to players completely, the mod just restores the health that is lost after the hit. So, it still won't protect from headshots and grenades.");
				ResizeDescriptionWidget();
				descriptionFrame.Show(true);
				return true;
			case healCargoPanel:
				descriptionTitle.SetText("Restore the 'health' of the players clothing and inventory when they get hit by another player in PvE area.");
				ResizeDescriptionWidget();
				descriptionFrame.Show(true);
				return true;
			case allowDmgBeyondZonePanel:
				descriptionTitle.SetText("Allow damage between PvP & PvE.\nWhen this is ON, a player inside a PvP zone will be able and allowed to kill a player outside the zone. When OFF, both players have to be within the zone, otherwise the killer will be considered a lawbreaker if they kill the player that is outside.");
				ResizeDescriptionWidget();
				descriptionFrame.Show(true);
				return true;
			case reflectDmgPanel:
				descriptionTitle.SetText("Types of damage that will be reflected back to the attacker when they attack a player in PvE area.");
				ResizeDescriptionWidget();
				descriptionFrame.Show(true);
				return true;
			case lbKillTypesPanel:
				descriptionTitle.SetText("Determines the cases when the player that has killed another one in PvE area should be treated as a lawbreaker.\nNOTE: any item in hands is treated as weapon (even a bandage), therefore it falls into the 1st category.");
				ResizeDescriptionWidget();
				descriptionFrame.Show(true);
				return true;
			case lbServerMessagePanel:
				descriptionTitle.SetText("Inform all the players on the server about a lawful kill.");
				ResizeDescriptionWidget();
				descriptionFrame.Show(true);
				return true;
			case lbAutoclearPanel:
				descriptionTitle.SetText("Clear the lawbreaker data automatically after a certain amount of time from their last murder.");
				ResizeDescriptionWidget();
				descriptionFrame.Show(true);
				return true;
			case lbAutoclearPeriodPanel:
				descriptionTitle.SetText("How long a lawbreaker data should be stored. Does nothing if the Autoclear is OFF.");
				ResizeDescriptionWidget();
				descriptionFrame.Show(true);
				return true;
			case lbAttackAnywherePanel:
				descriptionTitle.SetText("When someone becomes a lawbreaker, other players will be able to kill the lawbreaker without any consiquences. And they are allowed to attack him even in a PvE zone. So, you might want to allow the lawbreaker to be able to fight back. Otherwise, if you have target healing ON or damage reflection ON, the lawbreaker won't be able to fight against the headhunters.");
				ResizeDescriptionWidget();
				descriptionFrame.Show(true);
				return true;
			case lbPardonOnAnyDeathPanel:
				descriptionTitle.SetText("The lawbreakers get their status OFF when they die. With this setting you can determine when you want to forgive them.\nWhen this is ON, the lawbreaker will be forgiven even if they commit a suicide at their own base, for example.\nWhen OFF, they will be forgiven only when they have been killed by another player who has no lawbreaker status.");
				ResizeDescriptionWidget();
				descriptionFrame.Show(true);
				return true;
			case customMsgLbServerWidePanel:
				string msg = "By default the format of the message that is showwn to all players on the server looks like this:\n1 killed 2. Weapon: 3.\nThe 1 will be replaced with the killer name, 2 - victim name, 3 - weapon.";
				descriptionTitle.SetText(msg);
				ResizeDescriptionWidget();
				descriptionFrame.Show(true);
				return true;
			case customMsgLbPersonalPanel:
				descriptionTitle.SetText("Your custom message for the player who kills someone, this will replace the localized text.");
				ResizeDescriptionWidget();
				descriptionFrame.Show(true);
				return true;
			case mapShowLbMarkersPanel:
				descriptionTitle.SetText("Show lawbreaker's position marker on the map that is visible for all players on the server");
				ResizeDescriptionWidget();
				descriptionFrame.Show(true);
				return true;
			case mapLbMarkersUpdateFreqPanel:
				descriptionTitle.SetText("How often lawbreakers markers will be updated on the map. In seconds.");
				ResizeDescriptionWidget();
				descriptionFrame.Show(true);
				return true;
			case mapLbMarkersUseApproximatePanel:
				descriptionTitle.SetText("Use an approximate lawwbreaker's position for the marker instead of the exact point.");
				ResizeDescriptionWidget();
				descriptionFrame.Show(true);
				return true;
			case mapLbMarkersApproximateOffsetPanel:
				descriptionTitle.SetText("Max approximity offset from the exact lawbreaker position in meters. On every LB markers update the position for the marker will be set somewhere within this radius from the exact point.");
				ResizeDescriptionWidget();
				descriptionFrame.Show(true);
				return true;
			case mapLbCanSeeTheirOwnMarkerPanel:
				descriptionTitle.SetText("Allow a lawbreaker to see their own LB marker.");
				ResizeDescriptionWidget();
				descriptionFrame.Show(true);
				return true;
		}
		return false;
	}

	override bool OnMouseLeave(Widget w, Widget enterW, int x, int y) {
		if (w.GetTypeName() == "EditBoxWidget") {
			isTypingText = false;
		}
		descriptionFrame.Show(false);
		return true;
	}

	void ResizeDescriptionWidget() {
		float vsize = (descriptionTitle.GetNumLines() * 20) + 5;
		if (vsize < 25) vsize = 25;
		descriptionFrame.SetSize(0.48, vsize);
	}
}