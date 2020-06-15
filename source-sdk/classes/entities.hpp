#pragma once
#include "../../dependencies/math/math.hpp"
#include <array>
#include "collideable.hpp"
#include "client_class.hpp"
#include "../structs/animstate.hpp"
#include "../../dependencies/utilities/netvars/netvars.hpp"

#include "../../dependencies/utilities/csgo.hpp"

enum data_update_type_t
{
	DATA_UPDATE_CREATED = 0,
	DATA_UPDATE_DATATABLE_CHANGED
};

enum team_t : int
{
	team_spec = 1,
	team_t = 2,
	team_ct = 3
};

enum cs_weapon_type
{
	WEAPONTYPE_KNIFE = 0,
	WEAPONTYPE_PISTOL,
	WEAPONTYPE_SUBMACHINEGUN,
	WEAPONTYPE_RIFLE,
	WEAPONTYPE_SHOTGUN,
	WEAPONTYPE_SNIPER_RIFLE,
	WEAPONTYPE_MACHINEGUN,
	WEAPONTYPE_C4,
	WEAPONTYPE_PLACEHOLDER,
	WEAPONTYPE_GRENADE,
	WEAPONTYPE_UNKNOWN
};

enum client_frame_stage_t
{
	// (haven't run any frames yet)
	FRAME_UNDEFINED = -1,
	FRAME_START,
	// A network packet is being recieved
	FRAME_NET_UPDATE_START,
	// Data has been received and we're going to start calling PostDataUpdate
	FRAME_NET_UPDATE_POSTDATAUPDATE_START,
	// Data has been received and we've called PostDataUpdate on all data recipients
	FRAME_NET_UPDATE_POSTDATAUPDATE_END,
	// We've received all packets, we can now do interpolation, prediction, etc..
	FRAME_NET_UPDATE_END,
	// We're about to start rendering the scene
	FRAME_RENDER_START,
	// We've finished rendering the scene.
	FRAME_RENDER_END
};

enum move_type
{
	movetype_none = 0,
	movetype_isometric,
	movetype_walk,
	movetype_step,
	movetype_fly,
	movetype_flygravity,
	movetype_vphysics,
	movetype_push,
	movetype_noclip,
	movetype_ladder,
	movetype_observer,
	movetype_custom,
	movetype_last = movetype_custom,
	movetype_max_bits = 4,
	max_movetype
};

enum entity_flags
{
	fl_onground = (1 << 0),
	fl_ducking = (1 << 1),
	fl_waterjump = (1 << 2),
	fl_ontrain = (1 << 3),
	fl_inrain = (1 << 4),
	fl_frozen = (1 << 5),
	fl_atcontrols = (1 << 6),
	fl_client = (1 << 7),
	fl_fakeclient = (1 << 8),
	fl_inwater = (1 << 9),
	fl_fly = (1 << 10),
	fl_swim = (1 << 11),
	fl_conveyor = (1 << 12),
	fl_npc = (1 << 13),
	fl_godmode = (1 << 14),
	fl_notarget = (1 << 15),
	fl_aimtarget = (1 << 16),
	fl_partialground = (1 << 17),
	fl_staticprop = (1 << 18),
	fl_graphed = (1 << 19),
	fl_grenade = (1 << 20),
	fl_stepmovement = (1 << 21),
	fl_donttouch = (1 << 22),
	fl_basevelocity = (1 << 23),
	fl_worldbrush = (1 << 24),
	fl_object = (1 << 25),
	fl_killme = (1 << 26),
	fl_onfire = (1 << 27),
	fl_dissolving = (1 << 28),
	fl_transragdoll = (1 << 29),
	fl_unblockable_by_player = (1 << 30)
};

