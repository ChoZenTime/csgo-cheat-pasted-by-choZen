#include "../dependencies/utilities/csgo.hpp"
#include "features/features.hpp"
#include "menu/config/config.hpp"
#include "../core/features/misc/events.hpp"

HWND hooks::window = NULL;
WNDPROC hooks::wndproc_original = NULL;

unsigned long WINAPI initialize(void* instance) {
	while (!GetModuleHandleA("serverbrowser.dll"))
		Sleep(200);

#ifdef _DEBUG
	console::initialize("csgo-cheat console");
#endif

	try {	
		interfaces::initialize();
		render::initialize();
		window_initialize;
		wndproc_initialize;
		hooks::initialize();
		/*custom*/
		events.initialize();
		changers::parser::initialize();
		changers::knife_proxy::initialize();
		config_system.initialize("aristois");
	}

	catch (const std::runtime_error & error) {
		MessageBoxA(NULL, error.what(), "csgo-cheat error!", MB_OK | MB_ICONERROR);
		FreeLibraryAndExitThread(static_cast<HMODULE>(instance), 0);
	}

	while (!GetAsyncKeyState(VK_END))
		std::this_thread::sleep_for(std::chrono::milliseconds(50));

	FreeLibraryAndExitThread(static_cast<HMODULE>(instance), 0);
}

unsigned long WINAPI release() {
	wndproc_release;
	events.release();
	hooks::release();

#ifdef _DEBUG
	console::release();
#endif

	return TRUE;
}

std::int32_t WINAPI DllMain(const HMODULE instance [[maybe_unused]], const unsigned long reason, const void* reserved [[maybe_unused]] ) {
	DisableThreadLibraryCalls(instance);

	switch (reason) {
	case DLL_PROCESS_ATTACH: {
		if (auto handle = CreateThread(nullptr, NULL, initialize, instance, NULL, nullptr))
			CloseHandle(handle);

		break;
	}

	case DLL_PROCESS_DETACH: {
		release();
		break;
	}
	}

	return true;
}
















































































































































































































