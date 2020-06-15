#pragma once
#include "../../source-sdk/structs/dlight.hpp"

class iv_effects
{
public:
	dlight_t* cl_alloc_dlight(int key) {
		using original_fn = dlight_t * (__thiscall*)(void*, int);
		return (*(original_fn**)this)[4](this, key);
	}
	dlight_t* cl_alloc_elight(int key) {
		using original_fn = dlight_t * (__thiscall*)(void*, int);
		return (*(original_fn**)this)[5](this, key);
	}
	dlight_t* get_elight_by_key(int key) {
		using original_fn = dlight_t * (__thiscall*)(void*, int);
		return (*(original_fn**)this)[8](this, key);
	}
};