enum item_definitiones
{
	WEAPON_NONE = 0,
	WEAPON_DEAGLE,
	WEAPON_ELITE,
	WEAPON_FIVESEVEN,
	WEAPON_GLOCK,
	WEAPON_AK47 = 7,
	WEAPON_AUG,
	WEAPON_AWP,
	WEAPON_FAMAS,
	WEAPON_G3SG1,
	WEAPON_GALILAR = 13,
	WEAPON_M249,
	WEAPON_M4A1 = 16,
	WEAPON_MAC10,
	WEAPON_P90 = 19,
	WEAPON_MP5SD = 23,
	WEAPON_UMP45,
	WEAPON_XM1014,
	WEAPON_BIZON,
	WEAPON_MAG7,
	WEAPON_NEGEV,
	WEAPON_SAWEDOFF,
	WEAPON_TEC9,
	WEAPON_TASER,
	WEAPON_HKP2000,
	WEAPON_MP7,
	WEAPON_MP9,
	WEAPON_NOVA,
	WEAPON_P250,
	WEAPON_SHIELD,
	WEAPON_SCAR20,
	WEAPON_SG556,
	WEAPON_SSG08,
	WEAPON_KNIFEGG,
	WEAPON_KNIFE,
	WEAPON_FLASHBANG,
	WEAPON_HEGRENADE,
	WEAPON_SMOKEGRENADE,
	WEAPON_MOLOTOV,
	WEAPON_DECOY,
	WEAPON_INCGRENADE,
	WEAPON_C4,
	WEAPON_HEALTHSHOT = 57,
	WEAPON_KNIFE_T = 59,
	WEAPON_M4A1_SILENCER,
	WEAPON_USP_SILENCER,
	WEAPON_CZ75A = 63,
	WEAPON_REVOLVER,
	WEAPON_TAGRENADE = 68,
	WEAPON_FISTS,
	WEAPON_BREACHCHARGE,
	WEAPON_TABLET = 72,
	WEAPON_MELEE = 74,
	WEAPON_AXE,
	WEAPON_HAMMER,
	WEAPON_SPANNER = 78,
	WEAPON_KNIFE_GHOST = 80,
	WEAPON_FIREBOMB,
	WEAPON_DIVERSION,
	WEAPON_FRAG_GRENADE,
	WEAPON_SNOWBALL,
	WEAPON_BUMPMINE,
	WEAPON_KNIFE_BAYONET = 500,
	WEAPON_KNIFE_CSS = 503,
	WEAPON_KNIFE_FLIP = 505,
	WEAPON_KNIFE_GUT,
	WEAPON_KNIFE_KARAMBIT,
	WEAPON_KNIFE_M9_BAYONET,
	WEAPON_KNIFE_TACTICAL,
	WEAPON_KNIFE_FALCHION = 512,
	WEAPON_KNIFE_SURVIVAL_BOWIE = 514,
	WEAPON_KNIFE_BUTTERFLY,
	WEAPON_KNIFE_PUSH,
	WEAPON_KNIFE_CORD,
	WEAPON_KNIFE_CANIS,
	WEAPON_KNIFE_URSUS = 519,
	WEAPON_KNIFE_GYPSY_JACKKNIFE,
	WEAPON_KNIFE_OUTDOOR,
	WEAPON_KNIFE_STILETTO = 522,
	WEAPON_KNIFE_WIDOWMAKER,
	WEAPON_KNIFE_SKELETON = 525,
	GLOVE_STUDDED_BLOODHOUND = 5027,
	GLOVE_T_SIDE = 5028,
	GLOVE_CT_SIDE = 5029,
	GLOVE_SPORTY = 5030,
	GLOVE_SLICK = 5031,
	GLOVE_LEATHER_WRAP = 5032,
	GLOVE_MOTORCYCLE = 5033,
	GLOVE_SPECIALIST = 5034,
	GLOVE_HYDRA = 5035
};

enum weapon_type : std::int32_t
{
	IS_INVALID,
	IS_GRENADE,
	IS_KNIFE,
	IS_MISC,
	IS_PISTOL,
	IS_SMG,
	IS_RIFLE,
	IS_SNIPER,
	IS_HEAVY,
	IS_MAX
};

class entity_t {
public:
	void* animating()
	{
		return reinterpret_cast<void*>(std::uint32_t(this) + 0x4);
	}

	void* networkable()
	{
		return reinterpret_cast<void*>(std::uint32_t(this) + 0x8);
	}

	collideable_t* collideable()
	{
		return  utilities::call_virtual< collideable_t* (__thiscall*)(void*) >(this, 3)(this);
	}

	void release()
	{
		return utilities::call_virtual< void(__thiscall*)(void*) >(networkable(), 1)(networkable());
	}

	c_client_class* client_class()
	{
		return utilities::call_virtual< c_client_class* (__thiscall*)(void*) >(networkable(), 2)(networkable());
	}

	void notify_should_transmit(std::int32_t state)
	{
		return utilities::call_virtual< void(__thiscall*)(void*, std::int32_t) >(networkable(), 3)(networkable(), state);
	}

	void on_pre_data_changed(std::int32_t update_type)
	{
		return utilities::call_virtual< void(__thiscall*)(void*, std::int32_t) >(networkable(), 4)(networkable(), update_type);
	}

