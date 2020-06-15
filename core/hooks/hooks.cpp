#pragma once
#include "../../dependencies/utilities/csgo.hpp"
#include "../features/features.hpp"
#include "../features/misc/engine_prediction.hpp"
#include "../menu/menu.hpp"

hooks::create_move::fn create_move_original = nullptr;
hooks::paint_traverse::fn paint_traverse_original = nullptr;
hooks::game_events::fn fire_game_event_original = nullptr;
hooks::draw_model_execute::fn draw_model_execute_original = nullptr;
hooks::scene_end::fn scene_end_original = nullptr;
hooks::crc_server_check::fn crc_server_check_original = nullptr;
hooks::frame_stage_notify::fn frame_stage_notify_original = nullptr;
hooks::do_post_screen_effects::fn do_post_screen_effects_original = nullptr;
hooks::lock_cursor::fn lock_cursor_original = nullptr;
hooks::in_key_event::fn in_key_event_original = nullptr;
hooks::present::fn present_original = nullptr;
hooks::reset::fn reset_original = nullptr;

bool hooks::initialize() {
	auto create_move_target = reinterpret_cast<void*>(get_virtual(interfaces::clientmode, 24));
	auto do_post_screen_effects_target = reinterpret_cast<void*>(get_virtual(interfaces::clientmode, 44));
	auto paint_traverse_target = reinterpret_cast<void*>(get_virtual(interfaces::panel, 41));
	auto fire_game_events = reinterpret_cast<void*>(get_virtual(interfaces::event_manager, 9));
	auto draw_model_execute_target = reinterpret_cast<void*>(get_virtual(interfaces::model_render, 21));
	auto scene_end_target = reinterpret_cast<void*>(get_virtual(interfaces::render_view, 9));
	auto crc_server_check_target = reinterpret_cast<void*>(utilities::pattern_scan(ENGINE_DLL, sig_crc_server_check_target));
	auto frame_stage_notify_target = reinterpret_cast<void*>(get_virtual(interfaces::client, 37));
	auto lock_cursor_target = reinterpret_cast<void*>(get_virtual(interfaces::surface, 67));
	auto in_key_event_target = reinterpret_cast<void*>(get_virtual(interfaces::client, 21));
	auto present_target = reinterpret_cast<void*>(get_virtual(interfaces::directx, 17));
	auto reset_target = reinterpret_cast<void*>(get_virtual(interfaces::directx, 16));
	auto file_system_target = reinterpret_cast<void*>(get_virtual(interfaces::file_system, 128));
	
	
	if (MH_Initialize() != MH_OK) {
		throw std::runtime_error("failed to initialize MH_Initialize.");
		return false;
	}

	if (MH_CreateHook(create_move_target, &create_move::hook, reinterpret_cast<void**>(&create_move_original)) != MH_OK) {
		throw std::runtime_error("failed to initialize create_move. (outdated index?)");
		return false;
	}

	if (MH_CreateHook(do_post_screen_effects_target, &do_post_screen_effects::hook, reinterpret_cast<void**>(&do_post_screen_effects_original)) != MH_OK) {
		throw std::runtime_error("failed to initialize do_post_screen_effects. (outdated index?)");
		return false;
	}

	if (MH_CreateHook(paint_traverse_target, &paint_traverse::hook, reinterpret_cast<void**>(&paint_traverse_original)) != MH_OK) {
		throw std::runtime_error("failed to initialize paint_traverse. (outdated index?)");
		return false;
	}

	if (MH_CreateHook(draw_model_execute_target, &draw_model_execute::hook, reinterpret_cast<void**>(&draw_model_execute_original)) != MH_OK) {
		throw std::runtime_error("failed to initialize draw_model_execute. (outdated index?)");
		return false;
	}

	if (MH_CreateHook(scene_end_target, &scene_end::hook, reinterpret_cast<void**>(&scene_end_original)) != MH_OK) {
		throw std::runtime_error("failed to initialize scene_end. (outdated index?)");
		return false;
	}

	if (MH_CreateHook(fire_game_events, &game_events::hook, reinterpret_cast<void**>(&fire_game_event_original)) != MH_OK) {
		throw std::runtime_error("failed fire_game_events");
		return false;
	}

	if (MH_CreateHook(crc_server_check_target, &crc_server_check::hook, reinterpret_cast<void**>(&crc_server_check_original)) != MH_OK) {
		throw std::runtime_error("failed to initialize crc_server_check. (outdated index?)");
		return false;
	}

	if (MH_CreateHook(frame_stage_notify_target, &frame_stage_notify::hook, reinterpret_cast<void**>(&frame_stage_notify_original)) != MH_OK) {
		throw std::runtime_error("failed to initialize frame_stage_notify. (outdated index?)");
		return false;
	}

	if (MH_CreateHook(lock_cursor_target, &lock_cursor::hook, reinterpret_cast<void**>(&lock_cursor_original)) != MH_OK) {
		throw std::runtime_error("failed to initialize lock_cursor. (outdated index?)");
		return false;
	}
	
	if (MH_CreateHook(in_key_event_target, &in_key_event::hook, reinterpret_cast<void**>(&in_key_event_original)) != MH_OK) {
		throw std::runtime_error("failed to initialize in_key_event. (outdated index?)");
		return false;
	}

	if (MH_CreateHook(present_target, &present::hook, reinterpret_cast<void**>(&present_original)) != MH_OK) {
		throw std::runtime_error("failed to initialize present. (outdated index?)");
		return false;
	}

	if (MH_CreateHook(reset_target, &reset::hook, reinterpret_cast<void**>(&reset_original)) != MH_OK) {
		throw std::runtime_error("failed to initialize reset. (outdated index?)");
		return false;
	}

	if (MH_EnableHook(MH_ALL_HOOKS) != MH_OK) {
		throw std::runtime_error("failed to enable hooks.");
		return false;
	}
	
	console::log("[setup] hooks initialized!\n");
	return true;
}

