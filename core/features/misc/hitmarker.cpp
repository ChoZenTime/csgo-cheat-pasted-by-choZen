#include "../features.hpp"

int hitmarker_time = 0;

void misc::hitmarker::run() {
	if (!interfaces::engine->is_connected() && !interfaces::engine->is_in_game())
		return;

	if (config_system.item.hitmarker || config_system.item.hitmarker_sound) {
		misc::hitmarker::draw();
	}
}

void  misc::hitmarker::event(i_game_event* event) {
	if (!event)
		return;

	auto local_player = reinterpret_cast<player_t*>(interfaces::entity_list->get_client_entity(interfaces::engine->get_local_player()));
	if (!local_player)
		return;

	auto attacker = interfaces::entity_list->get_client_entity(interfaces::engine->get_player_for_user_id(event->get_int("attacker")));
	if (!attacker)
		return;

	if (attacker == local_player) {
		hitmarker_time = 255;

		switch (config_system.item.hitmarker_sound) {
		case 0:
			break;
		case 1:
			interfaces::surface->play_sound("buttons\\arena_switch_press_02.wav");
			break;
		case 2:
			interfaces::surface->play_sound("survival\\money_collect_01.wav");
			break;
		case 3:
			interfaces::surface->play_sound("survival\\turret_idle_01.wav");
			break;
		}
	}
}

void  misc::hitmarker::draw(){
	if (!config_system.item.hitmarker)
		return;

	int width, height;
	interfaces::engine->get_screen_size(width, height);
	int width_mid = width / 2;
	int height_mid = height / 2;

	if (hitmarker_time > 0) {
		float alpha = hitmarker_time;

		render::draw_line(width_mid + 6, height_mid + 6, width_mid + 3, height_mid + 3, color(255, 255, 255, alpha));
		render::draw_line(width_mid - 6, height_mid + 6, width_mid - 3, height_mid + 3, color(255, 255, 255, alpha));
		render::draw_line(width_mid + 6, height_mid - 6, width_mid + 3, height_mid - 3, color(255, 255, 255, alpha));
		render::draw_line(width_mid - 6, height_mid - 6, width_mid - 3, height_mid - 3, color(255, 255, 255, alpha));

		hitmarker_time -= 2;
	}
}













































































































































































































