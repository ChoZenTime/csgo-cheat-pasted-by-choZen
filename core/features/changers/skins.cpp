#include "../features.hpp"
#define INVALID_EHANDLE_INDEX 0xFFFFFFFF
#define GET_INDEX interfaces::model_info->get_model_index

static auto get_wearable_create_fn() -> create_client_class_fn {
	auto client_class = interfaces::client->get_client_classes();
	for (client_class = interfaces::client->get_client_classes();
		client_class; client_class = client_class->next_ptr) {

		if (client_class->class_id == (int)class_ids::ceconwearable)
			return client_class->create_fn;
	}
}

static auto make_glove(int entry, int serial) -> attributable_item_t* {
	static auto create_wearable_fn = get_wearable_create_fn();
	create_wearable_fn(entry, serial);

	const auto glove = static_cast<attributable_item_t*>(interfaces::entity_list->get_client_entity(entry));
	assert(glove); {
		static auto set_abs_origin_addr = utilities::pattern_scan(CLIENT_DLL, sig_set_abs_origin_addr);
		const auto set_abs_origin_fn = reinterpret_cast<void(__thiscall*)(void*, const std::array<float, 3>&)>(set_abs_origin_addr);
		static constexpr std::array<float, 3> new_pos = { 10000.f, 10000.f, 10000.f };
		set_abs_origin_fn(glove, new_pos);
	}
	return glove;
}

bool apply_glove(attributable_item_t* glove, const char* model, int item_definition_index, int paint_kit, int model_index, int entity_quality, float fallback_wear) {
	glove->item_definition() = item_definition_index;
	glove->fb_paintkit() = paint_kit;
	glove->set_model_index(model_index);
	glove->entity_quality() = entity_quality;
	glove->fb_wear() = fallback_wear;

	player_info_t info;
	interfaces::engine->get_player_info(interfaces::engine->get_local_player(), &info);	
	glove->acc_id() = info.xuidlow;
	
	return true;
}

bool apply_skin(attributable_item_t* skin, const char* model, int item_definition_index, int paint_kit, int model_index, int entity_quality, float fb_wear) {
	skin->item_definition() = item_definition_index;
	skin->fb_paintkit() = paint_kit;
	skin->model_index() = model_index;
	skin->entity_quality() = entity_quality;
	skin->fb_wear() = fb_wear;

	auto local_player = reinterpret_cast<player_t*>(interfaces::entity_list->get_client_entity(interfaces::engine->get_local_player()));
	if (!local_player)
		return false;

	auto viewmodel = reinterpret_cast<base_view_model_t*>(interfaces::entity_list->get_client_entity_handle(local_player->view_model()));
	if (!viewmodel)
		return false;

	auto h_view_model_weapon = viewmodel->m_hweapon();
	if (!h_view_model_weapon)
		return false;

	auto view_model_weapon = reinterpret_cast<attributable_item_t*>(interfaces::entity_list->get_client_entity_handle(h_view_model_weapon));
	if (view_model_weapon != skin)
		return false;

	viewmodel->model_index() = interfaces::model_info->get_model_index(model);

	return true;
}

