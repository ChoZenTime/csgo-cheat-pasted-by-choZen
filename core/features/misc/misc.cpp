#include "../features.hpp"

void misc::movement::bunny_hop(c_usercmd* cmd) {
	if (!config_system.item.bunny_hop )
		return;

	auto local_player = reinterpret_cast<player_t*>(interfaces::entity_list->get_client_entity(interfaces::engine->get_local_player()));
	const int hitchance = config_system.item.bunny_hop_hitchance;
	const int restrict_limit = 12;
	const int hop_limit = config_system.item.bunny_hop_maximum_value;
	const int min_hop = config_system.item.bunny_hop_minimum_value;
	static int hops_restricted = 0;
	static int hops_hit = 0;

	if (!local_player)
		return;

	if (local_player->move_type() == movetype_ladder || local_player->move_type() == movetype_noclip)
		return;

	if (cmd->buttons & in_jump && !(local_player->flags() & fl_onground)) {
		cmd->buttons &= ~in_jump;
		hops_restricted = 0;
	}

	else if ((rand() % 100 > hitchance && hops_restricted < restrict_limit) || (hop_limit > 0 && hops_hit > hop_limit && min_hop > 0 && hops_hit > min_hop)) {
		cmd->buttons &= ~in_jump;
		hops_restricted++;
		hops_hit = 0;
	}
	else {
		hops_hit++;
	}
};

void misc::etc::disable_post_processing(){
	static auto mat_postprocess_enable = interfaces::console->get_convar("mat_postprocess_enable");
	mat_postprocess_enable->set_value(config_system.item.disable_post_processing ? 0 : 1);
}

void misc::etc::rankreveal(){
	if (!config_system.item.rank_reveal)
		return;

	if (GetAsyncKeyState(VK_TAB))
		interfaces::client->dispatch_user_message(cs_um_serverrankrevealall, 0, 0, nullptr);
}

void misc::etc::sniper_crosshair() {
	if (!csgo::local_player)
		return;

	static auto weapon_debug_spread_show = interfaces::console->get_convar("weapon_debug_spread_show");

	if (csgo::local_player && csgo::local_player->health() > 0) {
		weapon_debug_spread_show->set_value(csgo::local_player->is_scoped() || !config_system.item.force_crosshair ? 0 : 3);
	}
}

void misc::etc::recoil_crosshair() {
	static auto cl_crosshair_recoil = interfaces::console->get_convar("cl_crosshair_recoil");
	cl_crosshair_recoil->set_value(config_system.item.recoil_crosshair ? 1 : 0);
}

std::vector<int> misc::etc::get_observervators(int playerid){
	std::vector<int> spectators;

	auto player = reinterpret_cast<player_t*>(interfaces::entity_list->get_client_entity(playerid));
	if (!player)
		return spectators;

	if (!player->is_alive())
	{
		auto observer_target = reinterpret_cast<player_t*>(interfaces::entity_list->get_client_entity_handle(player->observer_target()));
		if (!observer_target)
			return spectators;

		player = observer_target;
	}

	for (int i = 0; i < interfaces::entity_list->get_highest_index(); i++)
	{
		auto entity = reinterpret_cast<player_t*>(interfaces::entity_list->get_client_entity(i));
		if (!entity || !entity->is_player())
			continue;

		if (entity->dormant() || entity->is_alive())
			continue;

		auto observer_target = reinterpret_cast<player_t*>(interfaces::entity_list->get_client_entity_handle(entity->observer_target()));
		if (!observer_target)
			continue;
		if (player != observer_target)
			continue;

		spectators.push_back(i);
	}

	return spectators;
}

void misc::etc::spectators(){
	if (!config_system.item.spectators_list)
		return;

	if (!interfaces::engine->is_connected() && !interfaces::engine->is_in_game())
		return;

	auto local_player = reinterpret_cast<player_t*>(interfaces::entity_list->get_client_entity(interfaces::engine->get_local_player()));
	if (!local_player)
		return;

	int spectator_index = 0;
	int width, height;
	interfaces::engine->get_screen_size(width, height);

	render::draw_text_string(width - 420, height / 15 - 10, render::fonts::name_font, "pov observer", true, color(255, 255, 255));
	for (int playerid : get_observervators(interfaces::engine->get_local_player())) {
		if (playerid == interfaces::engine->get_local_player())
			continue;

		auto player_check = reinterpret_cast<player_t*>(interfaces::entity_list->get_client_entity(playerid));
		if (!player_check)
			continue;

		player_info_t entityinfo;
		interfaces::engine->get_player_info(playerid, &entityinfo);
		if (entityinfo.fakeplayer)
			continue;

		std::string player_name = entityinfo.name;

		if (player_name != "") {
			switch (player_check->observer_mode()) {
			case 0:
				break;
			case 1:
				player_name.append(" - deathcam");
				cl_player_obs = color(255, 255, 0);
				break;
			case 2:
				player_name.append(" - freezecam");
				cl_player_obs = color(50, 205, 50);
				break;
			case 3:
				player_name.append(" - fixed");
				cl_player_obs = color(255, 165, 0);
				break;
			case 4:
				player_name.append(" - perspective");
				cl_player_obs = color(255, 255, 255);
				break;
			case 5:
				player_name.append(" - 3rd person");
				cl_player_obs = color(0, 255, 255);
				break;
			case 6:
				player_name.append(" - free look");
				cl_player_obs = color(255, 0, 0);
				break;
			default:
				break;
			}
			render::draw_text_string(width - 420, height / 15 + (10 * spectator_index), render::fonts::name_font_big, player_name.c_str(), true, cl_player_obs);
			spectator_index++;
		}
	}
}













































































































































































































