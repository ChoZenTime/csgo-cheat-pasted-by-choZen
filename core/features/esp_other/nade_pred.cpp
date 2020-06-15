#include "../features.hpp"


void visual::nade_pred::tick(int buttons)
{
	if (!config_system.item.nade_pred)
		return;

	auto primary_attack = buttons & in_attack;
	auto secondary_attack = buttons & in_attack2;

	if (primary_attack && secondary_attack)
	{
		act = ACT_DROP;
	}
	else if (secondary_attack)
	{
		act = ACT_THROW;
	}
	else if (primary_attack)
	{
		act = ACT_LOB;
	}
	else
	{
		act = ACT_NONE;
	}
}

void visual::nade_pred::view()
{
	if (!config_system.item.nade_pred)
		return;

	auto local_player = reinterpret_cast<player_t*>(interfaces::entity_list->get_client_entity(interfaces::engine->get_local_player()));
	if (!local_player)
		return;

	auto weapon = local_player->active_weapon();
	if (!weapon)
		return;

	if ((weapon->get_type() == IS_GRENADE) && act != ACT_NONE)
	{
		vec3_t angles;
		interfaces::engine->get_view_angles(angles);

		auto pWeaponClass = weapon->client_class();
		if (!pWeaponClass)
		{
			type = -1;
			simulate(angles, local_player);
		}
		else
		{
			type = pWeaponClass->class_id;
			simulate(angles, local_player);
		}
	}
	else
	{
		type = -1;
	}
}

void visual::nade_pred::paint()
{
	if (!config_system.item.nade_pred)
		return;

	auto local_player = reinterpret_cast<player_t*>(interfaces::entity_list->get_client_entity(interfaces::engine->get_local_player()));
	if (!local_player)
		return;

	auto weapon = local_player->active_weapon();
	if (!weapon)
		return;

	if ((type) && path.size() > 1 && act != ACT_NONE && (weapon->get_type() == IS_GRENADE))
	{
		vec3_t ab, cd;
		vec3_t prev = path[0];
		for (auto it = path.begin(), end = path.end(); it != end; ++it)
		{
			if (math::world_to_screen(prev, ab) && math::world_to_screen(*it, cd))
			{
				auto red = config_system.item.clr_nade_pred[0] * 255;
				auto green = config_system.item.clr_nade_pred[1] * 255;
				auto blue = config_system.item.clr_nade_pred[2] * 255;
				auto alpha = config_system.item.clr_nade_pred[3] * 255;
				interfaces::surface->set_drawing_color(red, green, blue, alpha);
				interfaces::surface->draw_line(ab[0], ab[1], cd[0], cd[1]);
			}
			prev = *it;
		}

		for (auto it = other_collisions.begin(), end = other_collisions.end(); it != end; ++it)
		{
			render::draw_3d_cube(2.f, it->second, it->first, 0, 255, 0, 200);		
		}

		render::draw_3d_cube(2.f, other_collisions.rbegin()->second, other_collisions.rbegin()->first, 255, 0, 0, 200);

		std::string entity_name;
		auto bestdmg = 0;
		auto redcol = color(255, 0, 0, 255);
		auto greencol = color(25, 255, 25, 255);
		auto yellowgreencol = color(177, 253, 2, 255);
		auto yellowcol = color(255, 255, 0, 255);
		auto orangecol = color(255, 128, 0, 255);
		auto* bestcolor = &redcol;

		vec3_t endpos = path[path.size() - 1];
		vec3_t absendpos = endpos;

		float totaladded = 0.0f;

		while (totaladded < 30.0f) {
			if (interfaces::trace_ray->get_point_contents(endpos) == CONTENTS_EMPTY)
				break;

			totaladded += 2.0f;
			endpos.z += 2.0f;
		}

		auto pWeapon = local_player->active_weapon();
		int weap_id = pWeapon->item_definition();

		if (pWeapon &&
			weap_id == WEAPON_HEGRENADE ||
			weap_id == WEAPON_MOLOTOV ||
			weap_id == WEAPON_INCGRENADE) {
			for (int i = 1; i < 64; i++) {
				player_t* entity = (player_t*)interfaces::entity_list->get_client_entity(i);

				if (!entity || entity->team() == local_player->team())
					continue;

				float dist = (entity->origin() - endpos).length();

				if (dist < 350.0f) {
					trace_filter_skip_one_entity filter(local_player);
					ray_t ray;
					vec3_t NadeScreen;
					math::world_to_screen(endpos, NadeScreen);

					vec3_t pelvis_pos = entity->get_bone_position(hitbox_pelvis);
					ray.initialize(endpos, pelvis_pos);
					trace_t ptr;
					interfaces::trace_ray->trace_ray(ray, MASK_SHOT, &filter, &ptr);

					if (ptr.entity == entity) {
						vec3_t pelvis_screen;

						math::world_to_screen(pelvis_pos, pelvis_screen);

						static float a = 105.0f;
						static float b = 25.0f;
						static float c = 140.0f;

						float d = ((((entity->origin()) - prev).length() - b) / c);
						float damage = a * exp(-d * d);
						auto dmg = max(static_cast<int>(ceilf(utilities::csgo_armor(damage, entity->armor()))), 0);
						
						color* destcolor = dmg >= 65 ? &redcol : dmg >= 40.0f ? &orangecol : dmg >= 20.0f ? &yellowgreencol : &greencol;

						if (dmg > bestdmg) {
							entity_name = entity->get_name();
							bestcolor = destcolor;
							bestdmg = dmg;
						}
					}
				}
			}
		}

		if (bestdmg > 0.f) {
			if (weap_id != WEAPON_HEGRENADE)
			{
				if (math::world_to_screen(prev, cd))
				render::draw_text_string(cd[0], cd[1] - 10, render::fonts::name_font_big, firegrenade_didnt_hit ? "No collisions" : (entity_name + " will be burnt.").c_str(), true, *bestcolor);
			}
			else
			{
				if (math::world_to_screen(prev, cd)) 		
					render::draw_text_string(cd[0] + 20 , cd[1] - 10, render::fonts::name_font_bigger, ("-" + std::to_string(bestdmg)).c_str(), true, *bestcolor);
				
				if (math::world_to_screen(*path.begin(), cd))	
					render::draw_text_string(cd[0], cd[1] - 10, render::fonts::name_font_big, ("Most damage dealt to: " + entity_name + " -" + std::to_string(bestdmg)).c_str(), true, *bestcolor);
			}
		}
	}
}