	void on_data_changed(std::int32_t update_type)
	{
		return utilities::call_virtual< void(__thiscall*)(void*, std::int32_t) >(networkable(), 5)(networkable(), update_type);
	}

	void pre_data_update(std::int32_t update_type)
	{
		return utilities::call_virtual< void(__thiscall*)(void*, std::int32_t) >(networkable(), 6)(networkable(), update_type);
	}

	void post_data_update(std::int32_t update_type)
	{
		return utilities::call_virtual< void(__thiscall*)(void*, std::int32_t) >(networkable(), 7)(networkable(), update_type);
	}

	bool dormant()
	{
		return utilities::call_virtual< bool(__thiscall*)(void*) >(networkable(), 9)(networkable());
	}

	std::int32_t index()
	{
		return utilities::call_virtual< std::int32_t(__thiscall*)(void*) >(networkable(), 10)(networkable());
	}

	void set_destroyed_on_recreate_entities()
	{
		return utilities::call_virtual< void(__thiscall*)(void*) >(networkable(), 13)(networkable());
	}

	bool is_player()
	{
		return utilities::call_virtual< bool(__thiscall*)(void*) >(this, 157)(this);
	}

	bool is_weapon()
	{
		return utilities::call_virtual< bool(__thiscall*)(void*) >(this, 165)(this);
	}

	bool setup_bones(matrix_t* out, std::int32_t max_bones, std::int32_t mask, std::float_t time)
	{
		if (!this)return false;
		return utilities::call_virtual< bool(__thiscall*)(void*, matrix_t*, std::int32_t, std::int32_t, std::float_t) >(animating(), 13)(animating(), out, max_bones, mask, time);
	}

	model_t* model()
	{
		return utilities::call_virtual<  model_t* (__thiscall*)(void*) >(animating(), 8)(animating());
	}

	void update()
	{
		return utilities::call_virtual< void(__thiscall*)(void*) >(this, 218)(this);
	}

	std::int32_t draw_model(std::int32_t flags, uint8_t alpha)
	{
		return utilities::call_virtual<  std::int32_t(__thiscall*)(void*, std::int32_t, uint8_t) >(animating(), 9)(animating(), flags, alpha);
	}

	void set_angles(vec3_t angles)
	{
		using original_fn = void(__thiscall*)(void*, const vec3_t&);
		static original_fn set_angles_fn = (original_fn)((DWORD)utilities::pattern_scan(CLIENT_DLL, sig_set_angles));
		set_angles_fn(this, angles);
	}

	void set_position(vec3_t position)
	{
		using original_fn = void(__thiscall*)(void*, const vec3_t&);
		static original_fn set_position_fn = (original_fn)((DWORD)utilities::pattern_scan(CLIENT_DLL, sig_set_position));
		set_position_fn(this, position);
	}

	void set_model_index(std::int32_t index)
	{
		return utilities::call_virtual< void(__thiscall*)(void*, std::int32_t) >(this, 75)(this, index);
	}

	vec3_t get_absolute_origin()
	{
		__asm
		{
			MOV ECX, this
			MOV EAX, DWORD PTR DS : [ECX]
			CALL DWORD PTR DS : [EAX + 0x28]
		}
	}

	matrix_t& coord_frame()
	{
		const static auto m_CollisionGroup = netvar_manager::get_net_var(fnv::hash("DT_BaseEntity"), fnv::hash("m_CollisionGroup"));
		auto m_rgflCoordinateFrame = m_CollisionGroup - 0x30;

		return *reinterpret_cast<matrix_t*>(reinterpret_cast<std::uint32_t>(this) + m_rgflCoordinateFrame);
	}

	NETVAR("DT_CSPlayer", "m_fFlags", flags, std::int32_t);
	//OFFSET(bool, dormant, 0xED);
	NETVAR("DT_BaseEntity", "m_hOwnerEntity", owner_handle, std::uint32_t);
	NETVAR("DT_CSPlayer", "m_flSimulationTime", simulation_time, std::float_t);
	NETVAR("DT_BasePlayer", "m_vecOrigin", origin, vec3_t);
	NETVAR("DT_BasePlayer", "m_vecViewOffset[0]", view_offset, vec3_t);
	NETVAR("DT_CSPlayer", "m_iTeamNum", team, std::int32_t);
	NETVAR("DT_BaseEntity", "m_bSpotted", spotted, bool);
	NETVAR("DT_CSPlayer", "m_nSurvivalTeam", survival_team, std::int32_t);
	NETVAR("DT_CSPlayer", "m_flHealthShotBoostExpirationTime", health_boost_time, std::float_t);
	NETVAR("DT_BaseEntity", "m_vecMins", mins, vec3_t)
	NETVAR("DT_BaseEntity", "m_vecMaxs", maxs, vec3_t)
};

