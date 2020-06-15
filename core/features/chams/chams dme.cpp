#include "../features.hpp"

i_material* visual::chams::create_material(shader_type_t shade, bool ignorez, bool wireframe) {
	static const std::string material_name = "game_material.vmt";
	std::string shade_type;

	switch (shade) {
	case shader_type_t::VertexLitGeneric: shade_type = "VertexLitGeneric"; break;
	case shader_type_t::UnlitGeneric: shade_type = "UnlitGeneric"; break;
	case shader_type_t::Modulate: shade_type = "Modulate"; break;
	default: break;
	}

	std::string material_data;

	material_data += "\"" + shade_type + "\"\n{\n";

	material_data += "\t\"$model\" \"1\"\n";
	material_data += "\t\"$flat\" \"1\"\n";
	material_data += "\t\"$selfillum\" \"1\"\n";
	material_data += "\t\"$selfillumtint\" \"[8 8 8]\"\n";
	material_data += "\t\"$halflambert\" \"1\"\n";
	material_data += "\t\"$wireframe\" \"" + std::to_string(wireframe) + "\"\n";
	material_data += "\t\"$ignorez\" \"" + std::to_string(ignorez) + "\"\n";

	material_data += "\t\"$mod2x\" \"1\"\n";
	material_data += "\t\"$nocull\" \"1\"\n"; // for Modulate shader	

	material_data += "\"Proxies\"\n{\n";
	// add proxies here.
	material_data += "}\n"; // end of proxies.

	material_data += "}\n"; // end of shader type.

	auto kv = static_cast<c_key_values*>(malloc(36));
	kv->init(shade_type.c_str());
	kv->load_from_buffer(material_name.c_str(), material_data.c_str());

	return interfaces::material_system->create_material(material_name.c_str(), kv);
}

void visual::chams::run(i_mat_render_context* ctx, const draw_model_state_t& state, const model_render_info_t& info, matrix_t* bone_to_world) {
	
	static auto draw_model_execute_original = reinterpret_cast<hooks::draw_model_execute::fn>(hooks::get_virtual(interfaces::model_render, 21));
	
	if (!interfaces::engine->is_connected() && !interfaces::engine->is_in_game())
		return;

	if (!csgo::local_player )
		return;

	static i_material* mat = nullptr;
	static i_material* zmat = nullptr;

	static i_material* norm = create_material(shader_type_t::VertexLitGeneric, false, false);
	static i_material* znorm = create_material(shader_type_t::VertexLitGeneric, true, false);
	
	static i_material* flat = create_material(shader_type_t::UnlitGeneric, false, false);
	static i_material* zflat = create_material(shader_type_t::UnlitGeneric, true, false);
	
	static i_material* wire = create_material(shader_type_t::UnlitGeneric, false, true);
	static i_material* zwire = create_material(shader_type_t::UnlitGeneric, true, true);


	switch (config_system.item.cham_materials) {
	case 0:
		mat = norm;
		zmat = znorm;
		break;
	case 1:
		mat = flat;
		zmat = zflat;
		break;
	case 2:
		mat = wire;
		zmat = zwire;
		break;
	default:
		mat = norm;
		zmat = znorm;
		break;
	}

	auto model_name = interfaces::model_info->get_model_name((model_t*)info.model);
	auto entity = reinterpret_cast<player_t*>(interfaces::entity_list->get_client_entity(info.entity_index));
	if (!model_name || !entity)
		return;

	if (model_name && strstr(model_name, "models/player")) {
		
		if (config_system.item.backtrack_visualize) {

			if (!csgo::local_player->is_alive())
				return;

			if (entity && entity->is_alive() && !entity->dormant()) {
				int i = entity->index();

				if (csgo::local_player && csgo::local_player->is_alive() && entity->team() != csgo::local_player->team()) {

					auto record = &attack::backtrack::records[info.entity_index];
					if (!record || record->empty() || !record->back().matrix)
						return;

					if (attack::backtrack::valid_tick(record->front().simulation_time) == NULL)
						return;

					if (record && record->size() && attack::backtrack::valid_tick(record->front().simulation_time)) {
						interfaces::render_view->modulate_color(config_system.item.clr_sleeve_chams);
						interfaces::render_view->set_blend(config_system.item.clr_sleeve_chams[3]);
						interfaces::model_render->override_material(mat);

						draw_model_execute_original(interfaces::model_render, ctx, state, info, record->back().matrix);
					}
				}
			}
		}
		if (config_system.item.vis_chams_vis) {
			
			if (entity->is_enemy()) {
				
				if (config_system.item.vis_chams_invis) {
					interfaces::render_view->modulate_color(config_system.item.clr_chams_invis);
					interfaces::render_view->set_blend(config_system.item.clr_chams_invis[3]);
					interfaces::model_render->override_material(zmat);
					
					draw_model_execute_original(interfaces::model_render, ctx, state, info, bone_to_world);
				}
				
				interfaces::render_view->modulate_color(config_system.item.clr_chams_vis);
				interfaces::render_view->set_blend(config_system.item.clr_chams_vis[3]);
				interfaces::model_render->override_material(mat);
				
				draw_model_execute_original(interfaces::model_render, ctx, state, info, bone_to_world);
			}
		}
		if (config_system.item.vis_chams_vis_teammate) {
			
			if (!entity->is_enemy()) {

				if (config_system.item.vis_chams_invis_teammate) {		
					interfaces::render_view->modulate_color(config_system.item.clr_chams_invis_teammate);
					interfaces::render_view->set_blend(config_system.item.clr_chams_invis_teammate[3]);
					interfaces::model_render->override_material(zmat);
					
					draw_model_execute_original(interfaces::model_render, ctx, state, info, bone_to_world);
				}
				
				interfaces::render_view->modulate_color(config_system.item.clr_chams_vis_teammate);
				interfaces::render_view->set_blend(config_system.item.clr_chams_vis_teammate[3]);
				interfaces::model_render->override_material(mat);
				
				draw_model_execute_original(interfaces::model_render, ctx, state, info, bone_to_world);	
			
			}
		}
		if (!entity->alive()|| !entity->is_alive() || entity->client_class()->class_id == ccsragdoll) {
			int x = 0.5;
			float clr_chams_dead[4]{ x,x,x,x };

			interfaces::render_view->modulate_color(clr_chams_dead);
			interfaces::render_view->set_blend(clr_chams_dead[3]);
			interfaces::model_render->override_material(mat);

			draw_model_execute_original(interfaces::model_render, ctx, state, info, bone_to_world);
		}
	}
}
















































































































































































































