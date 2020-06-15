#pragma once

namespace hooks {
	bool initialize();
	void release();
	
	inline unsigned int get_virtual(void* _class, unsigned int index) { return static_cast<unsigned int>((*reinterpret_cast<int**>(_class))[index]); }


	extern WNDPROC wndproc_original;
	extern HWND window;
	LRESULT __stdcall wndproc(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam);

	namespace create_move {
		using fn = bool(__stdcall*)(float, c_usercmd*);
		static bool __fastcall hook(void* ecx, void* edx, int input_sample_frametime, c_usercmd* cmd);
	};

	namespace do_post_screen_effects {
		using fn = int(__thiscall*)(void*, int);
		static int __stdcall hook(int value);
	};

	namespace paint_traverse {
		using fn = void(__thiscall*)(i_panel*, unsigned int, bool, bool);
		static void __stdcall hook(unsigned int panel, bool force_repaint, bool allow_force);
	}

	namespace draw_model_execute {
		using fn = void(__thiscall*)(iv_model_render*, i_mat_render_context*, const draw_model_state_t&, const model_render_info_t&, matrix_t*);
		static void __stdcall hook(i_mat_render_context* ctx, const draw_model_state_t& state, const model_render_info_t& info, matrix_t* bone_to_world);
	}

	namespace scene_end {
		using fn = void(__thiscall*)(void*);
		static void __stdcall hook();
	}

	namespace crc_server_check {
		using fn = void(__thiscall*)(void*, void*);
		void __fastcall hook(void*, void*);
	}

	namespace frame_stage_notify {
		using fn = void(__thiscall*)(i_base_client_dll*, int);
		void __stdcall hook(int frame_stage);
	}

	namespace game_events {
		using fn = bool(__thiscall*)(i_game_event_manager2*, i_game_event*);
		bool __stdcall hook(i_game_event* event);
	}
	
	namespace in_key_event {
		using fn = int(__stdcall*)(int, int, const char*);
		int __fastcall hook(void* ecx, int edx, int event_code, int key_num, const char* current_binding);
	}

	namespace lock_cursor {
		using fn = void(__thiscall*)(void*);
		void __stdcall hook();
	}

	namespace present {
		using fn = long(__stdcall*)(IDirect3DDevice9*, RECT*, RECT*, HWND, RGNDATA*);
		long __stdcall hook(IDirect3DDevice9* device, RECT* source_rect, RECT* dest_rect, HWND dest_window_override, RGNDATA* dirty_region);
	}

	namespace reset {
		using fn = long(__stdcall*)(IDirect3DDevice9*, D3DPRESENT_PARAMETERS*);
		long __stdcall hook(IDirect3DDevice9* m_device, D3DPRESENT_PARAMETERS* presentation_parameters);
	}

	namespace file_system {
		int __fastcall hook(void* ecx, void* edx);
	}
}