class econ_view_item_t {
private:
	using str_32 = char[32];
public:
	NETVAR("DT_ScriptCreatedItem", "m_bInitialized", is_initialized, bool);
	NETVAR("DT_ScriptCreatedItem", "m_iEntityLevel", entity_level, std::int32_t);
	NETVAR("DT_ScriptCreatedItem", "m_iAccountID", account_id, std::int32_t);
	NETVAR("DT_ScriptCreatedItem", "m_iItemIDLow", item_id_low, std::int32_t);
	NETVAR("DT_BaseAttributableItem", "m_szCustomName", customname, str_32);
};

class base_view_model_t : public entity_t {
public:
	NETVAR("DT_BaseViewModel", "m_nModelIndex", model_index, std::int32_t);
	NETVAR("DT_BaseViewModel", "m_nViewModelIndex", view_model_index, std::int32_t);
	NETVAR("DT_BaseViewModel", "m_hWeapon", m_hweapon, std::int32_t);
	NETVAR("DT_BaseViewModel", "m_hOwner", m_howner, std::int32_t);
};

class attributable_item_t : public entity_t {
public:
	NETVAR("DT_BaseViewModel", "m_nModelIndex", model_index, std::int32_t);
	NETVAR("DT_BaseAttributableItem", "m_OriginalOwnerXuidLow", oo_xuid_low, std::int32_t);
	NETVAR("DT_BaseAttributableItem", "m_OriginalOwnerXuidHigh", oo_xuid_high, std::int32_t);
	NETVAR("DT_BaseAttributableItem", "m_nFallbackStatTrak", fb_stattrak, std::int32_t);
	NETVAR("DT_BaseAttributableItem", "m_nFallbackPaintKit", fb_paintkit, std::int32_t);
	NETVAR("DT_BaseAttributableItem", "m_nFallbackSeed", fb_seed, std::int32_t);
	NETVAR("DT_BaseAttributableItem", "m_iEntityQuality", entity_quality, std::int32_t);
	NETVAR("DT_BaseAttributableItem", "m_flFallbackWear", fb_wear, std::float_t);
	NETVAR("DT_BaseCombatWeapon", "m_hWeaponWorldModel", world_model_handle, std::uint32_t);
	NETVAR("DT_BaseAttributableItem", "m_iItemDefinitionIndex", item_definition, std::int16_t);
	NETVAR("DT_BaseAttributableItem", "m_iItemIDHigh", item_id_high, std::int32_t); //jakby crash to wez offset z hazedumpera
	NETVAR("DT_BaseAttributableItem", "m_iAccountID", acc_id, std::int32_t);
	//NETVAR( "DT_BaseAttributableItem", "m_Item",  m_Item2, econ_view_item_t);

	void set_model_index(const std::int32_t index)
	{
		return utilities::call_virtual<void(__thiscall*)(entity_t*, std::int32_t)>(this, 75)(this, index);
	}

	base_view_model_t* get_view_model()
	{
		return (base_view_model_t*)(DWORD)this;
	}

	econ_view_item_t& item()
	{
		return *(econ_view_item_t*)this;
	}
};


class weapon_t : public entity_t {
public:
	NETVAR("DT_BaseCombatWeapon", "m_flNextPrimaryAttack", next_primary_attack, std::float_t);
	NETVAR("DT_BaseCombatWeapon", "m_flNextSecondaryAttack", next_secondary_attack, std::float_t);
	NETVAR("DT_BaseCombatWeapon", "m_iClip1", clip1_count, std::int32_t);
	NETVAR("DT_BaseCombatWeapon", "m_iClip2", clip2_count, std::int32_t);
	NETVAR("DT_BaseCombatWeapon", "m_iPrimaryReserveAmmoCount", primary_reserve_ammo_acount, std::int32_t);
	NETVAR("DT_WeaponCSBase", "m_flRecoilIndex", recoil_index, std::float_t);
	NETVAR("DT_WeaponCSBaseGun", "m_zoomLevel", zoom_level, std::float_t);
	NETVAR("DT_BaseAttributableItem", "m_iItemDefinitionIndex", item_definition, std::int16_t);
	NETVAR("DT_BaseCombatWeapon", "m_iEntityQuality", entity_quality, std::int32_t);
	NETVAR("DT_CSPlayer", "m_flNextAttack", next_attack, std::float_t);
	

