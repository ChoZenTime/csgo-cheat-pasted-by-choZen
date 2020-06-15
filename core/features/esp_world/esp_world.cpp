#include "../features.hpp"

void visual::esp_world::run() {
	if (!config_system.item.visuals_enabled || (config_system.item.anti_screenshot && interfaces::engine->is_taking_screenshot()))
		return;

	auto local_player = reinterpret_cast<player_t*>(interfaces::entity_list->get_client_entity(interfaces::engine->get_local_player()));
	if (!local_player)
		return;

	for (int i = 1; i < interfaces::entity_list->get_highest_index(); i++) {
		auto entity = reinterpret_cast<player_t*>(interfaces::entity_list->get_client_entity(i));
		if (!entity)
			continue;
		
		if (entity && entity != local_player) {

			if (config_system.item.dropped_weapons || config_system.item.dropped_weapons_icon) {
				dropped_weapon(entity);
			}

			if (config_system.item.projectiles || config_system.item.projectiles_icon) {
				draw_throwable(entity);
			}
		}
	}
}

void visual::esp_world::dropped_weapon(player_t* entity) {

	auto red = config_system.item.clr_weapon_drop[0] * 255;
	auto green = config_system.item.clr_weapon_drop[1] * 255;
	auto blue = config_system.item.clr_weapon_drop[2] * 255;
	auto alpha = config_system.item.clr_weapon_drop[3] * 255;

	auto redz = config_system.item.clr_weapon[0] * 255;
	auto greenz = config_system.item.clr_weapon[1] * 255;
	auto bluez = config_system.item.clr_weapon[2] * 255;

	auto client_class = entity->client_class();
	if (!client_class)return;

	if (client_class->class_id == class_ids::cplantedc4)
		if (entity->c4_is_ticking())return;

	const model_t* model = entity->model();
	if (!model)return;

	const char* get_model = interfaces::model_info->get_model_name(model);
	if (!get_model)return;

	box dropped_box;
	if (!get_box(entity, dropped_box))
		return;

	const auto owner = reinterpret_cast<player_t*>(interfaces::entity_list->get_client_entity_handle(entity->owner_handle()));
	
	if (!owner && (client_class->class_id != cbaseweaponworldmodel && (
		client_class->class_id >= cweaponaug && client_class->class_id <= cweaponxm1014 ||
		client_class->class_id == cdeagle || client_class->class_id == cak47 ))) {
	
		auto weapon = (weapon_t*)entity;
		if (!weapon)return;
		
		auto info = weapon->get_weapon_data();	
		if (!info)return;
		
		auto hud_name = info->m_szHudName;
		if (!hud_name)return;

		wchar_t* weapon_name = interfaces::localize->find(hud_name);
		if (!weapon_name)return;
		_wcsupr(weapon_name);
		
		wchar_t icon_name[2] = { (wchar_t)visual::WeaponIcons[weapon->item_definition()], 0 };
		
		//	setup for drawing
		std::string ammo_s = std::to_string(weapon->clip1_count());
		const auto ammo = weapon->clip1_count();
		const auto max_ammo = info->m_iMaxClip;
		auto width = dropped_box.w;
		width *= ammo;
		width /= max_ammo;

		render::draw_rect(dropped_box.x, dropped_box.y, dropped_box.w, dropped_box.h, color(red, green, blue, alpha));

		if (!(info->m_iWeaponType == WEAPONTYPE_KNIFE || info->m_iWeaponType == WEAPONTYPE_C4 || info->m_iWeaponType == WEAPONTYPE_GRENADE)) {
			// background
			render::draw_filled_rect(dropped_box.x - 1, dropped_box.y + dropped_box.h + 1, dropped_box.w + 2, 4, color(0, 0, 0, alpha - 90));
			// ammo bar
			render::draw_filled_rect(dropped_box.x, dropped_box.y + dropped_box.h + 2, width, 2, color(redz, greenz, bluez, 240));
			//render::draw_text_string(dropped_box.x + width + 2, dropped_box.y  + dropped_box.h + 2, render::fonts::watermark_font, ammo_s, true, color(255, 255, 255, 240));
		}
		int h_index = 0;
		if (config_system.item.dropped_weapons) {
			render::draw_text_wchar(dropped_box.x + (dropped_box.w / 2), dropped_box.h + (6 * h_index) + dropped_box.y + 4, render::fonts::name_font, weapon_name, true, color(red, green, blue, alpha));
			h_index++;
		}
		if (config_system.item.dropped_weapons_icon) {
			render::draw_text_wchar(dropped_box.x + (dropped_box.w / 2), dropped_box.h + (6 * h_index) + dropped_box.y + 3, render::fonts::icon_font, icon_name, true, color(red, green, blue, alpha));
			h_index++;
		}
	}

	if (!owner && (client_class->class_id == ceconentity))
	{
		//if (!owner && (strstr(get_model, "w_defuser.mdl")))
		auto weapon = (weapon_t*)entity;
		if (!weapon)return;

		auto info = weapon->get_weapon_data();
		if (!info)return;

		auto weapon_name = info->m_szHudName;
		if (!weapon_name)return;

		wchar_t* localised_name = interfaces::localize->find(weapon_name);
		if (!localised_name)return;
		_wcsupr(localised_name);

		render::draw_rect(dropped_box.x, dropped_box.y, dropped_box.w, dropped_box.h, color(red, green, blue, alpha));
		
		int h_index = 0;
		if (config_system.item.dropped_weapons) {
			render::draw_text_wchar(dropped_box.x + (dropped_box.w / 2), dropped_box.h + (6 * h_index) + dropped_box.y + 4, render::fonts::name_font, localised_name, true, color(red, green, blue, alpha));
			h_index++;
		}
		if (config_system.item.dropped_weapons_icon) {
			render::draw_text_string(dropped_box.x + (dropped_box.w / 2), dropped_box.h + (6 * h_index) + dropped_box.y + 3, render::fonts::icon_font, "n", true, color(red, green, blue, alpha));
			h_index++;
		}
	}

	if (!owner && (client_class->class_id == cchicken))
	{	
		render::draw_rect(dropped_box.x, dropped_box.y, dropped_box.w, dropped_box.h, color(red, green, blue, alpha));

		if (config_system.item.dropped_weapons) 
			render::draw_text_string(dropped_box.x + (dropped_box.w / 2), dropped_box.h + dropped_box.y + 4, render::fonts::name_font, "Chicken", true, color(red, green, blue, alpha));
	}
	
	if (!owner && (client_class->class_id == chostage))
	{
		render::draw_rect(dropped_box.x, dropped_box.y, dropped_box.w, dropped_box.h, color(red, green, blue, alpha));

		if (config_system.item.dropped_weapons)
			render::draw_text_string(dropped_box.x + (dropped_box.w / 2), dropped_box.h + dropped_box.y + 4, render::fonts::name_font, "Hostage", true, color(red, green, blue, alpha));
	}

	if (!owner && (client_class->class_id == cc4))
	{
		auto weapon = (weapon_t*)entity;
		if (!weapon)return;

		auto info = weapon->get_weapon_data();
		if (!info)return;

		auto weapon_name = info->m_szHudName;
		if (!weapon_name)return;

		wchar_t* localised_name = interfaces::localize->find(weapon_name);
		if (!localised_name)return;
		_wcsupr(localised_name);

		render::draw_rect(dropped_box.x, dropped_box.y, dropped_box.w, dropped_box.h, color(red, green, blue, alpha));

		int h_index = 0;
		if (config_system.item.dropped_weapons) {
			render::draw_text_wchar(dropped_box.x + (dropped_box.w / 2), dropped_box.h + (6 * h_index) + dropped_box.y + 4, render::fonts::name_font, localised_name, true, color(red, green, blue, alpha));
			h_index++;
		}
		if (config_system.item.dropped_weapons_icon) {
			render::draw_text_string(dropped_box.x + (dropped_box.w / 2), dropped_box.h + (6 * h_index) + dropped_box.y + 3, render::fonts::icon_font, "j", true, color(red, green, blue, alpha));
			h_index++;
		}
	}
}

