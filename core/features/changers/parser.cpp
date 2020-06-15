#include "../features.hpp"

//std::vector<changers::parser::paint_kit> changers::parser::parser_skins;
//std::vector<changers::parser::paint_kit> changers::parser::parser_gloves;

class CCStrike15ItemSchema;
class CCStrike15ItemSystem;

template <typename Key, typename Value>
struct node_t {
	int previous_id;
	int next_id;
	void* _unknown_ptr;
	int _unknown;
	Key key;
	Value value;
};
template <typename key, typename value>
struct head_t {
	node_t<key, value>* memory;
	int allocation_count;
	int grow_size;
	int start_element;
	int next_available;
	int _unknown;
	int last_element;
};

struct string_t {
	char* buffer;
	int capacity;
	int grow_size;
	int length;
};

struct paint_kit_t {
	int id;
	string_t name;
	string_t description;
	string_t item_name;
	string_t material_name;
	string_t image_inventory;
	char pad_0x0054[0x8C];
};

struct CStickerKit
{
	int id;

	int item_rarity;

	string_t name;
	string_t description;
	string_t item_name;
	string_t material_name;
	string_t image_inventory;

	int tournament_event_id;
	int tournament_team_id;
	int tournament_player_id;
	bool is_custom_sticker_material;

	float rotate_end;
	float rotate_start;

	float scale_min;
	float scale_max;

	float wear_min;
	float wear_max;

	string_t image_inventory2;
	string_t image_inventory_large;

	std::uint32_t pad0[4];
};

void* get_export(const char* module_name, const char* export_name) {
	HMODULE mod;
	while (!((mod = GetModuleHandleA(module_name))))
		Sleep(100);
	return reinterpret_cast<void*>(GetProcAddress(mod, export_name));
}


void changers::parser::initialize() {
	std::ifstream items{ "csgo/scripts/items/items_game_cdn.txt" };
	const std::string gameItems{ std::istreambuf_iterator<char>{ items }, std::istreambuf_iterator<char>{ } };
	items.close();
	const auto V_UCS2ToUTF8 = static_cast<int(*)(const wchar_t* ucs2, char* utf8, int len)>(get_export("vstdlib.dll", "V_UCS2ToUTF8"));
	const auto sig_address = utilities::pattern_scan(CLIENT_DLL, "E8 ? ? ? ? FF 76 0C 8D 48 04 E8");
	const auto item_system_offset = *reinterpret_cast<std::int32_t*>(sig_address + 1);
	const auto item_system_fn = reinterpret_cast<CCStrike15ItemSystem * (*)()>(sig_address + 5 + item_system_offset);
	const auto item_schema = reinterpret_cast<CCStrike15ItemSchema*>(std::uint32_t(item_system_fn()) + sizeof(void*));
	{
		const auto get_paint_kit_definition_offset = *reinterpret_cast<std::int32_t*>(sig_address + 11 + 1);
		const auto get_paint_kit_definition_fn = reinterpret_cast<paint_kit_t * (__thiscall*)(CCStrike15ItemSchema*, int)>(sig_address + 11 + 5 + get_paint_kit_definition_offset);
		const auto start_element_offset = *reinterpret_cast<std::int32_t*>(std::uint32_t(get_paint_kit_definition_fn) + 8 + 2);
		const auto head_offset = start_element_offset - 12;
		const auto map_head = reinterpret_cast<head_t<int, paint_kit_t*>*>(std::uint32_t(item_schema) + head_offset);
		for (auto i = 0; i <= map_head->last_element; ++i) {
			const auto paint_kit = map_head->memory[i].value;
			if (paint_kit->id == 9001)
				continue;
			const auto wide_name = interfaces::localize->find(paint_kit->item_name.buffer + 1);
			char name[256];

			V_UCS2ToUTF8(wide_name, name, sizeof(name));
			/*if (paint_kit->id < 10000)
				parser_skins.push_back({ paint_kit->id, name });
			else
				parser_gloves.push_back({ paint_kit->id, name });*/

				//parser_skins.push_back({ paint_kit->id, name });
			const int itemNameLength = WideCharToMultiByte(CP_UTF8, 0, wide_name, -1, nullptr, 0, nullptr, nullptr);
			if (std::string name(itemNameLength, 0); WideCharToMultiByte(CP_UTF8, 0, wide_name, -1, &name[0], itemNameLength, nullptr, nullptr)) {
				if (paint_kit->id < 10000) {

					if (auto pos = gameItems.find('_' + std::string{ paint_kit->name.buffer } +'='); pos != std::string::npos && gameItems.substr(pos + paint_kit->name.length).find('_' + std::string{ paint_kit->name.buffer } +'=') == std::string::npos) {
						if (auto weaponName = gameItems.rfind("weapon_", pos); weaponName != std::string::npos) {
							name.back() = ' ';
							name += '(' + gameItems.substr(weaponName + 7, pos - weaponName - 7) + ')';
						}
					}
					parser_skins.push_back({ paint_kit->id, name });
				}

				else
					parser_gloves.push_back({ paint_kit->id, name });
			}

			std::sort(parser_skins.begin(), parser_skins.end());
			std::sort(parser_gloves.begin(), parser_gloves.end());

			/*
			std::sort(k_skins.begin(), k_skins.end());

			for (auto& skin : k_skins)
			{
				k_pure_skin_names.push_back(skin.name);
				k_skin_name_to_kit.push_back(skin.id);
			}

			for (auto& item : info)
			{
				k_pure_weapon_names.emplace_back(item.name);
				k_idx_to_defidex.push_back(item.definition_index);
			}
			*/
		}

	}
	printf("[setup] kit parser initialized!\n");
}


























































































































































































































































































































































































