	std::float_t get_spread()
	{
		return utilities::call_virtual< std::float_t(__thiscall*)(void*) >(this, 452)(this);
	}

	std::float_t inaccuracy()
	{
		return utilities::call_virtual< std::float_t(__thiscall*)(void*) >(this, 482)(this);
	}

	void update_accuracy_penalty()
	{
		return utilities::call_virtual< void(__thiscall*)(void*) >(this, 483)(this);
	}

	weapon_info_t* get_weapon_data()
	{
		return interfaces::weapon_system->get_weapon_data(this->item_definition());
	}

	std::int32_t get_type()
	{
		if (!this) return IS_INVALID;
		std::int32_t id = this->item_definition();
		switch (id) {
		case WEAPON_DEAGLE:
		case WEAPON_P250:
		case WEAPON_USP_SILENCER:
		case WEAPON_HKP2000:
		case WEAPON_GLOCK:
		case WEAPON_FIVESEVEN:
		case WEAPON_TEC9:
		case WEAPON_ELITE:
		case WEAPON_REVOLVER:
		case WEAPON_CZ75A:
			return IS_PISTOL;
			break;
		case WEAPON_MP9:
		case WEAPON_MP7:
		case WEAPON_MP5SD:
		case WEAPON_UMP45:
		case WEAPON_BIZON:
		case WEAPON_P90:
		case WEAPON_MAC10:
			return IS_SMG;
			break;
		case WEAPON_KNIFE_BAYONET:
		case WEAPON_KNIFE_CSS:
		case WEAPON_KNIFE_FLIP:
		case WEAPON_KNIFE_GUT:
		case WEAPON_KNIFE_KARAMBIT:
		case WEAPON_KNIFE_M9_BAYONET:
		case WEAPON_KNIFE_TACTICAL:
		case WEAPON_KNIFE_FALCHION:
		case WEAPON_KNIFE_SURVIVAL_BOWIE:
		case WEAPON_KNIFE_BUTTERFLY:
		case WEAPON_KNIFE_PUSH:
		case WEAPON_KNIFE_CORD:
		case WEAPON_KNIFE_CANIS:
		case WEAPON_KNIFE_URSUS:
		case WEAPON_KNIFE_GYPSY_JACKKNIFE:
		case WEAPON_KNIFE_OUTDOOR:
		case WEAPON_KNIFE_STILETTO:
		case WEAPON_KNIFE:
		case WEAPON_KNIFE_T:
			return IS_KNIFE;
			break;
		case WEAPON_SAWEDOFF:
		case WEAPON_XM1014:
		case WEAPON_MAG7:
		case WEAPON_NOVA:
		case WEAPON_M249:
		case WEAPON_NEGEV:
			return IS_HEAVY;
		case WEAPON_TASER:
		case WEAPON_C4:
			return IS_MISC;
			break;
		case WEAPON_HEGRENADE:
		case WEAPON_FLASHBANG:
		case WEAPON_DECOY:
		case WEAPON_SMOKEGRENADE:
		case WEAPON_INCGRENADE:
		case WEAPON_MOLOTOV:
			return IS_GRENADE;
			break;
		case WEAPON_AK47:
		case WEAPON_M4A1:
		case WEAPON_M4A1_SILENCER:
		case WEAPON_GALILAR:
		case WEAPON_FAMAS:
		case WEAPON_AUG:
		case WEAPON_SG556:
			return IS_RIFLE;
			break;
		case WEAPON_SCAR20:
		case WEAPON_G3SG1:
		case WEAPON_SSG08:
		case WEAPON_AWP:
			return IS_SNIPER;
			break;
		default:
			return IS_KNIFE;
		}
		return IS_INVALID;
	}

	bool is_empty()
	{
		return clip1_count() <= 0;
	}

	bool is_reloading()
	{
		//is crashing
		//static auto inReload = *(std::uint32_t*)(utilities::pattern_scan( GetModuleHandleW(L"client.dll"), "C6 87 ? ? ? ? ? 8B 06 8B CE FF 90") + 2);
		//return *(bool*)((std::uint32_t)this + inReload);
		return false;
	}

	bool can_fire()
	{
		if (!this)
			return false;

		auto type = get_type();
		if (type == IS_INVALID || type == IS_MISC || type == IS_GRENADE || type == IS_KNIFE)
			return false;

		if (is_empty())
			return false;

		return (next_primary_attack() < interfaces::globals->cur_time) && (next_attack() < interfaces::globals->cur_time);
	}
};