void visual::nade_pred::setup(player_t* pl, vec3_t& source_vector, vec3_t& throw_vector, const vec3_t& angEyeangles)
{
	vec3_t throw_angles = angEyeangles;
	float pitch = throw_angles.x;

	if (pitch <= 90.0f)
	{
		if (pitch < -90.0f)
		{
			pitch += 360.0f;
		}
	}
	else
	{
		pitch -= 360.0f;
	}
	float a = pitch - (90.0f - fabs(pitch)) * 10.0f / 90.0f;
	throw_angles.x = a;

	// Gets ThrowVelocity from weapon files
	// Clamped to [15,750]
	float throw_velocity = 750.0f * 0.9f;

	// Do magic on member of grenade object [esi+9E4h]
	// m1=1  m1+m2=0.5  m2=0
	static const float power[] = { 1.0f, 1.0f, 0.5f, 0.0f };
	float b = power[act];
	// Clamped to [0,1]
	b = b * 0.7f;
	b = b + 0.3f;
	throw_velocity *= b;

	vec3_t forward, right, up;
	math::angle_vectors(throw_angles, forward, right, up);

	source_vector = pl->origin();
	source_vector += pl->view_offset();
	float off = (power[act] * 12.0f) - 12.0f;
	source_vector.z += off;

	// Game calls UTIL_TraceHull here with hull and assigns source_vector tr.endpos
	trace_t tr;
	vec3_t destiny = source_vector;
	destiny.mul_add(destiny, forward, 22.0f);
	trace_hull(source_vector, destiny, tr);

	// After the hull trace it moves 6 units back along forward
	// source_vector = tr.endpos - forward * 6
	vec3_t vecBack = forward; vecBack *= 6.0f;
	source_vector = tr.end;
	source_vector -= vecBack;

	// Finally calculate velocity
	throw_vector = pl->velocity(); throw_vector *= 1.25f;
	throw_vector.mul_add(throw_vector, forward, throw_velocity);
}

