#pragma once
#include <cstdint>
#include "../../source-sdk/math/vector3d.hpp"

namespace utilities {
	template<typename FuncType>
	__forceinline static FuncType call_virtual(void* ppClass, int index) {
		int* pVTable = *(int**)ppClass;
		int dwAddress = pVTable[index];
		return (FuncType)(dwAddress);
	}
	namespace math {
		template <typename t> t clamp_value(t value, t min, t max) {
			if (value > max) {
				return max;
			}
			if (value < min) {
				return min;
			}
			return value;
		}
	}

	struct hud_weapons_t {
		std::int32_t* get_weapon_count() {
			return reinterpret_cast<std::int32_t*>(std::uint32_t(this) + 0x80);
		}
	};

	std::uint8_t* pattern_scan(const char* module_name, const char* signature) noexcept;
	std::uint8_t* pattern_scan(void* module, const char* signature);
	float csgo_armor(float damage, int armor_value);
	void dump_steam_id();
	void console_warning(const char* msg, ...);
	const char* hitgroup_name(int hitgroup);
	int epoch_time();
	void force_update();
	bool is_behind_smoke(vec3_t start_pos, vec3_t end_pos);
	template<class T>
	static T* find_hud_element(const char* name);
}