void hooks::release() {
	MH_Uninitialize();
	MH_DisableHook(MH_ALL_HOOKS);
}

bool __fastcall hooks::create_move::hook(void* ecx, void* edx, int input_sample_frametime, c_usercmd* cmd) {
	create_move_original(input_sample_frametime, cmd);

	if (!cmd || !cmd->command_number)
		return create_move_original(input_sample_frametime, cmd);

	csgo::local_player = reinterpret_cast<player_t*>(interfaces::entity_list->get_client_entity(interfaces::engine->get_local_player()));

	misc::movement::bunny_hop(cmd);
	misc::etc::disable_post_processing();
	misc::etc::recoil_crosshair();
	misc::etc::sniper_crosshair();
	misc::etc::rankreveal();
	visual::nade_pred::tick(cmd->buttons);
	visual::nightmode::run();
	
	attack::triggerbot::run(cmd);
	
	std::uint32_t* frame_pointer;
	__asm mov frame_pointer, ebp;
	bool& send_packet = *reinterpret_cast<bool*>(*frame_pointer - 0x1C);

	prediction::start(cmd); {
		
		attack::aimbot::run(cmd);
		attack::backtrack::run(cmd);

	} prediction::end();
	
	auto old_viewangles = cmd->viewangles;
	auto old_forwardmove = cmd->forwardmove;
	auto old_sidemove = cmd->sidemove;

	math::correct_movement(old_viewangles, cmd, old_forwardmove, old_sidemove);

	cmd->forwardmove = std::clamp(cmd->forwardmove, -450.0f, 450.0f);
	cmd->sidemove = std::clamp(cmd->sidemove, -450.0f, 450.0f);
	//cmd->upmove = std::clamp(cmd->sidemove, -320.0f, 320.0f);
	cmd->upmove = std::clamp(cmd->upmove, -450.0f, 450.0f);

	cmd->viewangles.normalize();
	cmd->viewangles.x = std::clamp(cmd->viewangles.x, -89.0f, 89.0f);
	cmd->viewangles.y = std::clamp(cmd->viewangles.y, -180.0f, 180.0f);
	cmd->viewangles.z = 0.0f;

	return false;
}

int __stdcall hooks::do_post_screen_effects::hook(int value) {
	visual::glow::run();
	visual::ambient_light::run();

	return do_post_screen_effects_original(interfaces::clientmode, value);
}

void __stdcall hooks::paint_traverse::hook(unsigned int panel, bool force_repaint, bool allow_force) {
	auto panel_to_draw = fnv::hash(interfaces::panel->get_panel_name(panel));

	switch (panel_to_draw) {
	case fnv::hash("MatSystemTopPanel"):
		
		render::draw_text_string(10, 10, render::fonts::watermark_font, "csgo-cheat", false, color::white(255));

		visual::esp_player::run();
		visual::esp_bomb ::run();
		visual::esp_world::run();
		visual::nade_pred::paint();

		misc::etc::spectators();
		//event_logs.run();
	
		break;

	case fnv::hash("FocusOverlayPanel"):
		
		break;
	}

	paint_traverse_original(interfaces::panel, panel, force_repaint, allow_force);
}