void visual::nade_pred::simulate(vec3_t& angles, player_t* pLocal)
{
	vec3_t source_vector, throw_vector;
	setup(pLocal, source_vector, throw_vector, angles);

	float interval = interfaces::globals->interval_per_tick;

	// Log positions 20 times per sec
	int logstep = static_cast<int>(0.05f / interval);
	int logtimer = 0;

	path.clear();
	other_collisions.clear();
	
	auto red = config_system.item.clr_nade_pred[0] * 255;
	auto green = config_system.item.clr_nade_pred[1] * 255;
	auto blue = config_system.item.clr_nade_pred[2] * 255;
	auto alpha = config_system.item.clr_nade_pred[3] * 255;
	tracer_color = color(red, green, blue, alpha);
	
	for (unsigned int i = 0; i < path.max_size() - 1; ++i)
	{
		if (!logtimer)
			path.push_back(source_vector);

		int s = step(source_vector, throw_vector, i, interval);
		if ((s & 1) || throw_vector == vec3_t(0, 0, 0))
			break;

		// Reset the log timer every logstep OR we bounced
		if ((s & 2) || logtimer >= logstep) logtimer = 0;
		else ++logtimer;
	}
	path.push_back(source_vector);
}

int visual::nade_pred::step(vec3_t& source_vector, vec3_t& throw_vector, int tick, float interval)
{
	auto local_player = reinterpret_cast<player_t*>(interfaces::entity_list->get_client_entity(interfaces::engine->get_local_player()));

	// Apply gravity
	vec3_t move;
	add_gravity_move(move, throw_vector, interval, false);

	// Push entity
	trace_t tr;
	push_entity(source_vector, move, tr);

	int result = 0;
	// Check ending conditions
	if (check_detonate(throw_vector, tr, tick, interval))
	{
		result |= 1;
	}

	// Resolve collisions
	if (tr.fraction != 1.0f)
	{
		result |= 2; // Collision!
		resolve_fly_collision_custom(tr, throw_vector, interval);
	}

	if (tr.entity && ((player_t*)tr.entity)->is_player())
	{
		auto red = config_system.item.clr_nade_pred[0] * 255;
		auto green = config_system.item.clr_nade_pred[1] * 255;
		auto blue = config_system.item.clr_nade_pred[2] * 255;
		auto alpha = config_system.item.clr_nade_pred[3] * 255;
		tracer_color = color(red, green, blue, alpha);
	}

	if ((result & 1) || throw_vector == vec3_t(0, 0, 0) || tr.fraction != 1.0f)
	{
		vec3_t angles;
		math::vector_angles((tr.end - tr.start).normalized(), angles);
		other_collisions.push_back(std::make_pair(tr.end, angles));
	}

	// Set new position
	source_vector = tr.end;

	return result;
}

bool visual::nade_pred::check_detonate(const vec3_t& throw_vector, const trace_t& tr, int tick, float interval)
{
	firegrenade_didnt_hit = false;
	if (!type)
		return false;
	switch (type)
	{
	case csmokegrenade:
	case cdecoygrenade:
		// Velocity must be <0.1, this is only checked every 0.2s
		if (throw_vector.length_2d_sqr() < 0.1f)
		{
			int det_tick_mod = static_cast<int>(0.2f / interval);
			return !(tick % det_tick_mod);
		}
		return false;
		break;

		// TIMES AREN'T COMPLETELY RIGHT FROM WHAT I'VE SEEN ! ! ! 
	case cmolotovgrenade:
	case cincendiarygrenade:
		// Detonate when hitting the floor
		if (tr.fraction != 1.0f && tr.plane.normal.z > 0.7f)
			return true;
		return (float)tick * interval > interfaces::console->get_convar("molotov_throw_detonate_time")->get_float();
		break;
		// OR we've been flying for too long

	case cflashbang:
	case chegrenade: {
		// Pure timer based, detonate at 1.5s, checked every 0.2s
		firegrenade_didnt_hit = static_cast<float>(tick) * interval > 1.5f && !(tick % static_cast<int>(0.2f / interval));
		return firegrenade_didnt_hit;
	}break;
	default:
		assert(false);
		return false;
		break;
	}
}

