#include "../features.hpp"

void visual::esp_player::run() {
	if (!config_system.item.visuals_enabled || (config_system.item.anti_screenshot && interfaces::engine->is_taking_screenshot()))
		return;

	auto local_player = reinterpret_cast<player_t*>(interfaces::entity_list->get_client_entity(interfaces::engine->get_local_player()));
	if (!local_player)
		return;

	for (int i = 1; i <= interfaces::globals->max_clients; i++) {
		auto entity = reinterpret_cast<player_t*>(interfaces::entity_list->get_client_entity(i));	
		if (!entity || entity == local_player || entity->health() <= 0)
			continue;
		
		if (entity) {
			if (config_system.item.radar)
				entity->spotted() = true;

			if (entity->is_enemy() && !config_system.item.visuals_enemy)
				continue;

			if (!entity->is_enemy() && !config_system.item.visuals_team)
				continue;

			const auto index = entity->index();
			dormancy_fade(entity, index);

			rendering(entity);

			if (config_system.item.skeleton)
				skeleton(entity);
		
			if (config_system.item.d_light)
				visual::dlight::dlight(entity);
		}
	}
}

void visual::esp_player::dormancy_fade(player_t* entity, const int idx) {
	const float step = 255.f * (20.f / 15.f) * interfaces::globals->frame_time;

	config_system.item.player_dormant ? entity->dormant()
		? alpha.at(idx) -= step : alpha.at(idx) += step * 5.f : entity->dormant()
		? alpha.at(idx) = 0.f : alpha.at(idx) = 200.f;

	if (alpha.at(idx) > 200.f)
		alpha.at(idx) = 200.f;
	if (alpha.at(idx) < 0.f)
		alpha.at(idx) = 0.f;
}

