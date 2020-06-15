#pragma once
#include "../../dependencies/utilities/csgo.hpp"

namespace attack {
	
	namespace aimbot {
	int find_target(c_usercmd* user_cmd);
	int get_nearest_bone(player_t* entity, c_usercmd* user_cmd) ;
	void weapon_settings(weapon_t* weapon) ;
	
	static inline int aimbone;
	static inline float aim_smooth;
	static inline float aim_fov;
	static inline float rcs_x;
	static inline float rcs_y;
	static inline int hitbox_id;
	static inline vec3_t angle;
	static inline int kill_delay;

	void run(c_usercmd*);
	void event_player_death(i_game_event* event);
	void auto_pistol(c_usercmd* user_cmd);

	};

	namespace backtrack {
		struct stored_records {
			vec3_t head;
			float simulation_time;
			matrix_t matrix[128];
		};

		struct convars {
			convar* update_rate;
			convar* max_update_rate;
			convar* interp;
			convar* interp_ratio;
			convar* min_interp_ratio;
			convar* max_interp_ratio;
			convar* max_unlag;
		};

		inline std::deque<stored_records> records[65];
		inline convars cvars;

		void update();
		void run(c_usercmd*) ;
		float get_lerp_time();
		int time_to_ticks(float time);
		bool valid_tick(float simtime) ;
		static inline void init() {
			records->clear();

			cvars.update_rate = interfaces::console->get_convar("cl_updaterate");
			cvars.max_update_rate = interfaces::console->get_convar("sv_maxupdaterate");
			cvars.interp = interfaces::console->get_convar("cl_interp");
			cvars.interp_ratio = interfaces::console->get_convar("cl_interp_ratio");
			cvars.min_interp_ratio = interfaces::console->get_convar("sv_client_min_interp_ratio");
			cvars.max_interp_ratio = interfaces::console->get_convar("sv_client_max_interp_ratio");
			cvars.max_unlag = interfaces::console->get_convar("sv_maxunlag");
		}
	};

	namespace triggerbot {
		void run(c_usercmd* user_cmd);
	};
}

namespace visual {
	inline std::unordered_map<int, wchar_t> WeaponIcons = {
		{ WEAPON_KNIFE_BAYONET, 'J' },
		{ WEAPON_KNIFE_CSS, 'J' },
		{ WEAPON_KNIFE_FLIP, 'J' },
		{ WEAPON_KNIFE_GUT, 'J' },
		{ WEAPON_KNIFE_KARAMBIT, 'J' },
		{ WEAPON_KNIFE_M9_BAYONET, 'J' },
		{ WEAPON_KNIFE_TACTICAL, 'J' },
		{ WEAPON_KNIFE_FALCHION, 'J' },
		{ WEAPON_KNIFE_SURVIVAL_BOWIE, 'J' },
		{ WEAPON_KNIFE_BUTTERFLY, 'J' },
		{ WEAPON_KNIFE_PUSH, 'J' },
		{ WEAPON_KNIFE_CORD, 'J' },
		{ WEAPON_KNIFE_CANIS, 'J' },
		{ WEAPON_KNIFE_URSUS, 'J' },
		{ WEAPON_KNIFE_GYPSY_JACKKNIFE, 'J' },
		{ WEAPON_KNIFE_OUTDOOR, 'J' },
		{ WEAPON_KNIFE_STILETTO, 'J' },
		{ WEAPON_KNIFE, 'J' },
		{ WEAPON_KNIFE_T, 'J' },
		{ WEAPON_DEAGLE, 'F' },
		{ WEAPON_ELITE, 'S' },
		{ WEAPON_FIVESEVEN, 'U' },
		{ WEAPON_GLOCK, 'C' },
		{ WEAPON_HKP2000, 'Y' },
		{ WEAPON_P250, 'Y' },
		{ WEAPON_USP_SILENCER, 'Y' },
		{ WEAPON_TEC9, 'C' },
		{ WEAPON_REVOLVER, 'F' },
		{ WEAPON_MAC10, 'L' },
		{ WEAPON_UMP45, 'Q' },
		{ WEAPON_MP5SD, 'Q' },
		{ WEAPON_BIZON, 'D' },
		{ WEAPON_MP7, 'X' },
		{ WEAPON_MP9, 'D' },
		{ WEAPON_P90, 'M' },
		{ WEAPON_GALILAR, 'V' },
		{ WEAPON_FAMAS, 'T' },
		{ WEAPON_M4A1_SILENCER, 'W' },
		{ WEAPON_M4A1, 'W' },
		{ WEAPON_AUG, 'E' },
		{ WEAPON_SG556, '[' },
		{ WEAPON_AK47, 'B' },
		{ WEAPON_G3SG1, 'I' },
		{ WEAPON_SCAR20, 'I' },
		{ WEAPON_AWP, 'R' },
		{ WEAPON_SSG08, 'N' },
		{ WEAPON_XM1014, ']' },
		{ WEAPON_SAWEDOFF, 'K' },
		{ WEAPON_MAG7, 'K' },
		{ WEAPON_NOVA, 'K' },
		{ WEAPON_NEGEV, 'Z' },
		{ WEAPON_M249, 'Z' },
		{ WEAPON_TASER, 'Y' },
		{ WEAPON_FLASHBANG, 'G' },
		{ WEAPON_HEGRENADE, 'H' },
		{ WEAPON_SMOKEGRENADE, 'P' },
		{ WEAPON_MOLOTOV, 'H' },
		{ WEAPON_DECOY, 'G' }, //k
		{ WEAPON_INCGRENADE, 'H' },
		{ WEAPON_C4, '\\' },
		{ WEAPON_CZ75A, 'Y' },
	};