void visual::esp_world::draw_throwable(player_t* entity) {
	const model_t* nadeModel = entity->model();
	if (!nadeModel)
		return;

	studio_hdr_t* hdr = interfaces::model_info->get_studio_model(nadeModel);
	if (!hdr)
		return;

	if (!strstr(hdr->name_char_array, "thrown") && !strstr(hdr->name_char_array, "dropped"))
		return;

	std::string grenade_name = "", grenade_icon = "";
	color grenade_color = NULL;
	int alpha = config_system.item.clr_weapon_drop[3] * 255;

	i_material* mats[32];
	interfaces::model_info->get_model_materials(nadeModel, hdr->textures_count, mats);

	vec3_t grenade_position, grenade_origin;
	grenade_origin = entity->origin();
	if (!math::world_to_screen(grenade_origin, grenade_position))
		return;

	int h_index = 0;
	for (int i = 0; i < hdr->textures_count; i++) {
		i_material* mat = mats[i];
		if (!mat)
			continue;
		
		if (strstr(mat->get_name(), ("flashbang"))) {
			grenade_name = "FLASH";
			grenade_icon = "G"; //i = copyright
			grenade_color = color(255, 255, 10, alpha);
			break;
		}
		else if (strstr(mat->get_name(), ("m67_grenade")) || strstr(mat->get_name(), ("hegrenade"))) {
			grenade_name = "GRENADE";
			grenade_icon = "H"; //j =big bomb
			grenade_color = color(255, 0, 0, alpha);
			break;
		}
		else if (strstr(mat->get_name(), ("smoke"))) {
			grenade_name = "SMOKE";
			grenade_icon = "P"; // k = hostage
			grenade_color = color(0, 155, 255, alpha);
			auto time = interfaces::globals->interval_per_tick * (interfaces::globals->tick_count - entity->smoke_grenade_tick_begin());

			if (!(18 - time < 0)) {
				render::draw_filled_rect(grenade_position.x - 18, grenade_position.y + (10 * h_index), 36, 3, color(10, 10, 10, 180));
				render::draw_filled_rect(grenade_position.x - 18, grenade_position.y + (10 * h_index), time * 2, 3, color(167, 24, 71, 255));
				h_index++;
			}
			break;
		}
		else if (strstr(mat->get_name(), ("decoy"))) {
			grenade_name = "DECOY";
			grenade_icon = "H"; // m = kevler
			grenade_color = color(192, 192, 192, alpha);
			break;
		}
		else if (strstr(mat->get_name(), ("incendiary"))) {
			grenade_name = "FIRE-INC";
			grenade_icon = "G"; // n = defkit
			grenade_color = color(255, 0, 255, alpha);
			break;
		}
		else if (strstr(mat->get_name(), ("molotov"))) {
			grenade_name = "FIRE-MOLO";
			grenade_icon = "H"; // l = kevler+helm
			grenade_color = color(255, 0, 255, alpha);
			break;
		}
	}

	box grenade_box;
	if (!get_box(entity, grenade_box))
		return;

	render::draw_rect(grenade_box.x, grenade_box.y, grenade_box.w, grenade_box.h, grenade_color);

	if (config_system.item.projectiles) {
		render::draw_text_string(grenade_box.x + (grenade_box.w / 2), grenade_box.h + (6 * h_index) + grenade_box.y + 2, render::fonts::name_font, grenade_name.c_str(), true, grenade_color);
		h_index++;
	}
	if (config_system.item.projectiles_icon) {
		render::draw_text_string(grenade_box.x + (grenade_box.w / 2), grenade_box.h + (6 * h_index) + grenade_box.y + 2, render::fonts::icon_font, grenade_icon.c_str(), true, grenade_color);
		h_index++;
	}
}













































































































































































