void changers::skins::run() {
	//model knifes
	auto model_bayonet = "models/weapons/v_knife_bayonet.mdl";
	auto model_m9 = "models/weapons/v_knife_m9_bay.mdl";
	auto model_karambit = "models/weapons/v_knife_karam.mdl";
	auto model_bowie = "models/weapons/v_knife_survival_bowie.mdl";
	auto model_butterfly = "models/weapons/v_knife_butterfly.mdl";
	auto model_falchion = "models/weapons/v_knife_falchion_advanced.mdl";
	auto model_flip = "models/weapons/v_knife_flip.mdl";
	auto model_gut = "models/weapons/v_knife_gut.mdl";
	auto model_huntsman = "models/weapons/v_knife_tactical.mdl";
	auto model_daggers = "models/weapons/v_knife_push.mdl";
	auto model_navaja = "models/weapons/v_knife_gypsy_jackknife.mdl";
	auto model_stiletto = "models/weapons/v_knife_stiletto.mdl";
	auto model_talon = "models/weapons/v_knife_widowmaker.mdl";
	auto model_ursus = "models/weapons/v_knife_ursus.mdl";
	auto model_canis = "models/weapons/v_knife_canis.mdl";
	auto model_outdoor = "models/weapons/v_knife_outdoor.mdl";
	auto model_cord = "models/weapons/v_knife_cord.mdl";
	auto model_skeleton = "models/weapons/v_knife_skeleton.mdl";
	auto model_css = "models/weapons/v_knife_css.mdl";
	//model gloves
	auto model_blood = "models/weapons/v_models/arms/glove_bloodhound/v_glove_bloodhound.mdl";
	auto model_sport = "models/weapons/v_models/arms/glove_sporty/v_glove_sporty.mdl";
	auto model_slick = "models/weapons/v_models/arms/glove_slick/v_glove_slick.mdl";
	auto model_leath = "models/weapons/v_models/arms/glove_handwrap_leathery/v_glove_handwrap_leathery.mdl";
	auto model_moto = "models/weapons/v_models/arms/glove_motorcycle/v_glove_motorcycle.mdl";
	auto model_speci = "models/weapons/v_models/arms/glove_specialist/v_glove_specialist.mdl";
	auto model_hydra = "models/weapons/v_models/arms/glove_bloodhound/v_glove_bloodhound_hydra.mdl";

	if (!interfaces::engine->is_connected() && !interfaces::engine->is_in_game())
		return;

	auto local_player = reinterpret_cast<player_t*>(interfaces::entity_list->get_client_entity(interfaces::engine->get_local_player()));
	if (!local_player)
		return;

	// gloveschanger
	if (config_system.item.glovechanger_enable) {
		//credit to namazso for nskinz

		auto const wearables = local_player->get_wearables();
		if (!wearables)
			return;

		static auto glove_handle = std::uint32_t(0);

		auto glove = reinterpret_cast<attributable_item_t*>(interfaces::entity_list->get_client_entity_handle(wearables[0]));

		if (!glove) // There is no glove
		{
			const auto our_glove = reinterpret_cast<attributable_item_t*>(interfaces::entity_list->get_client_entity_handle(glove_handle));

			if (our_glove) // Try to get our last created glove
			{
				wearables[0] = glove_handle;
				glove = our_glove;
			}
		}
		// We are dead but we have a glove, destroy it
		if (!local_player->is_alive())
		{
			if (glove) {
				glove->set_destroyed_on_recreate_entities();
				glove->release();
			}
			return;
		}
		if (!glove) // We don't have a glove, but we should
		{
			const auto entry = interfaces::entity_list->get_highest_index() + 1;
			const auto serial = rand() % 0x1000;
			glove = make_glove(entry, serial);   // He he
			wearables[0] = entry | serial << 16;
			glove_handle = wearables[0]; // Let's store it in case we somehow lose it.
		}
		if (glove)
		{
			auto g_wear = config_system.item.glove_wear;

			//apply glove skins
			switch (config_system.item.glove_model) {
			case 1:
				apply_glove(glove, model_blood, GLOVE_STUDDED_BLOODHOUND, config_system.item.paint_kit_index_glove, GET_INDEX(model_blood), 3, g_wear);
				break;
			case 2:
				apply_glove(glove, model_sport, GLOVE_SPORTY, config_system.item.paint_kit_index_glove, GET_INDEX(model_sport), 3, g_wear);
				break;
			case 3:
				apply_glove(glove, model_slick, GLOVE_SLICK, config_system.item.paint_kit_index_glove, GET_INDEX(model_slick), 3, g_wear);
				break;
			case 4:
				apply_glove(glove, model_leath, GLOVE_LEATHER_WRAP, config_system.item.paint_kit_index_glove, GET_INDEX(model_leath), 3, g_wear);
				break;
			case 5:
				apply_glove(glove, model_moto, GLOVE_MOTORCYCLE, config_system.item.paint_kit_index_glove, GET_INDEX(model_moto), 3, g_wear);
				break;
			case 6:
				apply_glove(glove, model_speci, GLOVE_SPECIALIST, config_system.item.paint_kit_index_glove, GET_INDEX(model_speci), 3, g_wear);
				break;
			case 7:
				apply_glove(glove, model_hydra, GLOVE_HYDRA, config_system.item.paint_kit_index_glove, GET_INDEX(model_hydra), 3, g_wear);
				break;
			}

			glove->item_id_high() = -1;
			glove->fb_seed() = 0;
			glove->fb_stattrak() = -1;
			glove->pre_data_update(DATA_UPDATE_CREATED);
			//glove->set_model_index(-1);

			*reinterpret_cast<int*>(std::uint32_t(glove) + 0x64) = -1;
		}
	}

	// skinchanger
	if (config_system.item.skinchanger_enable) {
		auto active_weapon = local_player->active_weapon();
		if (!active_weapon)
			return;

		auto my_weapons = local_player->get_weapons();
		for (size_t i = 0; my_weapons[i] != INVALID_EHANDLE_INDEX; i++) {
			auto weapon = reinterpret_cast<attributable_item_t*>(interfaces::entity_list->get_client_entity_handle(my_weapons[i]));
			if (!weapon)		
				continue;

			auto wear = config_system.item.knife_wear;

			//apply knife skins
			if (weapon->client_class()->class_id == class_ids::cknife) {
				switch (config_system.item.knife_model) {
				case 1:
					apply_skin(weapon, model_bayonet, WEAPON_KNIFE_BAYONET, config_system.item.paint_kit_index_knife, GET_INDEX(model_bayonet), 3, wear);
					break;
				case 2:
					apply_skin(weapon, model_m9, WEAPON_KNIFE_M9_BAYONET, config_system.item.paint_kit_index_knife, GET_INDEX(model_m9), 3, wear);
					break;
				case 3:
					apply_skin(weapon, model_karambit, WEAPON_KNIFE_KARAMBIT, config_system.item.paint_kit_index_knife, GET_INDEX(model_karambit), 3, wear);
					break;
				case 4:
					apply_skin(weapon, model_bowie, WEAPON_KNIFE_SURVIVAL_BOWIE, config_system.item.paint_kit_index_knife, GET_INDEX(model_bowie), 3, wear);
					break;
				case 5:
					apply_skin(weapon, model_butterfly, WEAPON_KNIFE_BUTTERFLY, config_system.item.paint_kit_index_knife, GET_INDEX(model_butterfly), 3, wear);
					break;
				case 6:
					apply_skin(weapon, model_falchion, WEAPON_KNIFE_FALCHION, config_system.item.paint_kit_index_knife, GET_INDEX(model_falchion), 3, wear);
					break;
				case 7:
					apply_skin(weapon, model_flip, WEAPON_KNIFE_FLIP, config_system.item.paint_kit_index_knife, GET_INDEX(model_flip), 3, wear);
					break;
				case 8:
					apply_skin(weapon, model_gut, WEAPON_KNIFE_GUT, config_system.item.paint_kit_index_knife, GET_INDEX(model_gut), 3, wear);
					break;
				case 9:
					apply_skin(weapon, model_huntsman, WEAPON_KNIFE_TACTICAL, config_system.item.paint_kit_index_knife, GET_INDEX(model_huntsman), 3, wear);
					break;
				case 10:
					apply_skin(weapon, model_daggers, WEAPON_KNIFE_PUSH, config_system.item.paint_kit_index_knife, GET_INDEX(model_daggers), 3, wear);
					break;
				case 11:
					apply_skin(weapon, model_navaja, WEAPON_KNIFE_GYPSY_JACKKNIFE, config_system.item.paint_kit_index_knife, GET_INDEX(model_navaja), 3, wear);
					break;
				case 12:
					apply_skin(weapon, model_stiletto, WEAPON_KNIFE_STILETTO, config_system.item.paint_kit_index_knife, GET_INDEX(model_stiletto), 3, wear);
					break;
				case 13:
					apply_skin(weapon, model_talon, WEAPON_KNIFE_WIDOWMAKER, config_system.item.paint_kit_index_knife, GET_INDEX(model_talon), 3, wear);
					break;
				case 14:
					apply_skin(weapon, model_ursus, WEAPON_KNIFE_URSUS, config_system.item.paint_kit_index_knife, GET_INDEX(model_ursus), 3, wear);
					break;
				case 15:
					apply_skin(weapon, model_canis, WEAPON_KNIFE_CANIS, config_system.item.paint_kit_index_knife, GET_INDEX(model_canis), 3, wear);
					break;
				case 16:
					apply_skin(weapon, model_outdoor, WEAPON_KNIFE_OUTDOOR, config_system.item.paint_kit_index_knife, GET_INDEX(model_outdoor), 3, wear);
					break;
				case 17:
					apply_skin(weapon, model_cord, WEAPON_KNIFE_CORD, config_system.item.paint_kit_index_knife, GET_INDEX(model_cord), 3, wear);
					break;
				case 18:
					apply_skin(weapon, model_skeleton, WEAPON_KNIFE_SKELETON, config_system.item.paint_kit_index_knife, GET_INDEX(model_skeleton), 3, wear);
					break;
				case 19:
					apply_skin(weapon, model_css, WEAPON_KNIFE_CSS, config_system.item.paint_kit_index_knife, GET_INDEX(model_css), 3, wear);
					break;
				}
			}

			//apply weapon skins
			switch (weapon->item_definition()) {
			case WEAPON_USP_SILENCER:
				weapon->fb_paintkit() = config_system.item.paint_kit_index_usp, weapon->fb_wear() = wear;
				break;
			case WEAPON_HKP2000:
				weapon->fb_paintkit() = config_system.item.paint_kit_index_p2000, weapon->fb_wear() = wear;
				break;
			case WEAPON_GLOCK:
				weapon->fb_paintkit() = config_system.item.paint_kit_index_glock, weapon->fb_wear() = wear;
				break;
			case WEAPON_P250:
				weapon->fb_paintkit() = config_system.item.paint_kit_index_p250, weapon->fb_wear() = wear;
				break;
			case WEAPON_FIVESEVEN:
				weapon->fb_paintkit() = config_system.item.paint_kit_index_fiveseven, weapon->fb_wear() = wear;
				break;
			case WEAPON_TEC9:
				weapon->fb_paintkit() = config_system.item.paint_kit_index_tec, weapon->fb_wear() = wear;
				break;
			case WEAPON_CZ75A:
				weapon->fb_paintkit() = config_system.item.paint_kit_index_cz, weapon->fb_wear() = wear;
				break;
			case WEAPON_ELITE:
				weapon->fb_paintkit() = config_system.item.paint_kit_index_duals, weapon->fb_wear() = wear;
				break;
			case WEAPON_DEAGLE:
				weapon->fb_paintkit() = config_system.item.paint_kit_index_deagle, weapon->fb_wear() = wear;
				break;
			case WEAPON_REVOLVER:
				weapon->fb_paintkit() = config_system.item.paint_kit_index_revolver, weapon->fb_wear() = wear;
				break;
			case WEAPON_FAMAS:
				weapon->fb_paintkit() = config_system.item.paint_kit_index_famas, weapon->fb_wear() = wear;
				break;
			case WEAPON_GALILAR:
				weapon->fb_paintkit() = config_system.item.paint_kit_index_galil, weapon->fb_wear() = wear;
				break;
			case WEAPON_M4A1:
				weapon->fb_paintkit() = config_system.item.paint_kit_index_m4a4, weapon->fb_wear() = wear;
				break;
			case WEAPON_M4A1_SILENCER:
				weapon->fb_paintkit() = config_system.item.paint_kit_index_m4a1, weapon->fb_wear() = wear;
				break;
			case WEAPON_AK47:
				weapon->fb_paintkit() = config_system.item.paint_kit_index_ak47, weapon->fb_wear() = wear;
				break;
			case WEAPON_SG556:
				weapon->fb_paintkit() = config_system.item.paint_kit_index_sg553, weapon->fb_wear() = wear;
				break;
			case WEAPON_AUG:
				weapon->fb_paintkit() = config_system.item.paint_kit_index_aug, weapon->fb_wear() = wear;
				break;
			case WEAPON_SSG08:
				weapon->fb_paintkit() = config_system.item.paint_kit_index_ssg08, weapon->fb_wear() = wear;
				break;
			case WEAPON_AWP:
				weapon->fb_paintkit() = config_system.item.paint_kit_index_awp, weapon->fb_wear() = wear;
				break;
			case WEAPON_SCAR20:
				weapon->fb_paintkit() = config_system.item.paint_kit_index_scar, weapon->fb_wear() = wear;
				break;
			case WEAPON_G3SG1:
				weapon->fb_paintkit() = config_system.item.paint_kit_index_g3sg1, weapon->fb_wear() = wear;
				break;
			case WEAPON_SAWEDOFF:
				weapon->fb_paintkit() = config_system.item.paint_kit_index_sawoff, weapon->fb_wear() = wear;
				break;
			case WEAPON_M249:
				weapon->fb_paintkit() = config_system.item.paint_kit_index_m249, weapon->fb_wear() = wear;
				break;
			case WEAPON_NEGEV:
				weapon->fb_paintkit() = config_system.item.paint_kit_index_negev, weapon->fb_wear() = wear;
				break;
			case WEAPON_MAG7:
				weapon->fb_paintkit() = config_system.item.paint_kit_index_mag7, weapon->fb_wear() = wear;
				break;
			case WEAPON_XM1014:
				weapon->fb_paintkit() = config_system.item.paint_kit_index_xm1014, weapon->fb_wear() = wear;
				break;
			case WEAPON_NOVA:
				weapon->fb_paintkit() = config_system.item.paint_kit_index_nova, weapon->fb_wear() = wear;
				break;
			case WEAPON_BIZON:
				weapon->fb_paintkit() = config_system.item.paint_kit_index_bizon, weapon->fb_wear() = wear;
				break;
			case WEAPON_MP5SD:
				weapon->fb_paintkit() = config_system.item.paint_kit_index_mp5sd, weapon->fb_wear() = wear;
				break;
			case WEAPON_MP7:
				weapon->fb_paintkit() = config_system.item.paint_kit_index_mp7, weapon->fb_wear() = wear;
				break;
			case WEAPON_MP9:
				weapon->fb_paintkit() = config_system.item.paint_kit_index_mp9, weapon->fb_wear() = wear;
				break;
			case WEAPON_MAC10:
				weapon->fb_paintkit() = config_system.item.paint_kit_index_mac10, weapon->fb_wear() = wear;
				break;
			case WEAPON_P90:
				weapon->fb_paintkit() = config_system.item.paint_kit_index_p90, weapon->fb_wear() = wear;
				break;
			case WEAPON_UMP45:
				weapon->fb_paintkit() = config_system.item.paint_kit_index_ump45, weapon->fb_wear() = wear;
				break;
			}

			weapon->oo_xuid_low() = 0;
			weapon->oo_xuid_high() = 0;
			weapon->fb_seed() = 661;
			weapon->item_id_high() = -1;
		}
	}
}










































































































































































