void __stdcall hooks::scene_end::hook() {
	visual::nade_pred::view();

	scene_end_original(interfaces::render_view);
}

bool __stdcall hooks::game_events::hook(i_game_event* event){
	return fire_game_event_original(interfaces::event_manager, event);
}

void __stdcall hooks::draw_model_execute::hook(i_mat_render_context* ctx, const draw_model_state_t& state, const model_render_info_t& info, matrix_t* bone_to_world) {
	if (!interfaces::engine->is_connected() || interfaces::engine->is_taking_screenshot() || interfaces::model_render->is_forced_material_override())
		return draw_model_execute_original(interfaces::model_render, ctx, state, info, bone_to_world);
	
	visual::chams::run(ctx, state, info, bone_to_world);

	draw_model_execute_original(interfaces::model_render, ctx, state, info, bone_to_world);	
	interfaces::model_render->override_material(NULL);
}

void __fastcall hooks::crc_server_check::hook(void* ecx, void* edx) {
	return;
}

void __stdcall hooks::frame_stage_notify::hook(int frame_stage) {
	static auto backtrack_init = (attack::backtrack::init(), false);

	if (frame_stage == FRAME_RENDER_START) {
	}

	else if (frame_stage == FRAME_NET_UPDATE_POSTDATAUPDATE_START && interfaces::engine->is_in_game()) {
		//changers::skins::run(false);
		changers::skins::run();	
	}

	else if (frame_stage == FRAME_NET_UPDATE_POSTDATAUPDATE_END && interfaces::engine->is_in_game()) {
		//changers::skins::run(true);
	}

	else if (frame_stage == FRAME_NET_UPDATE_START && interfaces::engine->is_in_game()) {
		visual::esp_sound::draw();
	}

	else if (frame_stage == FRAME_NET_UPDATE_END && interfaces::engine->is_in_game()) {
		attack::backtrack::update();
	}

	frame_stage_notify_original(interfaces::client, frame_stage);
}

extern LRESULT ImGui_ImplDX9_WndProcHandler(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam);
LRESULT __stdcall hooks::wndproc(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam) {
	static bool pressed = false;

	if (!pressed && GetAsyncKeyState(VK_INSERT)) {
		pressed = true;
	}
	else if (pressed && !GetAsyncKeyState(VK_INSERT)) {
		pressed = false;

		menu.opened = !menu.opened;
	}

	if (menu.opened) {
		interfaces::inputsystem->enable_input(false);

	}
	else if (!menu.opened) {
		interfaces::inputsystem->enable_input(true);
	}

	if (menu.opened && ImGui_ImplDX9_WndProcHandler(hwnd, message, wparam, lparam))
		return true;

	return CallWindowProcA(wndproc_original, hwnd, message, wparam, lparam);
}

void __stdcall hooks::lock_cursor::hook() {
	if (menu.opened) {
		interfaces::surface->unlock_cursor();
		return;
	}
	return lock_cursor_original(interfaces::surface);
}

int __fastcall hooks::in_key_event::hook(void* ecx, int edx, int event_code, int key_num, const char* current_binding) {
	return in_key_event_original(event_code, key_num, current_binding);
}

static bool initialized = false;
long __stdcall hooks::present::hook(IDirect3DDevice9* device, RECT* source_rect, RECT* dest_rect, HWND dest_window_override, RGNDATA* dirty_region) {
	if (!initialized) {
		menu.apply_fonts();
		menu.setup_resent(device);
		initialized = true;
	}
	if (initialized) {
		menu.pre_render(device);
		menu.post_render();

		menu.run_popup();
		menu.run();
		menu.end_present(device);
	}

	return present_original(device, source_rect, dest_rect, dest_window_override, dirty_region);
}

long __stdcall hooks::reset::hook(IDirect3DDevice9* device, D3DPRESENT_PARAMETERS* present_parameters) {
	if (!initialized)
		reset_original(device, present_parameters);

	menu.invalidate_objects();
	long hr = reset_original(device, present_parameters);
	menu.create_objects(device);

	return hr;
}

int __fastcall hooks::file_system::hook(void* ecx, void* edx){
	static auto return_to_client_panorama = utilities::pattern_scan(CLIENT_DLL, sig_return_to_client_panorama);
	static auto return_to_engine = utilities::pattern_scan(ENGINE_DLL, sig_return_to_engine);

	if (_ReturnAddress() == return_to_client_panorama || _ReturnAddress() == return_to_engine)
		return 0; //return 0 in some cases to not being kicked from game

	return 1; //return 1 to allow loading of chams materials
}













































































































































































































