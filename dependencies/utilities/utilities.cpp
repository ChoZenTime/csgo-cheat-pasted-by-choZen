#include "utilities.hpp"
#include "../utilities/csgo.hpp"
#include <psapi.h>

std::uint8_t* utilities::pattern_scan(const char* module_name, const char* signature) noexcept {
    const auto module_handle = GetModuleHandleA(module_name);

    if (!module_handle)
        return nullptr;

    static auto pattern_to_byte = [](const char* pattern) {
        auto bytes = std::vector<int>{};
        auto start = const_cast<char*>(pattern);
        auto end = const_cast<char*>(pattern) + std::strlen(pattern);

        for (auto current = start; current < end; ++current) {
            if (*current == '?') {
                ++current;

                if (*current == '?')
                    ++current;

                bytes.push_back(-1);
            }
            else {
                bytes.push_back(std::strtoul(current, &current, 16));
            }
        }
        return bytes;
    };

    auto dos_header = reinterpret_cast<PIMAGE_DOS_HEADER>(module_handle);
    auto nt_headers =
        reinterpret_cast<PIMAGE_NT_HEADERS>(reinterpret_cast<std::uint8_t*>(module_handle) + dos_header->e_lfanew);

    auto size_of_image = nt_headers->OptionalHeader.SizeOfImage;
    auto pattern_bytes = pattern_to_byte(signature);
    auto scan_bytes = reinterpret_cast<std::uint8_t*>(module_handle);

    auto s = pattern_bytes.size();
    auto d = pattern_bytes.data();

    for (auto i = 0ul; i < size_of_image - s; ++i) {
        bool found = true;

        for (auto j = 0ul; j < s; ++j) {
            if (scan_bytes[i + j] != d[j] && d[j] != -1) {
                found = false;
                break;
            }
        }
        if (found)
            return &scan_bytes[i];
    }

    return nullptr;
}

std::uint8_t* utilities::pattern_scan(void* module, const char* signature) {
	static auto pattern_to_byte = [](const char* pattern) {
		auto bytes = std::vector<int>{};
		auto start = const_cast<char*>(pattern);
		auto end = const_cast<char*>(pattern) + strlen(pattern);

		for (auto current = start; current < end; ++current) {
			if (*current == '?') {
				++current;
				if (*current == '?')
					++current;
				bytes.push_back(-1);
			}
			else {
				bytes.push_back(strtoul(current, &current, 16));
			}
		}
		return bytes;
	};

	auto dos_headers = reinterpret_cast<PIMAGE_DOS_HEADER>(module);
	auto nt_headers = reinterpret_cast<PIMAGE_NT_HEADERS>((std::uint8_t*)module + dos_headers->e_lfanew);

	auto size_of_image = nt_headers->OptionalHeader.SizeOfImage;
	auto pattern_bytes = pattern_to_byte(signature);
	auto scan_bytes = reinterpret_cast<std::uint8_t*>(module);

	auto s = pattern_bytes.size();
	auto d = pattern_bytes.data();

	for (auto i = 0ul; i < size_of_image - s; ++i) {
		bool found = true;
		for (auto j = 0ul; j < s; ++j) {
			if (scan_bytes[i + j] != d[j] && d[j] != -1) {
				found = false;
				break;
			}
		}
		if (found) {
			return &scan_bytes[i];
		}
	}
	return nullptr;
}

float utilities::csgo_armor(float damage, int armor_value) {
    float armor_ratio = 0.5f;
    float armor_bonus = 0.5f;
    if (armor_value > 0) {
        float armor_new = damage * armor_ratio;
        float armor = (damage - armor_new) * armor_bonus;

        if (armor > static_cast<float>(armor_value)) {
            armor = static_cast<float>(armor_value) * (1.f / armor_bonus);
            armor_new = damage - armor;
        }

        damage = armor_new;
    }
    return damage;
}

