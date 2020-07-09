#pragma once
#define CLIENT_DLL "client.dll"
#define ENGINE_DLL "engine.dll"

//main
#define window_initialize hooks::window =FindWindow("Valve001", NULL)
#define wndproc_initialize hooks::wndproc_original = reinterpret_cast<WNDPROC>(SetWindowLongW(window_initialize, GWL_WNDPROC, reinterpret_cast<LONG>(hooks::wndproc)))
#define wndproc_release SetWindowLongW(FindWindowW(L"Valve001", NULL), GWL_WNDPROC, reinterpret_cast<LONG>(hooks::wndproc_original))

//interfaces
#define sig_client_state "A1 ? ? ? ? 8B 80 ? ? ? ? C3"
#define sig_directx "A1 ? ? ? ? 50 8B 08 FF 51 0C"
#define sig_input "B9 ? ? ? ? F3 0F 11 04 24 FF 50 10"
#define sig_glow_manager "0F 11 05 ? ? ? ? 83 C8 01 C7 05 ? ? ? ? 00 00 00 00"
#define sig_player_move_helper "8B 0D ? ? ? ? 8B 46 08 68"
#define sig_weapon_data "8B 35 ? ? ? ? FF 10 0F B7 C0"
#define sig_render_beams "B9 ? ? ? ? A1 ? ? ? ? FF 10 A1 ? ? ? ? B9"
#define sig_file_system "8B 0D ? ? ? ? 8D 95 ? ? ? ? 6A 00 C6"

//misc
#define sig_set_angles "55 8B EC 83 E4 F8 83 EC 64 53 56 57 8B F1"
#define sig_prediction_random_seed "8B 0D ? ? ? ? BA ? ? ? ? E8 ? ? ? ? 83 C4 04"
//make gloves
#define sig_set_abs_origin_addr "55 8B EC 83 E4 F8 51 53 56 57 8B F1 E8"  

//entitiys
#define sig_has_c4  "56 8B F1 85 F6 74 31"
#define sig_is_reloading  "C6 87 ? ? ? ? ? 8B 06 8B CE FF 90"
#define sig_set_position "55 8B EC 83 E4 F8 51 53 56 57 8B F1 E8"

#define sig_key_values_init  "55 8B EC 51 33 C0 C7 45"
#define sig_key_values_load_from_buffer  "55 8B EC 83 E4 F8 83 EC 34 53 8B 5D 0C 89"
#define sig_key_values_find_key  "55 8B EC 83 EC 1C 53 8B D9 85 DB"
#define sig_key_values_set_string  "55 8B EC A1 ? ? ? ? 53 56 57 8B F9 8B 08 8B 01"

//hooks
#define sig_crc_server_check_target  "55 8B EC 81 EC ? ? ? ? 53 8B D9 89 5D F8 80"
#define sig_return_to_client_panorama  "83 F8 02 0F 85 ? ? ? ? 8B 3D ? ? ? ? FF D7"
#define sig_return_to_engine  "83 F8 02 75 6C 68 ? ? ? ? FF 15 ? ? ? ? 8B 4C 24 28 83 C4 04"


#include <windows.h>
#include <iostream>
#include <cstdint>
#include <memory>
#include <vector>
#include <thread>
#include <chrono>
#include <array>
#include <fstream>
#include <istream>
#include <unordered_map>
#include <intrin.h>
#include <filesystem>
#include <algorithm>
#include <cmath>
#include <deque>

#include <d3d9.h>
#include <d3dx9.h>
#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")
#include "../../core/menu/imgui/imgui.h"
#include "../../core/menu/imgui/dx9/imgui_impl_dx9.h"

#include "../../core/menu/config/config.hpp"

#include "../utilities/singleton.hpp"
#include "../utilities/fnv.hpp"
#include "../utilities/utilities.hpp"
#include "../../dependencies/minhook/minhook.h"
#include "../interfaces/interfaces.hpp"
#include "../../source-sdk/sdk.hpp"
#include "../../core/hooks/hooks.hpp"
#include "../../dependencies/math/math.hpp"
#include "../../dependencies/utilities/renderer/renderer.hpp"
#include "../../dependencies/utilities/console/console.hpp"
#include "../utilities/csgo.hpp"

namespace csgo {
	extern player_t* local_player;
}