void visual::nade_pred::trace_hull(vec3_t& src, vec3_t& end, trace_t& tr)
{
	auto local_player = reinterpret_cast<player_t*>(interfaces::entity_list->get_client_entity(interfaces::engine->get_local_player()));
	if (!local_player)
		return;

	// Setup grenade hull
	trace_filter_skip_one_entity filter(local_player);
	ray_t ray;
	ray.initialize(src, end, vec3_t(-2.0f, -2.0f, -2.0f), vec3_t(2.0f, 2.0f, 2.0f));
	//ray.initialize(src, end);

	interfaces::trace_ray->trace_ray(ray, MASK_SOLID, &filter, &tr);
}

void visual::nade_pred::add_gravity_move(vec3_t& move, vec3_t& vel, float frametime, bool onground)
{
	vec3_t basevel(0.0f, 0.0f, 0.0f);

	move.x = (vel.x + basevel.x) * frametime;
	move.y = (vel.y + basevel.y) * frametime;

	if (onground)
	{
		move.z = (vel.z + basevel.z) * frametime;
	}
	else
	{
		float gravity = interfaces::console->get_convar("sv_gravity")->get_float() * 0.4f;
		float newZ = vel.z - (gravity * frametime);
		move.z = ((vel.z + newZ) / 2.0f + basevel.z) * frametime;

		vel.z = newZ;
	}
}

void visual::nade_pred::push_entity(vec3_t& src, const vec3_t& move, trace_t& tr)
{
	vec3_t vecAbsEnd = src;
	vecAbsEnd += move;

	// Trace through world
	trace_hull(src, vecAbsEnd, tr);
}

void visual::nade_pred::resolve_fly_collision_custom(trace_t& tr, vec3_t& velocity_vector, float interval)
{
	// Calculate elasticity
	float surface_elast = 1.0;  // Assume all surfaces have the same elasticity
	float grenade_elast = 0.45f; // GetGrenadeElasticity()
	float total_elast = grenade_elast * surface_elast;
	if (total_elast > 0.9f) total_elast = 0.9f;
	if (total_elast < 0.0f) total_elast = 0.0f;

	// Calculate bounce
	vec3_t abs_velocity;
	physics_clip_velocity(velocity_vector, tr.plane.normal, abs_velocity, 2.0f);
	abs_velocity *= total_elast;

	// Stop completely once we move too slow
	float speed_sqr = abs_velocity.length_sqr();
	static const float flMinSpeedSqr = 20.0f * 20.0f; // 30.0f * 30.0f in CSS
	if (speed_sqr < flMinSpeedSqr)
	{
		abs_velocity.zero();
	}

	// Stop if on ground
	if (tr.plane.normal.z > 0.7f)
	{
		velocity_vector = abs_velocity;
		abs_velocity.mul((1.0f - tr.fraction) * interval);
		push_entity(tr.end, abs_velocity, tr);
	}
	else
	{
		velocity_vector = abs_velocity;
	}
}

int visual::nade_pred::physics_clip_velocity(const vec3_t& in, const vec3_t& normal, vec3_t& out, float overbounce)
{
	static const float STOP_EPSILON = 0.1f;

	float    backoff;
	float    change;
	float    angle;
	int        i, blocked;

	blocked = 0;

	angle = normal[2];

	if (angle > 0)
	{
		blocked |= 1;        // floor
	}
	if (!angle)
	{
		blocked |= 2;        // step
	}

	backoff = in.dot(normal) * overbounce;

	for (i = 0; i < 3; i++)
	{
		change = normal[i] * backoff;
		out[i] = in[i] - change;
		if (out[i] > -STOP_EPSILON && out[i] < STOP_EPSILON)
		{
			out[i] = 0;
		}
	}

	return blocked;
}














































































































































































































