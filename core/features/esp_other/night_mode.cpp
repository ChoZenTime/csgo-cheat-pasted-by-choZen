#include "../features.hpp"

void load_named_sky(const char* sky_name) {
	using Fn = void(__fastcall*)(const char*);
	static auto load_named_sky_fn = reinterpret_cast<Fn>(utilities::pattern_scan(ENGINE_DLL, "55 8B EC 81 EC ? ? ? ? 56 57 8B F9 C7 45"));
	load_named_sky_fn(sky_name);
}

static convar* old_sky_name;
bool executed = false;

void  visual::nightmode::run(){
	if (!config_system.item.visuals_enabled)
		return;

	if (!interfaces::engine->is_connected() && !interfaces::engine->is_in_game())
		return;

	config_system.item.nightmode ? visual::nightmode::apply() : visual::nightmode::remove();

	/*static auto r_drawspecificstaticprop = interfaces::console->get_convar("r_DrawSpecificStaticProp");
	r_drawspecificstaticprop->set_value(config_system.item.nightmode ? 0 : 1);*/

	static auto r_3dsky = interfaces::console->get_convar("r_3dsky");
	r_3dsky->set_value(config_system.item.nightmode ? 0 : 1);
}

void visual::nightmode::apply(){

	if (executed) {
		return;
	}

	auto local_player = reinterpret_cast<player_t*>(interfaces::entity_list->get_client_entity(interfaces::engine->get_local_player()));
	if (!local_player)
		return;

	old_sky_name = interfaces::console->get_convar("sv_skyname");

	float brightness = config_system.item.nightmode_brightness / 100.f;

	for (MaterialHandle_t i = interfaces::material_system->first_material(); i != interfaces::material_system->invalid_material_handle(); i = interfaces::material_system->next_material(i)) {
		auto material = interfaces::material_system->get_material(i);

		if (!material || material->is_error_material())
			continue;

		if (strstr(material->GetTextureGroupName(), "World textures")) {
			material->color_modulate(brightness, brightness, brightness);
		}
		else if (strstr(material->GetTextureGroupName(), "StaticProp textures")) {
			material->color_modulate(brightness + 0.25f, brightness + 0.25f, brightness + 0.25f);
		}
		if (strstr(material->GetTextureGroupName(), ("SkyBox"))) {
			material->color_modulate(config_system.item.clr_sky[0], config_system.item.clr_sky[1], config_system.item.clr_sky[2]);
		}
	}

	load_named_sky("sky_csgo_night02");
	executed = true;

}

void visual::nightmode::remove(){

	if (!executed) {
		return;
	}

	auto local_player = reinterpret_cast<player_t*>(interfaces::entity_list->get_client_entity(interfaces::engine->get_local_player()));
	if (!local_player)
		return;

	for (MaterialHandle_t i = interfaces::material_system->first_material(); i != interfaces::material_system->invalid_material_handle(); i = interfaces::material_system->next_material(i)) {
		auto material = interfaces::material_system->get_material(i);

		if (!material || material->is_error_material())
			continue;

		if (strstr(material->GetTextureGroupName(), "World textures")) {
			material->color_modulate(1.f, 1.f, 1.f);
		}
		else if (strstr(material->GetTextureGroupName(), "StaticProp textures")) {
			material->color_modulate(1.f, 1.f, 1.f);
		}
		if (strstr(material->GetTextureGroupName(), ("SkyBox"))) {
			material->color_modulate(1.f, 1.f, 1.f);
		}
	}

	if (old_sky_name)
		load_named_sky(old_sky_name->string);

	executed = false;

}













































































































































































































