#include "../features.hpp"

void visual::esp_bomb::run() {
	if (!config_system.item.visuals_enabled || (config_system.item.anti_screenshot && interfaces::engine->is_taking_screenshot()))
		return;

	auto local_player = reinterpret_cast<player_t*>(interfaces::entity_list->get_client_entity(interfaces::engine->get_local_player()));
	if (!local_player)
		return;

	if (!config_system.item.bomb_planted)
		return;

	for (int i = 1; i < interfaces::entity_list->get_highest_index(); i++) {
		auto entity = reinterpret_cast<player_t*>(interfaces::entity_list->get_client_entity(i));
		if(!entity)
			continue;

		if (entity && entity != local_player) {
			auto client_class = entity->client_class();
			if (config_system.item.bomb_planted) {
				if (client_class->class_id == class_ids::cplantedc4) {
					if (entity->c4_is_ticking() && !entity->c4_is_defused()) {
						const float bomb_duration = entity->c4_blow_time() - interfaces::globals->cur_time;

						if (bomb_duration > 0.f) 
							bomb_esp(entity);		
					}	
					bomb_defuse_esp(entity);
					
					visual::esp_world::dropped_weapon(entity);
				}
			}
		}
	}
}

void visual::esp_bomb::bomb_esp(player_t* entity){
	if (!entity)
		return;

	auto local_player = reinterpret_cast<player_t*>(interfaces::entity_list->get_client_entity(interfaces::engine->get_local_player()));
	if (!local_player)
		return;
	
	auto explode_time = entity->c4_blow_time();
	auto remaining_time = explode_time - (interfaces::globals->interval_per_tick * local_player->get_tick_base());
	if (remaining_time < 0)
		return;

	int width, height;
	interfaces::engine->get_screen_size(width, height);

	vec3_t bomb_origin, bomb_position;
	bomb_origin = entity->origin();

	explode_time -= interfaces::globals->interval_per_tick * local_player->get_tick_base();
	if (explode_time <= 0)
		explode_time = 0;

	char buffer[32];
	sprintf(buffer, "%s - %.0fs", entity->c4_site() ? "B" : "A", explode_time);

	auto c4_tick = interfaces::console->get_convar("mp_c4timer")->get_int();
	auto value = (explode_time * height) / c4_tick;

	//bomb damage indicator calculations, credits casual_hacker
	float damage;
	auto distance = local_player->get_eye_pos().distance_to(entity->get_eye_pos());
	auto a = 450.7f;
	auto b = 75.68f;
	auto c = 789.2f;
	auto d = ((distance - b) / c);
	auto fl_damage = a * exp(-d * d);
	damage = float((std::max)((int)ceilf(utilities::csgo_armor(fl_damage, local_player->armor())), 0));

	//convert damage to string
	std::string damage_text;
	damage_text += "-";
	damage_text += std::to_string((int)(damage));
	damage_text += "HP";

	//render on screen bomb bar
	if (explode_time <= 10) {
		render::draw_filled_rect(0, 0, 20, value, color(255, 0, 0, 150));
	}
	else {
		render::draw_filled_rect(0, 0, 20, value, color(0, 255, 0, 150));
	}

	player_t* bomb = nullptr;
	for (int i = 1; i < interfaces::entity_list->get_highest_index(); i++) {

		if (entity->client_class()->class_id == class_ids::cplantedc4) {
			bomb = (player_t*)entity;
			break;
		}
	}

	//render bomb timer
	render::draw_text_string(22, value - 11, render::fonts::name_font_big, buffer, false, color(255, 255, 255));

	//render bomb damage
	if (local_player->is_alive()) {
		render::draw_text_string(22, value - 21, render::fonts::name_font_big, damage_text, false, color(255, 255, 255));
	}

	//render fatal check
	if (local_player->is_alive() && damage >= local_player->health()) {
		render::draw_text_string(22, value - 31, render::fonts::name_font_big, "FATAL", false, color(255, 0, 0));
	}

	if (!math::world_to_screen(bomb_origin, bomb_position))
		return;
	//render classic world timer + bar
	render::draw_text_string(bomb_position.x, bomb_position.y, render::fonts::name_font_big, buffer, true, color(255, 255, 255));
	render::draw_filled_rect(bomb_position.x - c4_tick / 2, bomb_position.y + 13, c4_tick, 3, color(10, 10, 10, 180)); //c4_timer / 2 so it always will be centered
	render::draw_filled_rect(bomb_position.x - c4_tick / 2, bomb_position.y + 13, explode_time, 3, color(167, 24, 71, 255));
}

void visual::esp_bomb::bomb_defuse_esp(player_t* entity){
	auto local_player = reinterpret_cast<player_t*>(interfaces::entity_list->get_client_entity(interfaces::engine->get_local_player()));
	if (!local_player)
		return;

	int width, height;
	interfaces::engine->get_screen_size(width, height);

	auto c4_tick = interfaces::console->get_convar("mp_c4timer")->get_int();
	auto c4_end_time = entity->c4_blow_time() - (interfaces::globals->interval_per_tick * local_player->get_tick_base());
	auto c4_defuse_time = entity->c4_defuse_countdown() - (interfaces::globals->interval_per_tick * local_player->get_tick_base());

	char defuse_time_string[24];
	sprintf_s(defuse_time_string, sizeof(defuse_time_string) - 1, "def- %.1f", c4_defuse_time);

	auto defuse_value = (c4_defuse_time * height) / (c4_tick);

	vec3_t bomb_origin, bomb_position;
	bomb_origin = entity->origin();

	if (!math::world_to_screen(bomb_origin, bomb_position))
		return;

	if (entity->c4_gets_defused() > 0) {

		if (c4_end_time > c4_defuse_time) {
			render::draw_filled_rect(20, 0, 20, defuse_value, color(0, 191, 255, 150));// on srcreen
			render::draw_text_string(42, defuse_value - 11, render::fonts::name_font_big, defuse_time_string, false, color(0, 191, 255));// on srcreen
			render::draw_text_string(bomb_position.x, bomb_position.y - 18, render::fonts::name_font_big, defuse_time_string, true, color(0, 191, 255));// on bomb
		}
		else {
			render::draw_filled_rect(20, 0, 20, defuse_value, color(255, 0, 0, 150));// on srcreen
			render::draw_text_string(42, defuse_value - 11, render::fonts::name_font_big, "NO TIME", false, color(255, 0, 0));// on srcreen
			render::draw_text_string(bomb_position.x, bomb_position.y - 18, render::fonts::name_font_big, "NO TIME", true, color(255, 0, 0));// on bomb
		}
	}
}















































































































































































