	struct box {
		int x, y, w, h;
		box() = default;
		box(int x, int y, int w, int h) {
			this->x = x;
			this->y = y;
			this->w = w;
			this->h = h;
		}
	};

	static inline bool get_box(entity_t* entity, box& box) {
		matrix_t& tran_frame = entity->coord_frame();

		const vec3_t min = entity->mins();
		const vec3_t max = entity->maxs();

		vec3_t screen_boxes[8];

		vec3_t points[] = {
			vec3_t(min.x, min.y, min.z),
			vec3_t(min.x, max.y, min.z),
			vec3_t(max.x, max.y, min.z),
			vec3_t(max.x, min.y, min.z),
			vec3_t(max.x, max.y, max.z),
			vec3_t(min.x, max.y, max.z),
			vec3_t(min.x, min.y, max.z),
			vec3_t(max.x, min.y, max.z)
		};

		for (int i = 0; i <= 7; i++)
			if (!math::world_to_screen(math::vector_transform(points[i], tran_frame), screen_boxes[i]))
				return false;

		vec3_t box_array[] = {
			screen_boxes[3], // fl
			screen_boxes[5], // br
			screen_boxes[0], // bl
			screen_boxes[4], // fr
			screen_boxes[2], // fr
			screen_boxes[1], // br
			screen_boxes[6], // bl
			screen_boxes[7] // fl
		};

		float left = screen_boxes[3].x, bottom = screen_boxes[3].y, right = screen_boxes[3].x, top = screen_boxes[3].y;

		for (int i = 0; i <= 7; i++) {
			if (left > box_array[i].x)
				left = box_array[i].x;

			if (bottom < box_array[i].y)
				bottom = box_array[i].y;

			if (right < box_array[i].x)
				right = box_array[i].x;

			if (top > box_array[i].y)
				top = box_array[i].y;
		}

		box.x = static_cast<int>(left);
		box.y = static_cast<int>(top);
		box.w = static_cast<int>(right) - static_cast<int>(left);
		box.h = static_cast<int>(bottom) - static_cast<int>(top);

		return true;
	}

	namespace esp_player {
		static inline std::array< float, 64 > alpha;

		void run();
		void dormancy_fade(player_t* entity, const int idx);
		void rendering(player_t* entity);
		void skeleton(player_t* entity);
	};

	namespace esp_world {
		void run();
		void dropped_weapon(player_t* entity);
		void draw_throwable(player_t* entity);
	};
	
	namespace esp_bomb {
		void run();
		void bomb_esp(player_t* entity);
		void bomb_defuse_esp(player_t* entity);
	};