class player_t : public entity_t {
private:
	template <typename T>
	T& read(std::uint32_t offset)
	{
		return *reinterpret_cast<T*>(reinterpret_cast<std::uint32_t>(this) + offset);
	}

	template <typename T>
	void write(std::uint32_t offset, T data)
	{
		*reinterpret_cast<T*>(reinterpret_cast<std::uint32_t>(this) + offset) = data;
	}

public:
	NETVAR("DT_BasePlayer", "m_hViewModel[0]", view_model, std::int32_t);
	NETVAR("DT_CSPlayer", "m_bHasDefuser", has_defuser, bool);
	NETVAR("DT_CSPlayer", "m_bGunGameImmunity", has_gun_game_immunity, bool);
	NETVAR("DT_CSPlayer", "m_iShotsFired", shots_fired, std::int32_t);
	NETVAR("DT_CSPlayer", "m_angEyeAngles", eye_angles, vec3_t);
	NETVAR("DT_CSPlayer", "m_ArmorValue", armor, std::int32_t);
	NETVAR("DT_CSPlayer", "m_bHasHelmet", has_helmet, bool);
	NETVAR("DT_CSPlayer", "m_bIsScoped", is_scoped, bool);
	NETVAR("DT_CSPlayer", "m_bIsDefusing", is_defusing, bool);
	NETVAR("DT_CSPlayer", "m_iAccount", money, std::int32_t);
	NETVAR("DT_CSPlayer", "m_flLowerBodyYawTarget", lower_body_yaw, std::float_t);
	NETVAR("DT_CSPlayer", "m_flNextAttack", next_attack, std::float_t);
	NETVAR("DT_CSPlayer", "m_flFlashDuration", flash_duration, std::float_t);
	NETVAR("DT_CSPlayer", "m_flFlashMaxAlpha", flash_alpha, std::float_t);
	NETVAR("DT_CSPlayer", "m_bHasNightVision", m_bHasNightVision, std::float_t);
	NETVAR("DT_CSPlayer", "m_bNightVisionOn", m_bNightVisionOn, std::float_t);
	NETVAR("DT_CSPlayer", "m_iHealth", health, std::int32_t);
	NETVAR("DT_CSPlayer", "m_lifeState", life_state, std::int32_t);
	NETVAR("DT_CSPlayer", "m_fFlags", flags, std::int32_t);
	NETVAR("DT_BasePlayer", "m_viewPunchAngle", punch_angle, vec3_t);
	NETVAR("DT_BasePlayer", "m_aimPunchAngle", aim_punch_angle, vec3_t);
	NETVAR("DT_BasePlayer", "m_vecVelocity[0]", velocity, vec3_t);
	NETVAR("DT_BasePlayer", "m_flMaxspeed", max_speed, std::float_t);
	NETVAR("DT_BaseEntity", "m_flShadowCastDistance", m_flFOVTime, std::float_t);
	NETVAR("DT_BasePlayer", "m_iObserverMode", observer_mode, std::int32_t);
	NETVAR("DT_BasePlayer", "m_hObserverTarget", observer_target, std::int32_t);
	NETVAR("DT_BasePlayer", "m_nHitboxSet", hitbox_set, std::int32_t);
	NETVAR("DT_CSPlayer", "m_flDuckAmount", duck_amount, std::float_t);
	NETVAR("DT_CSPlayer", "m_bHasHeavyArmor", has_heavy_armor, bool);
	NETVAR("DT_SmokeGrenadeProjectile", "m_nSmokeEffectTickBegin", smoke_grenade_tick_begin, std::int32_t);
	NETVAR("DT_CSPlayer", "m_nTickBase", get_tick_base, std::int32_t);

	NETVAR("DT_PlantedC4", "m_bBombTicking", c4_is_ticking, bool);
	NETVAR("DT_PlantedC4", "m_bBombDefused", c4_is_defused, bool);
	NETVAR("DT_PlantedC4", "m_hBombDefuser", c4_gets_defused, std::float_t);
	NETVAR("DT_PlantedC4", "m_flC4Blow", c4_blow_time, std::float_t);
	NETVAR("DT_PlantedC4", "m_flDefuseCountDown", c4_defuse_countdown, std::float_t);
	NETVAR("DT_PlantedC4", "m_nBombSite", c4_site, std::int32_t);

	NETVAR("DT_CSPlayerResource", "m_bombsiteCenterA", c4_pos_a, vec3_t);
	NETVAR("DT_CSPlayerResource", "m_bombsiteCenterB", c4_pos_b, vec3_t);
	NETVAR("DT_CSPlayerResource", "m_iPlayerC4", c4_index, std::int32_t);
	NETVAR("DT_CSPlayerResource", "CSPlayerResource", c4_resource, std::int32_t);