void utilities::dump_steam_id() {
	if (!interfaces::engine->is_connected() && !interfaces::engine->is_in_game()) {
		utilities::console_warning("[dump id] ");
		interfaces::console->console_printf("you must be in game to dump player info. \n");
		return;
	}

	static constexpr auto steam_id64_constant = 0x0110000100000000ULL;

	for (int i = 1; i <= interfaces::globals->max_clients; i++) {
		auto entity = reinterpret_cast<player_t*>(interfaces::entity_list->get_client_entity(i));

		if (!entity)
			continue;

		player_info_t info;
		interfaces::engine->get_player_info(i, &info);

		auto steam_id64 = steam_id64_constant + info.friendsid;

		std::stringstream output;

		std::string player_name = info.name;
		std::transform(player_name.begin(), player_name.end(), player_name.begin(), ::tolower);

		if (!info.fakeplayer) {
			utilities::console_warning("[dump id] ");
			output << "player " << player_name.c_str() << ": " << "steamcommunity.com/profiles/" << steam_id64 << "/" << "\n";
		}
		else {
			utilities::console_warning("[dump id] ");
			output << "player " << player_name.c_str() << ": " << "bot" << "\n";
		}

		interfaces::console->console_printf("%s", output.str());

	}
}

void utilities::console_warning(const char* msg, ...) {
	if (msg == nullptr)
		return;
	typedef void(__cdecl* console_warning_fn)(const char* msg, va_list);
	static console_warning_fn fn = (console_warning_fn)GetProcAddress(GetModuleHandle("tier0.dll"), "Warning");
	char buffer[989];
	va_list list;
	va_start(list, msg);
	vsprintf(buffer, msg, list);
	va_end(list);
	fn(buffer, list);
}

const char* utilities::hitgroup_name(int hitgroup) {
	switch (hitgroup) {
	case hitgroup_head:
		return "head";
	case hitgroup_leftleg:
		return "left leg";
	case hitgroup_rightleg:
		return "right leg";
	case hitgroup_stomach:
		return "stomach";
	case hitgroup_leftarm:
		return "left arm";
	case hitgroup_rightarm:
		return "right arm";
	default:
		return "body";
	}
}

int utilities::epoch_time() {
	return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
}

template<class T>
static T* utilities::find_hud_element(const char* name) {
	static auto fn = *reinterpret_cast<DWORD**>(utilities::pattern_scan(CLIENT_DLL, "B9 ? ? ? ? E8 ? ? ? ? 8B 5D 08") + 1);

	static auto find_hud_element = reinterpret_cast<DWORD(__thiscall*)(void*, const char*)>(utilities::pattern_scan(CLIENT_DLL, "55 8B EC 53 8B 5D 08 56 57 8B F9 33 F6 39 77 28"));
	return (T*)find_hud_element(fn, name);
}


void utilities::force_update() {
    static auto fn = utilities::pattern_scan(CLIENT_DLL, "55 8B EC 51 53 56 8B 75 08 8B D9 57 6B FE 2C 89 5D FC");
    static auto full_update = utilities::pattern_scan(ENGINE_DLL, "A1 ? ? ? ? B9 ? ? ? ? 56 FF 50 14 8B 34 85");

    if (full_update)
	{
        static auto clear_hud_icon = fn;

        if (clear_hud_icon)
		{
            static auto clear_hud_weapon_icon_fn = reinterpret_cast<std::int32_t(__thiscall*)(void*, std::int32_t)>(clear_hud_icon);
            auto element = utilities::find_hud_element<std::uint32_t*>(("CCSGO_HudWeaponSelection"));

            if (element && clear_hud_weapon_icon_fn) 
			{
                auto hud_weapons = reinterpret_cast<hud_weapons_t*>(std::uint32_t(element) - 0xA0);
                if (hud_weapons == nullptr)
                    return;

                if (!*hud_weapons->get_weapon_count())
                    return;

                for (std::int32_t i = 0; i < *hud_weapons->get_weapon_count(); i++)
                    i = clear_hud_weapon_icon_fn(hud_weapons, i);

                typedef void(*cl_full_update_t) (void);
				cl_full_update_t cl_fullupdate = (cl_full_update_t)(full_update);
                
				cl_fullupdate();
            }
        }
    }
}

bool utilities::is_behind_smoke(vec3_t start_pos, vec3_t end_pos) {
	typedef bool(__cdecl* line_goes_through_smoke) (vec3_t, vec3_t);
	static line_goes_through_smoke line_goes_through_smoke_fn = 0;

	if (!line_goes_through_smoke_fn)
		line_goes_through_smoke_fn = reinterpret_cast<line_goes_through_smoke>(utilities::pattern_scan(CLIENT_DLL, "55 8B EC 83 EC 08 8B 15 ? ? ? ? 0F 57 C0"));

	if (line_goes_through_smoke_fn)
		return line_goes_through_smoke_fn(start_pos, end_pos);
	return false;
}













































































































































































