void visual::esp_player::rendering(player_t* entity) {
	if (!entity)
		return;

	if ((entity->dormant() && alpha[entity->index()] == 0) && !config_system.item.player_dormant)
		return;

	player_info_t info;
	interfaces::engine->get_player_info(entity->index(), &info);

	box bbox;
	if (!get_box(entity, bbox))
		return;

	if (config_system.item.player_box) {
		auto red = config_system.item.clr_box[0] * 255;
		auto green = config_system.item.clr_box[1] * 255;
		auto blue = config_system.item.clr_box[2] * 255;
	
		render::draw_outline(bbox.x - 1, bbox.y - 1, bbox.w + 2, bbox.h + 2, color(0, 0, 0, alpha[entity->index()] - 90));	
		render::draw_rect(bbox.x, bbox.y, bbox.w, bbox.h, color(red, green, blue, (alpha[entity->index()] - 90)));
		render::draw_outline(bbox.x + 1, bbox.y + 1, bbox.w - 2, bbox.h - 2, color(0, 0, 0, alpha[entity->index()] - 90));
	}

	if (config_system.item.player_health) {
		box temp(bbox.x - 4, bbox.y + (bbox.h - bbox.h * (utilities::math::clamp_value<int>(entity->health(), 0, 100.f) / 100.f)), 1, bbox.h * (utilities::math::clamp_value<int>(entity->health(), 0, 100) / 100.f) - (entity->health() >= 100 ? 0 : -1));
		box temp_bg(bbox.x - 4, bbox.y, 1, bbox.h);

		// change the color depending on the entity health
		auto health_color = color((255 - entity->health() * 2.55), (entity->health() * 2.55), 0, alpha[entity->index()]);


		int player_health = entity->health() > 100 ? 100 : entity->health();	
		int col[3] = { 0, 0, 0 };
		if (player_health >= 85)
		{
			col[0] = 83; col[1] = 200; col[2] = 84;
		}
		else if (player_health >= 70)
		{
			col[0] = 107; col[1] = 142; col[2] = 35;
		}
		else if (player_health >= 55)
		{
			col[0] = 173; col[1] = 255; col[2] = 47;
		}
		else if (player_health >= 40)
		{
			col[0] = 255; col[1] = 215; col[2] = 0;
		}
		else if (player_health >= 25)
		{
			col[0] = 255; col[1] = 127; col[2] = 80;
		}
		else if (player_health >= 10)
		{
			col[0] = 205; col[1] = 92; col[2] = 92;
		}
		else if (player_health >= 0) 
		{
			col[0] = 178; col[1] = 34; col[2] = 34;
		}
		color hec = color(col[0], col[1], col[2], alpha[entity->index()] -185);


		// clamp health (custom maps, danger zone, etc)
		if (entity->health() > 100)
			health_color = color(0, 255, 0);

		//draw actual dynamic hp bar
		if (entity->health() > 100) {
			render::draw_filled_rect(temp_bg.x - 1, temp_bg.y - 1, temp_bg.w + 3, temp_bg.h + 2, color(0, 0, 0, alpha[entity->index()] - 90));
		}
		if (entity->health() < 100) {
			render::draw_filled_rect(temp.x - 1, temp.y - 1, temp.w + 3, temp.h + 2, color(0, 0, 0, alpha[entity->index()] - 90));
			render::draw_text_string(temp.x - 6, temp.y - 3, render::fonts::watermark_font, std::to_string(entity->health()), true, color(255, 255, 255));
		}
		
		render::draw_filled_rect(temp.x, temp.y, temp.w + 2, temp.h, color(health_color));
		render::draw_filled_rect(temp.x, temp.y, temp.w + 2, temp.h, color(hec));
	}

	if (config_system.item.player_name) {
		auto red = config_system.item.clr_name[0] * 255;
		auto green = config_system.item.clr_name[1] * 255;
		auto blue = config_system.item.clr_name[2] * 255;
	
		std::string print(info.name); //no bot on name
		render::draw_text_string(bbox.x + (bbox.w / 2), bbox.y - 15, render::fonts::player_font, print, true, color(red, green, blue, alpha[entity->index()]));
	}
	{
		std::vector<std::pair<std::string, color>> flags;	

		if (info.fakeplayer)
			flags.push_back(std::pair<std::string, color>(std::string("BOT"), color(50, 250, 50, alpha[entity->index()])));   //bot check
		
		if (config_system.item.player_flags_armor && entity->has_helmet() && entity->armor() > 0)
			flags.push_back(std::pair<std::string, color>("HK", color(255, 255, 255, alpha[entity->index()])));		
		else if (config_system.item.player_flags_armor && !entity->has_helmet() && entity->armor() > 0)
			flags.push_back(std::pair<std::string, color>("K", color(255, 255, 255, alpha[entity->index()])));

		if (config_system.item.player_flags_money && entity->money())
			flags.push_back(std::pair<std::string, color>(std::string("$").append(std::to_string(entity->money())), color(120, 190, 10, alpha[entity->index()])));

		if (config_system.item.player_flags_scoped && entity->is_scoped())
			flags.push_back(std::pair<std::string, color>(std::string("ZOOM"), color(80, 160, 200, alpha[entity->index()])));

		if (config_system.item.player_flags_c4 && entity->has_c4())
			flags.push_back(std::pair<std::string, color>(std::string("BOMB"), color(255, 127, 36, alpha[entity->index()])));

		if (config_system.item.player_flags_kit && entity->has_defuser())
			flags.push_back(std::pair<std::string, color>(std::string("KIT"), color(28, 134, 238, alpha[entity->index()])));

		if (config_system.item.player_flags_flashed && entity->is_flashed())
			flags.push_back(std::pair<std::string, color>(std::string("FLASHED"), color(255, 255, 0, alpha[entity->index()])));

		//if (config_system.item.player_flags_reloading && (weapon_t*)entity->active_weapon()->is_reloading()) 
		//	flags.push_back(std::pair<std::string, color>(std::string("RELOADING"), color(0, 255, 10, alpha[entity->index()])));
		
		if (config_system.item.player_flags_reloading && (weapon_t*)!entity->active_weapon()->can_fire())
			flags.push_back(std::pair<std::string, color>(std::string("CANT FIRE"), color(155, 255, 10, alpha[entity->index()])));

		if (config_system.item.player_flags_defuse && entity->is_defusing() && !entity->has_defuser())
			flags.push_back(std::pair<std::string, color>(std::string("DEFUSING"), color(0, 191, 255, alpha[entity->index()])));

		if (config_system.item.player_flags_defuse && entity->is_defusing() && entity->has_defuser())
			flags.push_back(std::pair<std::string, color>(std::string("DEFUSING"), color(28, 134, 238, alpha[entity->index()])));

		if (config_system.item.player_flags_pos && entity->get_callout())
			flags.push_back(std::pair<std::string, color>(std::string(entity->get_callout()), color(60, 190, 90, alpha[entity->index()])));

		auto position = 0;
		for (auto text : flags) {
			render::draw_text_string(bbox.x + bbox.w + 3, bbox.y + position - 2, render::fonts::name_font_small, text.first, false, text.second);
			position += 10;
		}
	}

	if (config_system.item.player_weapon || config_system.item.player_weapon_icon) {
		auto red = config_system.item.clr_weapon[0] * 255;
		auto green = config_system.item.clr_weapon[1] * 255;
		auto blue = config_system.item.clr_weapon[2] * 255;

		int redz = config_system.item.clr_weapon_drop[0] * 255;
		int greenz = config_system.item.clr_weapon_drop[1] * 255;
		int bluez = config_system.item.clr_weapon_drop[2] * 255;
		
		auto weapon = entity->active_weapon();
		if (!weapon)return;
		
		auto info = weapon->get_weapon_data();
		if (!info)return;

		const char* hud_name = info->m_szHudName;
		if (!hud_name)return;

		wchar_t* weapon_name = interfaces::localize->find(hud_name);
		if (!weapon_name)return;
		_wcsupr(weapon_name);

		wchar_t icon_name[2] = { (wchar_t)visual::WeaponIcons[weapon->item_definition()], 0 };

		//	setup for drawing
		std::string ammo_s = "" + std::to_string(weapon->clip1_count());
		const auto ammo = weapon->clip1_count();
		const auto max_ammo = info->m_iMaxClip;
		auto width = bbox.w;
		width *= ammo;
		width /= max_ammo;

		if (!(info->m_iWeaponType == WEAPONTYPE_KNIFE || info->m_iWeaponType == WEAPONTYPE_GRENADE || info->m_iWeaponType == WEAPONTYPE_C4)) {
		// background
		render::draw_filled_rect(bbox.x - 1, bbox.y + bbox.h + 1, bbox.w + 2, 4, color(0, 0, 0, alpha[entity->index()] - 90));
		// ammo bar
		render::draw_filled_rect(bbox.x, bbox.y + bbox.h + 2, width, 2, color(redz, greenz, bluez, 240));
		//render::draw_text_string(bbox.x + width + 2, bbox.y + bbox.h + 2, render::fonts::watermark_font, ammo_s, true, color(255, 255, 255, 240));
		}
		
		int h_index = 0;
		if (config_system.item.player_weapon) {
			render::draw_text_wchar(bbox.x + (bbox.w / 2), bbox.h + (6 * h_index) + bbox.y + 4, render::fonts::name_font, weapon_name, true, color(red, green, blue, alpha[entity->index()]));
			h_index++;
		}
		if (config_system.item.player_weapon_icon) {
			render::draw_text_wchar(bbox.x + (bbox.w / 2), bbox.h + (6 * h_index) + bbox.y + 4, render::fonts::icon_font, icon_name, true, color(red, green, blue, alpha[entity->index()]));
			h_index++;
		}

	}
}

