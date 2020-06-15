#include "../features.hpp"

void visual::glow::run() {
	if (!config_system.item.visuals_enabled || !config_system.item.visuals_glow)
		return;

	auto local_player = reinterpret_cast<player_t*>(interfaces::entity_list->get_client_entity(interfaces::engine->get_local_player()));
	if (!local_player)
		return;

	i_material* mat_glow = interfaces::material_system->find_material("dev/glow_color", TEXTURE_GROUP_OTHER, true);
	interfaces::model_render->override_material(mat_glow);

	for (size_t i = 0; i < interfaces::glow_manager->size; i++) {
		auto& glow = interfaces::glow_manager->objects[i];
		if (glow.unused())
			continue;

		auto glow_entity = reinterpret_cast<player_t*>(glow.entity);
		auto client_class = glow_entity->client_class();
		if (!glow_entity || glow_entity->dormant())
			continue;

		auto is_enemy = glow_entity->is_enemy();
		auto is_teammate = !glow_entity->is_enemy();

		switch (client_class->class_id) {
		case class_ids::ccsplayer:
			if (is_enemy && config_system.item.visuals_glow_enemy) {
				glow.set(config_system.item.clr_glow[0], config_system.item.clr_glow[1], config_system.item.clr_glow[2], config_system.item.clr_glow[3]);
			}
			else if (is_teammate && config_system.item.visuals_glow_team) {
				glow.set(config_system.item.clr_glow_team[0], config_system.item.clr_glow_team[1], config_system.item.clr_glow_team[2], config_system.item.clr_glow_team[3]);
			}
			break;
		case class_ids::cplantedc4:
			if (config_system.item.visuals_glow_planted) {
				glow.set(config_system.item.clr_glow_planted[0], config_system.item.clr_glow_planted[1], config_system.item.clr_glow_planted[2], config_system.item.clr_glow_planted[3]);
			}
			break;
			//	grenedes
		case class_ids::chegrenade:
		case class_ids::cflashbang:
		case class_ids::cmolotovgrenade:
		case class_ids::cmolotovprojectile:
		case class_ids::cincendiarygrenade:
		case class_ids::cdecoygrenade:
		case class_ids::cdecoyprojectile:
		case class_ids::csmokegrenade:
		case class_ids::csmokegrenadeprojectile:
		case class_ids::particlesmokegrenade:
		case class_ids::cbasecsgrenade:
		case class_ids::cbasecsgrenadeprojectile:
		case class_ids::cbasegrenade:
		case class_ids::cbaseparticleentity:
		case class_ids::csensorgrenade:
		case class_ids::csensorgrenadeprojectile:
			if (config_system.item.visuals_glow_nades) {
				glow.set(config_system.item.clr_glow_dropped_nade[0], config_system.item.clr_glow_dropped_nade[1], config_system.item.clr_glow_dropped_nade[2], config_system.item.clr_glow_dropped_nade[3]);
			}
			break;
		}

		if (client_class->class_id == cak47 || client_class->class_id == cdeagle || client_class->class_id == cc4 || client_class->class_id == ceconentity ||
			client_class->class_id >= cweaponaug && client_class->class_id <= cweaponxm1014) {
			if (config_system.item.visuals_glow_weapons) {
				glow.set(config_system.item.clr_glow_dropped[0], config_system.item.clr_glow_dropped[1], config_system.item.clr_glow_dropped[2], config_system.item.clr_glow_dropped[3]);
			}
		}
	}
}













































































































































































































