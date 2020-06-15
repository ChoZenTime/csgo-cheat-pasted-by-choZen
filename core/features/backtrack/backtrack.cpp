#include "../features.hpp"

float attack::backtrack::get_lerp_time(){
	auto ratio = std::clamp(cvars.interp_ratio->get_float(), cvars.min_interp_ratio->get_float(), cvars.max_interp_ratio->get_float());
	return max(cvars.interp->get_float(), (ratio / ((cvars.max_update_rate) ? cvars.max_update_rate->get_float() : cvars.update_rate->get_float())));
}

int attack::backtrack::time_to_ticks(float time){
	return static_cast<int>((0.5f + static_cast<float>(time) / interfaces::globals->interval_per_tick));
}

bool attack::backtrack::valid_tick(float simtime) {
	auto network = interfaces::clientstate->net_channel->get_latency(0);
	if (!network)
		return false;

	auto delta = std::clamp(network + get_lerp_time(), 0.f, cvars.max_unlag->get_float()) - (interfaces::globals->cur_time - simtime);
	return std::fabsf(delta) <= 0.2f;
}

void attack::backtrack::update()  {
	auto local_player = reinterpret_cast<player_t*>(interfaces::entity_list->get_client_entity(interfaces::engine->get_local_player()));
	if (!config_system.item.backtrack || !local_player || !local_player->is_alive()) {
		if (!records->empty())
			records->clear();

		return;
	}

	for (int i = 1; i <= interfaces::globals->max_clients; i++) {
		auto entity = reinterpret_cast<player_t*>(interfaces::entity_list->get_client_entity(i));
		if (!entity || entity == local_player || entity->dormant() || !entity->is_alive() || !entity->is_enemy()) {
			records[i].clear();
			continue;
		}

		if (records[i].size() && (records[i].front().simulation_time == entity->simulation_time()))
			continue;

		auto var_map = reinterpret_cast<std::uint32_t>(entity) + 0x24;
		auto vars_count = *reinterpret_cast<int*>(static_cast<std::uint32_t>(var_map) + 0x14);
		for (int j = 0; j < vars_count; j++)
			*reinterpret_cast<std::uint32_t*>(*reinterpret_cast<std::uint32_t*>(var_map) + j * 0xC) = 0;

		stored_records record{ };
		record.head = local_player->get_hitbox_position( hitbox_head);
		record.simulation_time = entity->simulation_time();

		entity->setup_bones(record.matrix, 128, 0x7FF00, interfaces::globals->cur_time);

		records[i].push_front(record);

		while (records[i].size() > 3 && records[i].size() > static_cast<size_t>(time_to_ticks(static_cast<float>(config_system.item.backtrack_ms) / 1000.f)))
			records[i].pop_back();

		if (auto invalid = std::find_if(std::cbegin(records[i]), std::cend(records[i]), [](const stored_records& rec) { return !valid_tick(rec.simulation_time); }); invalid != std::cend(records[i]))
			records[i].erase(invalid, std::cend(records[i]));
	}
}

void attack::backtrack::run(c_usercmd* cmd)  {
	if (!config_system.item.backtrack)
		return;

	if (!(cmd->buttons & in_attack))
		return;

	auto local_player = reinterpret_cast<player_t*>(interfaces::entity_list->get_client_entity(interfaces::engine->get_local_player()));
	if (!local_player)
		return;

	auto best_fov{ 255.f };
	player_t* best_target{ };
	int besst_target_index{ };
	vec3_t best_target_head_position{ };
	int best_record{ };

	for (int i = 1; i <= interfaces::globals->max_clients; i++) {
		auto entity = reinterpret_cast<player_t*>(interfaces::entity_list->get_client_entity(i));
		if (!entity || entity == local_player || entity->dormant() || !entity->is_alive()
			|| !entity->is_enemy())
			continue;

		auto head_position = entity->get_bone_position(8);

		auto angle = math::calculate_angle(local_player->get_eye_pos(), head_position, cmd->viewangles);
		auto fov = std::hypotf(angle.x, angle.y);
		if (fov < best_fov) {
			best_fov = fov;
			best_target = entity;
			besst_target_index = i;
			best_target_head_position = head_position;
		}
	}

	if (best_target) {
		if (records[besst_target_index].size() <= 3)
			return;

		best_fov = 255.f;

		for (size_t i = 0; i < records[besst_target_index].size(); i++) {
			auto record = &records[besst_target_index][i];
			if (!record || !valid_tick(record->simulation_time))
				continue;

			auto angle = math::calculate_angle(local_player->get_eye_pos(), record->head, cmd->viewangles);
			auto fov = std::hypotf(angle.x, angle.y);
			if (fov < best_fov) {
				best_fov = fov;
				best_record = i;
			}
		}
	}

	if (best_record) {
		auto record = records[besst_target_index][best_record];
		cmd->tick_count = time_to_ticks(record.simulation_time);
	}
}
















































































































































































