	NETVAR_PTR("DT_BasePlayer", "m_szLastPlaceName", get_callout, char);
	NETVAR_PTR("DT_CSPlayer", "m_hMyWearables", get_wearables, std::uint32_t);
	NETVAR_PTR("DT_CSPlayer", "m_hMyWeapons", get_weapons, std::uint32_t);

	NETVAR("DT_DynamicProp", "m_bShouldGlow", should_glow, bool);

	NETVAR("DT_CSPlayer", "m_hActiveWeapon", weapon_handle, std::uint32_t);

	std::float_t get_defuse_length()
	{
		std::float_t return_value = c4_defuse_countdown() - interfaces::globals->cur_time;
		return return_value < 0 ? 0.f : return_value;
	}

	bool has_c4()
	{
		static auto ret = reinterpret_cast<bool(__thiscall*)(void*)>(utilities::pattern_scan(CLIENT_DLL, sig_has_c4));
		return ret(this);
	}

	weapon_t* active_weapon()
	{
		return reinterpret_cast<weapon_t*>(interfaces::entity_list->get_client_entity_handle(weapon_handle()));
	}

	vec3_t get_eye_pos()
	{
		return origin() + view_offset();
	}

	anim_state* get_anim_state()
	{
		return *reinterpret_cast<anim_state**>(this + 0x3900);
	}

	bool can_see_player_pos(player_t* player, const vec3_t& pos)
	{
		trace_t tr;
		ray_t ray;
		trace_filter filter;
		filter.skip = this;

		auto start = get_eye_pos();
		auto dir = (pos - start).normalized();

		ray.initialize(start, pos);
		interfaces::trace_ray->trace_ray(ray, MASK_SHOT | CONTENTS_GRATE, &filter, &tr);

		return tr.entity == player || tr.fraction > 0.97f;
	}

	vec3_t get_bone_position(int bone)
	{
		matrix_t bone_matrices[128];
		if (setup_bones(bone_matrices, 128, 256, 0.0f))
			return vec3_t{ bone_matrices[bone][0][3], bone_matrices[bone][1][3], bone_matrices[bone][2][3] };
		else
			return vec3_t{ };
	}

	vec3_t get_hitbox_position(int hitbox_id)
	{
		matrix_t bone_matrix[MAXSTUDIOBONES];

		if (setup_bones(bone_matrix, MAXSTUDIOBONES, BONE_USED_BY_HITBOX, 0.0f)) {
			auto studio_model = interfaces::model_info->get_studio_model(model());

			if (studio_model) {
				auto hitbox = studio_model->hitbox_set(0)->hitbox(hitbox_id);

				if (hitbox) {
					auto min = vec3_t{}, max = vec3_t{};

					math::transform_vector(hitbox->mins, bone_matrix[hitbox->bone], min);
					math::transform_vector(hitbox->maxs, bone_matrix[hitbox->bone], max);

					return vec3_t((min.x + max.x) * 0.5f, (min.y + max.y) * 0.5f, (min.z + max.z) * 0.5f);
				}
			}
		}
		return vec3_t{};
	}

	bool is_enemy()
	{
		auto local_player = reinterpret_cast<player_t*>(interfaces::entity_list->get_client_entity(interfaces::engine->get_local_player()));
		if (!local_player)return false;

		if (!this->is_alive())return false;

		if (!team())return false;
		if (local_player->team() != this->team())return true;

		static convar* game_type = nullptr;
		if (!game_type)game_type = interfaces::console->get_convar("game_type");
		game_type->flags &= ~fcvar_cheat;

		if (game_type->get_int() == 6) {
			if (!survival_team())return false;
			if (local_player->survival_team() == -1)return true;
			if (local_player->survival_team() != this->survival_team())return true;
		}

		return false;
	}

	bool is_alive()
	{
		return life_state() == 0;
	}

	bool alive() // test if better than life-state
	{
		return health() > 0;
	}

	bool is_moving()
	{
		if (this->velocity().length() > 0.1f)
			return true;

		return false;
	}

	bool is_in_air()
	{
		if (this->flags() & fl_onground)
			return false;

		return true;
	}

	bool is_flashed()
	{
		if (this->flash_duration() > 0.0f)
			return true;

		return false;
	}

	void update_client_side_animations()
	{
		return utilities::call_virtual< void(__thiscall*)(void*) >(this, 223)(this);
	}

