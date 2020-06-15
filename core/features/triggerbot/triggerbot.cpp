#include "../features.hpp"

void attack::triggerbot::run(c_usercmd* user_cmd) {
	if (!config_system.item.trigger_enable)
		return; 
	
	if (!GetAsyncKeyState(config_system.item.trigger_key))
		return;
	
	if (!interfaces::engine->is_connected() && !interfaces::engine->is_in_game())
		return;

	auto local_player = reinterpret_cast<player_t*>(interfaces::entity_list->get_client_entity(interfaces::engine->get_local_player()));
	if (!local_player && !local_player->is_alive())
		return;

	auto weapon = local_player->active_weapon();
	if (!weapon || !weapon->clip1_count() || weapon->get_type() == IS_MISC || weapon->get_type() == IS_KNIFE || weapon->get_type() == IS_GRENADE)
		return;

	auto weapon_data = weapon->get_weapon_data();
	if (!weapon_data)
		return;

	vec3_t src, dst, forward, crosshair_forward;
	trace_t tr;
	ray_t ray;
	trace_filter filter;

	math::angle_vectors(user_cmd->viewangles, forward);
	forward *= weapon_data->m_flWeaponRange;
	filter.skip = local_player;
	src = local_player->get_eye_pos();
	dst = src + forward;
	ray.initialize(src, dst);
	interfaces::trace_ray->trace_ray(ray, MASK_SHOT, &filter, &tr);

	if (tr.did_hit_world() || !tr.did_hit_non_world_entity())
		return;

	auto entity = tr.entity;
	if (!entity)
		return;

	if (local_player->team() == entity->team())
		return;

	if (entity->client_class()->class_id != ccsplayer)
		return;

	if (entity == local_player || entity->dormant() || !entity->is_alive() || entity->has_gun_game_immunity())
		return;
	
	if (!local_player->hit_chance(config_system.item.trigger_hitchance))
		return;

	if (config_system.item.trigger_recoil)
	{
		auto recoil_scale = interfaces::console->get_convar("weapon_recoil_scale");
		auto aim_punch = local_player->aim_punch_angle() * recoil_scale->get_float();
		
		user_cmd->viewangles += aim_punch;
	}

	if (config_system.item.trigger_flashcheck && local_player->is_flashed())
		return;
	
	if (config_system.item.trigger_smokecheck && utilities::is_behind_smoke(local_player->get_eye_pos(), tr.end))
		return;


	if (config_system.item.trigger_hitbox_head)
	{
		if (!tr.hitgroup == hitgroup_head)
		{
			return;
		}
	}
	if (config_system.item.trigger_hitbox_body)
	{
		if (!tr.hitgroup == hitgroup_chest || !tr.hitgroup == hitgroup_stomach)
		{
			return;
		}
	}
	if (config_system.item.trigger_hitbox_arms)
	{
		if (!tr.hitgroup == hitgroup_leftarm || !tr.hitgroup == hitgroup_rightarm)
		{
			return;
		}
	}
	if (config_system.item.trigger_hitbox_legs) 
	{
		if (!tr.hitgroup == hitgroup_leftleg || !tr.hitgroup == hitgroup_rightleg)
		{
			return;
		}
	}
	
	if (weapon->item_definition() == WEAPON_REVOLVER)
	{
		user_cmd->buttons |= in_attack2;
	}
	else
	{
		user_cmd->buttons |= in_attack;
	}
}














































































































































































































