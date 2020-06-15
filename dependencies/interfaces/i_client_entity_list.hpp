#pragma once
#include <cstdint>

class i_client_entity_list {
public:
	void* get_client_entity(std::int32_t index) {
		using original_fn = void* (__thiscall*)(i_client_entity_list*, std::int32_t);
		return (*(original_fn * *)this)[3](this, index);
	}
	void* get_client_entity_handle(std::uint32_t handle_index) {
		using original_fn = void* (__thiscall*)(i_client_entity_list*, std::uint32_t);
		return (*(original_fn * *)this)[4](this, handle_index);
	}
	std::int32_t number_of_entities(bool include_non_networkable) {
		using original_fn = std::int32_t(__thiscall*)(i_client_entity_list*, bool);
		return (*(original_fn**)this)[5](this, include_non_networkable);
	}
	std::int32_t get_highest_index() {
		using original_fn = std::int32_t(__thiscall*)(i_client_entity_list*);
		return (*(original_fn**)this)[6](this);
	}
	void set_max_entities(int max_entities) {
		using original_fn = void(__thiscall*)(i_client_entity_list*, int);
		return (*(original_fn**)this)[7](this, max_entities);
	}
	std::int32_t get_max_entities() {
		using original_fn = std::int32_t(__thiscall*)(i_client_entity_list*);
		return (*(original_fn**)this)[8](this);
	}
};