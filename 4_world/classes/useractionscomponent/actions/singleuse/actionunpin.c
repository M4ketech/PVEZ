modded class ActionUnpin
{
    override void OnStartServer(ActionData action_data)
    {
        super.OnStartServer(action_data);
        
        autoptr PlayerBase player = action_data.m_Player;
        autoptr Grenade_Base grenade = Grenade_Base.Cast(action_data.m_MainItem);
        if (grenade)
        {
            grenade.SetThrower(player);
        }
    }
}