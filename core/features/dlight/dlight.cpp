#include "../features.hpp"

void visual::dlight::dlight( player_t* entity){
	if (!config_system.item.d_light)
		return;

	if (interfaces::engine->is_in_game() && interfaces::engine->is_connected()){
		vec3_t getorig = entity->origin();
		vec3_t  getheadorig = entity->get_eye_pos();
		
		if (entity->is_enemy()&& !entity->dormant()) {

			dlight_t* elight = interfaces::effects->cl_alloc_elight(entity->index());
			elight->color.r = float(config_system.item.clr_d_light[0] * 255.f);
			elight->color.g = float(config_system.item.clr_d_light[1] * 255.f);;
			elight->color.b = float(config_system.item.clr_d_light[2] * 255.f);;
			elight->color.exponent = 8.f;
			elight->direction = getheadorig;
			elight->origin = getheadorig;
			elight->radius = 200.0f;		//200.0f
			elight->die_time = interfaces::globals->cur_time + 0.1f;
			elight->decay = 50.0f;			//25.0f
			elight->key = entity->index();

			dlight_t* dlight = interfaces::effects->cl_alloc_dlight(entity->index());
			dlight->color.r = float(config_system.item.clr_d_light[0] * 255.f);
			dlight->color.g = float(config_system.item.clr_d_light[1] * 255.f);;
			dlight->color.b = float(config_system.item.clr_d_light[2] * 255.f);;
			dlight->color.exponent = 8.f;
			dlight->direction = getorig;
			dlight->origin = getorig;
			dlight->radius = 100.f;		//100.0f
			dlight->die_time = interfaces::globals->cur_time + 0.1f;
			dlight->decay = dlight->radius / 2.f;		//5.f
			dlight->key = entity->index();
		}
	}
}















































































































































































