	namespace esp_sound {
		struct c_sound_info {
			c_sound_info(vec3_t positions, float times, int userids) {
				this->position = positions;
				this->time = times;
				this->userid = userids;
			}

			vec3_t position;
			float time;
			int userid;
		};
		void draw();		
		void draw_circle(color colors, vec3_t position);
		void event_player_footstep(i_game_event* event);
		void event_player_hurt(i_game_event* event);
	};

	namespace nightmode {
		void run() ;
		void apply() ;
		void remove();
	};

	namespace ambient_light {
		void run();
	};

	namespace nade_pred {
		void tick(int buttons);
		void view(); 
		void paint();  

		void setup(player_t* pl, vec3_t& vecSrc, vec3_t& vecThrow, const vec3_t& angEyeAngles);
		void simulate(vec3_t& Angles, player_t* pLocal);
		int step(vec3_t& vecSrc, vec3_t& vecThrow, int tick, float interval);
		bool check_detonate(const vec3_t& vecThrow, const trace_t& tr, int tick, float interval);
		void trace_hull(vec3_t& src, vec3_t& end, trace_t& tr);
		void add_gravity_move(vec3_t& move, vec3_t& vel, float frametime, bool onground);
		void push_entity(vec3_t& src, const vec3_t& move, trace_t& tr);
		void resolve_fly_collision_custom(trace_t& tr, vec3_t& vecVelocity, float interval);
		int physics_clip_velocity(const vec3_t& in, const vec3_t& normal, vec3_t& out, float overbounce); 

		enum {
			ACT_NONE,
			ACT_LOB,
			ACT_DROP,
			ACT_THROW
		};
		static inline int act = 0;
		static inline int type = 0;
		static inline std::vector<vec3_t> path;
		static inline std::vector<std::pair<vec3_t, vec3_t>> other_collisions;
		static inline color tracer_color = color(255, 255, 0, 255);
		static inline bool firegrenade_didnt_hit = false;
	};

	namespace chams {
		enum class shader_type_t {
			VertexLitGeneric = 0,
			UnlitGeneric,
			Modulate
		};
		i_material* create_material(shader_type_t shade, bool ignorez, bool wireframe);	
		void run(i_mat_render_context* ctx, const draw_model_state_t& state, const model_render_info_t& info, matrix_t* bone_to_world);
	};

	namespace dlight {
		void dlight(player_t* entity);
	};

	namespace glow {
		void run();
	};
}

namespace game_data
{
	struct weapon_info
	{
		constexpr weapon_info(const char* model, const char* icon = nullptr) :
			model(model),
			icon(icon)
		{}
		const char* model;
		const char* icon;
	};

	struct weapon_name
	{
		constexpr weapon_name(const int definition_index, const char* name) :
			definition_index(definition_index),
			name(name)
		{}
		int definition_index = 0;
		const char* name = nullptr;
	};

	struct quality_name
	{
		constexpr quality_name(const int index, const char* name) :
			index(index),
			name(name)
		{}
		int index = 0;
		const char* name = nullptr;
	};

	const weapon_info* get_weapon_info(int defindex);
	extern const std::vector<weapon_name> knife_names;
	extern const std::vector<weapon_name> glove_names;
	extern const std::vector<weapon_name> weapon_names;
	extern const std::vector<quality_name> quality_names;
}


namespace changers {
	namespace knife_proxy {
		void initialize();
	}; 
	
	namespace parser {
		struct paint_kit {
			int id;
			std::string name;
			bool operator < (const paint_kit& other) {
				return name < other.name;
			}
		};
		void initialize();

		inline std::vector<paint_kit> parser_skins;
		inline std::vector<paint_kit> parser_gloves;
	};

	namespace skins {
		void run();
	};
};

namespace misc {
	namespace movement {
		void bunny_hop(c_usercmd* cmd);
	};

	namespace etc {
		std::vector<int> get_observervators(int playerid);
		static inline color cl_player_obs;
		void disable_post_processing();
		void rankreveal();
		void sniper_crosshair();
		void recoil_crosshair();
		void spectators();
	};

	namespace hitmarker {
		void run();
		void event(i_game_event* event);
		void draw();
	};
}