void visual::esp_player::skeleton(player_t* entity) {
	if (!config_system.item.skeleton)
		return;

	auto p_studio_hdr = interfaces::model_info->get_studio_model(entity->model());
	if (!p_studio_hdr)
		return;

	vec3_t v_parent, v_child, s_parent, s_child;

	
	/* old skeleton
	for (int i = 0; i < p_studio_hdr->bones_count; i++) {
		studio_bone_t* bone = p_studio_hdr->bone(i);
		if (!bone)
			return;

		if (bone && (bone->flags & BONE_USED_BY_HITBOX) && (bone->parent != -1)) {
			v_child = entity->get_bone_position(i);
			v_parent = entity->get_bone_position(bone->parent);

			if (math::world_to_screen(v_parent, s_parent) && math::world_to_screen(v_child, s_child))
				render::draw_line(s_parent[0], s_parent[1], s_child[0], s_child[1], color(255, 255, 255, alpha[entity->index()]));
		}
	}*/

	//new skeleton with fixed shoulders
	for (int j = 0; j < p_studio_hdr->bones_count; j++){
		studio_bone_t* bone = p_studio_hdr->bone(j);
		if (!bone)return;
		
		if (bone && (bone->flags & BONE_USED_BY_HITBOX) && (bone->parent != -1)){
			v_child = entity->get_bone_position(j);
			v_parent = entity->get_bone_position(bone->parent);

			int chest_bone = 6;  // Parameter of relevant Bone number
			//vec3_t breast_bone; // New reference Point for connecting many bones
			
			vec3_t upper_direction = entity->get_bone_position(chest_bone + 1) - entity->get_bone_position(chest_bone); // direction vector from chest to neck
			vec3_t breast_bone = entity->get_bone_position(chest_bone) + upper_direction / 2;
			
			vec3_t v_delta_child = v_child - breast_bone; // Used to determine close bones to the reference point
			vec3_t v_delta_parent = v_parent - breast_bone;

			// Eliminating / Converting all disturbing bone positions in three steps.
			if ((v_delta_parent.size() < 9 && v_delta_child.size() < 9))
				v_parent = breast_bone;

			if (j == chest_bone - 1)
				v_child = breast_bone;

			if (abs(v_delta_child.z) < 5 && (v_delta_parent.size() < 5 && v_delta_child.size() < 5) || j == chest_bone)
				continue;
	
			if (math::world_to_screen(v_parent, s_parent) && math::world_to_screen(v_child, s_child))
				render::draw_line(s_parent[0], s_parent[1], s_child[0], s_child[1], color(255, 255, 255, alpha[entity->index()]));
		}
	}
}













































































































































































