	vec3_t& abs_origin()
	{
		return utilities::call_virtual< vec3_t& (__thiscall*)(void*) >(this, 10)(this);
	}

	vec3_t& abs_angles()
	{
		return utilities::call_virtual< vec3_t& (__thiscall*)(void*) >(this, 11)(this);
	}

	std::int32_t move_type()
	{
		static std::int32_t type = netvar_manager::get_net_var(fnv::hash("DT_BaseEntity"), fnv::hash("m_nRenderMode")) + 1;
		return read<std::int32_t>(type);
	}

	bool hit_chance(std::float_t hitchance)
	{
		if (!active_weapon())
			return false;

		if (hitchance > 0 && active_weapon()->item_definition() != WEAPON_REVOLVER) {
			std::float_t Inaccuracy = active_weapon()->inaccuracy();

			if (Inaccuracy == 0) {
				Inaccuracy = 0.0000001;
			}

			Inaccuracy = 1 / Inaccuracy;
			return (((hitchance * 1.5f) <= Inaccuracy) ? true : false);
		}
		return true;
	}

	player_info_t get_player_info()
	{
		player_info_t info;
		interfaces::engine->get_player_info(this->index(), &info);
		return info;
	}

	std::string get_name(bool console_safe = false)
	{
		player_info_t pinfo = this->get_player_info();

		char* pl_name = pinfo.name;
		char buf[128];
		std::int32_t c = 0;

		for (std::int32_t i = 0; pl_name[i]; ++i)
		{
			if (c >= sizeof(buf) - 1)
				break;

			switch (pl_name[i])
			{
			case '"': if (console_safe) break;
			case '\\':
			case ';': if (console_safe) break;
			case '\n':
				break;
			default:
				buf[c++] = pl_name[i];
			}
		}

		buf[c] = '\0';
		return std::string(buf);
	}

	void think()
	{
		utilities::call_virtual< void(__thiscall*)(void*) >(this, 138)(this);
	}

	void pre_think()
	{
		utilities::call_virtual< void(__thiscall*)(void*) >(this, 315)(this);
	}

	void post_think()
	{
		utilities::call_virtual< void(__thiscall*)(void*) >(this, 316)(this);
	}
};

class c_key_values {
public:
	~c_key_values()
	{
		free(this);
	}

	void init(const char* key_name)
	{
		using InitKeyValues_t = void(__thiscall*)(void*, const char*);
		static InitKeyValues_t InitKeyValuesEx = nullptr;

		if (!InitKeyValuesEx)
			InitKeyValuesEx = *reinterpret_cast<InitKeyValues_t>(utilities::pattern_scan(CLIENT_DLL, sig_key_values_init));

		InitKeyValuesEx(this, key_name);
	}

	void load_from_buffer(const char* resource, const char* buf, void* file = nullptr, const char* id = nullptr, void* eval_proc = nullptr, void* unk = nullptr)
	{
		using LoadFromBuffer_t = void(__thiscall*)(void*, const char*, const char*, void*, const char*, void*, void*);
		static LoadFromBuffer_t LoadFromBufferEx = nullptr;

		if (!LoadFromBufferEx)
			LoadFromBufferEx = *reinterpret_cast<LoadFromBuffer_t>(utilities::pattern_scan(CLIENT_DLL, sig_key_values_load_from_buffer));

		LoadFromBufferEx(this, resource, buf, file, id, eval_proc, unk);
	}

	c_key_values* find_key(const char* name, bool create)
	{
		using findkey_t = c_key_values * (__thiscall*)(c_key_values*, const char*, bool);
		static auto FindKey = *reinterpret_cast<findkey_t>(utilities::pattern_scan(CLIENT_DLL, sig_key_values_find_key));
		return FindKey(this, name, create);
	}

	void set_int(const char* key_name, std::int32_t val)
	{
		auto key_int = find_key(key_name, true);
		if (key_int) {
			*reinterpret_cast<std::int32_t*>(reinterpret_cast<std::uint32_t>(key_int) + 0xC) = val;
			*reinterpret_cast<char*>(reinterpret_cast<std::uint32_t>(key_int) + 0x10) = 2;
		}
	}

	void set_string(const char* name, const char* a2)
	{
		auto key = find_key(name, true);
		using setstring_t = void(__thiscall*)(void*, const char*);
		if (key) {
			static auto SetString = *reinterpret_cast<setstring_t>(utilities::pattern_scan(CLIENT_DLL, sig_key_values_set_string));
			SetString(key, a2);
		}
	}
};