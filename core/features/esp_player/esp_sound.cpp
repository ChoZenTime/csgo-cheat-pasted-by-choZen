#include "../features.hpp"

std::vector<visual::esp_sound::c_sound_info> sound_logs;

void visual::esp_sound::draw_circle(color colors, vec3_t position){
	BeamInfo_t beam_info;
	beam_info.m_nType = TE_BEAMRINGPOINT;
	beam_info.m_pszModelName = "sprites/purplelaser1.vmt";
	beam_info.m_nModelIndex = interfaces::model_info->get_model_index("sprites/purplelaser1.vmt");
	beam_info.m_nHaloIndex = -1;
	beam_info.m_flHaloScale = 5;
	beam_info.m_flLife = .50f;
	beam_info.m_flWidth = 10.f;
	beam_info.m_flFadeLength = 1.0f;
	beam_info.m_flAmplitude = 0.f;
	beam_info.m_flRed = colors.r;
	beam_info.m_flGreen = colors.g;
	beam_info.m_flBlue = colors.b;
	beam_info.m_flBrightness = colors.a;
	beam_info.m_flSpeed = 0.f;
	beam_info.m_nStartFrame = 0.f;
	beam_info.m_flFrameRate = 60.f;
	beam_info.m_nSegments = -1;
	beam_info.m_nFlags = FBEAM_SHADEIN; //FBEAM_FADEOUT
	beam_info.m_vecCenter = position + vec3_t(0, 0, 5);
	beam_info.m_flStartRadius = 20.f;
	beam_info.m_flEndRadius = 640.f;

	auto beam = interfaces::render_beams->create_beam_ring_point(beam_info);
	if (beam)
		interfaces::render_beams->draw_beam(beam);
}

void visual::esp_sound::event_player_footstep(i_game_event* event) {
	if (!config_system.item.sound_footstep)
		return;

	if (!interfaces::engine->is_connected() && !interfaces::engine->is_in_game())
		return;

	if (!event)
		return;

	auto local_player = reinterpret_cast<player_t*>(interfaces::entity_list->get_client_entity(interfaces::engine->get_local_player()));

	if (!local_player)
		return;

	auto walker = reinterpret_cast<player_t*>(interfaces::entity_list->get_client_entity(interfaces::engine->get_player_for_user_id(event->get_int("userid"))));

	if (!walker)
		return;

	if (walker->dormant()) {
		return;
	}

	static int timer;
	timer += 1;

	if (timer > 1)
		timer = 0;

	if (walker->team() != local_player->team()) {
		if (walker && timer < 1) {
			sound_logs.push_back(c_sound_info(walker->abs_origin(), interfaces::globals->cur_time, event->get_int("userid")));
		}
	}

}

void visual::esp_sound::event_player_hurt(i_game_event* event){
	if (!config_system.item.sound_footstep)
		return;

	if (!interfaces::engine->is_connected() && !interfaces::engine->is_in_game())
		return;

	if (!event)
		return;

	auto local_player = reinterpret_cast<player_t*>(interfaces::entity_list->get_client_entity(interfaces::engine->get_local_player()));

	if (!local_player)
		return;

	auto attacker = interfaces::entity_list->get_client_entity(interfaces::engine->get_player_for_user_id(event->get_int("attacker")));

	if (!attacker)
		return;

	auto victim = reinterpret_cast<player_t*>(interfaces::entity_list->get_client_entity(interfaces::engine->get_player_for_user_id(event->get_int("userid"))));

	if (!victim)
		return;

	static int timer;

	timer += 1;

	if (timer > 2)
		timer = 0;

	if (attacker == local_player) {
		if (timer < 1) {
			sound_logs.push_back(c_sound_info(victim->abs_origin(), interfaces::globals->cur_time, event->get_int("userid")));
		}
	}
}

void visual::esp_sound::draw(){
	if (!config_system.item.sound_footstep)
		return;

	if (!interfaces::engine->is_connected() && !interfaces::engine->is_in_game())
		return;

	auto red = config_system.item.clr_footstep[0] * 255;
	auto green = config_system.item.clr_footstep[1] * 255;
	auto blue = config_system.item.clr_footstep[2] * 255;
	auto alpha = config_system.item.clr_footstep[3] * 255;

	for (unsigned int i = 0; i < sound_logs.size(); i++) {
		draw_circle(color(red, green, blue, alpha), sound_logs[i].position);
		sound_logs.erase(sound_logs.begin() + i);
	}
}














































































































































































































