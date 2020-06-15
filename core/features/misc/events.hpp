#pragma once
#include "../../../dependencies/utilities/csgo.hpp"

class c_hooked_events : public i_game_event_listener2 {
public:
	void fire_game_event(i_game_event* event);
	void initialize();
	void release();
	int get_event_debug_id(void);
};

extern c_hooked_events events;
