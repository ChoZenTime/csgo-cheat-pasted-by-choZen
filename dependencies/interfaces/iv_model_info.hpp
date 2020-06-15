#pragma once
#include "../../source-sdk/structs/models.hpp"
#include "../../source-sdk/classes/studio.hpp"

class iv_model_info {
public:
	model_t* get_model(int index) {
		return utilities::call_virtual< model_t* (__thiscall*)(decltype(this), int) >(this, 1)(this, index);
	}
	int get_model_index(const char* filename) {
		return utilities::call_virtual< int(__thiscall*)(decltype(this), const char*) >(this, 2)(this, filename);
	}
	const char* get_model_name(const model_t* model) {
		return utilities::call_virtual< const char* (__thiscall*)(decltype(this), const model_t*)>(this, 3)(this, model);
	}
	studio_hdr_t* get_studio_model(const model_t* model) {
		return utilities::call_virtual< studio_hdr_t* (__thiscall*)(decltype(this), const model_t*)>(this, 32)(this, model);
	}
	int get_model_materials(const model_t* model, int count, i_material** ppMaterial) {
		return utilities::call_virtual< int(__thiscall*)(decltype(this), const model_t* model, int count, i_material** ppMaterial) >(this, 19)(this, model, count, ppMaterial);
	}
};