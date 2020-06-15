#include "../features.hpp"

typedef void(*recv_var_proxy_fn)(const c_recv_proxy_data* p_data, void* p_struct, void* p_out);
recv_var_proxy_fn original_fn = nullptr;

enum sequence
{
	SEQUENCE_DEFAULT_DRAW = 0,
	SEQUENCE_DEFAULT_IDLE1 = 1,
	SEQUENCE_DEFAULT_IDLE2 = 2,
	SEQUENCE_DEFAULT_LIGHT_MISS1 = 3,
	SEQUENCE_DEFAULT_LIGHT_MISS2 = 4,
	SEQUENCE_DEFAULT_HEAVY_MISS1 = 9,
	SEQUENCE_DEFAULT_HEAVY_HIT1 = 10,
	SEQUENCE_DEFAULT_HEAVY_BACKSTAB = 11,
	SEQUENCE_DEFAULT_LOOKAT01 = 12,

	SEQUENCE_BUTTERFLY_DRAW = 0,
	SEQUENCE_BUTTERFLY_DRAW2 = 1,
	SEQUENCE_BUTTERFLY_LOOKAT01 = 13,
	SEQUENCE_BUTTERFLY_LOOKAT03 = 15,

	SEQUENCE_FALCHION_IDLE1 = 1,
	SEQUENCE_FALCHION_HEAVY_MISS1 = 8,
	SEQUENCE_FALCHION_HEAVY_MISS1_NOFLIP = 9,
	SEQUENCE_FALCHION_LOOKAT01 = 12,
	SEQUENCE_FALCHION_LOOKAT02 = 13,

	SEQUENCE_DAGGERS_IDLE1 = 1,
	SEQUENCE_DAGGERS_LIGHT_MISS1 = 2,
	SEQUENCE_DAGGERS_LIGHT_MISS5 = 6,
	SEQUENCE_DAGGERS_HEAVY_MISS2 = 11,
	SEQUENCE_DAGGERS_HEAVY_MISS1 = 12,

	SEQUENCE_BOWIE_IDLE1 = 1,
};

inline int random_sequence(int low, int high)
{
	return rand() % (high - low + 1) + low;
}

const static std::unordered_map<std::string, int(*)(int)> animation_fix_map
{
	{ "models/weapons/v_knife_butterfly.mdl", [](int sequence) -> int
	{
		switch (sequence)
		{
		case SEQUENCE_DEFAULT_DRAW:
			return random_sequence(SEQUENCE_BUTTERFLY_DRAW, SEQUENCE_BUTTERFLY_DRAW2);
		case SEQUENCE_DEFAULT_LOOKAT01:
			return random_sequence(SEQUENCE_BUTTERFLY_LOOKAT01, SEQUENCE_BUTTERFLY_LOOKAT03);
		default:
			return sequence + 1;
		}
	} },
	{ "models/weapons/v_knife_falchion_advanced.mdl", [](int sequence) -> int
	{
		switch (sequence)
		{
		case SEQUENCE_DEFAULT_IDLE2:
			return SEQUENCE_FALCHION_IDLE1;
		case SEQUENCE_DEFAULT_HEAVY_MISS1:
			return random_sequence(SEQUENCE_FALCHION_HEAVY_MISS1, SEQUENCE_FALCHION_HEAVY_MISS1_NOFLIP);
		case SEQUENCE_DEFAULT_LOOKAT01:
			return random_sequence(SEQUENCE_FALCHION_LOOKAT01, SEQUENCE_FALCHION_LOOKAT02);
		case SEQUENCE_DEFAULT_DRAW:
		case SEQUENCE_DEFAULT_IDLE1:
			return sequence;
		default:
			return sequence - 1;
		}
	} },
	{ "models/weapons/v_knife_push.mdl", [](int sequence) -> int
	{
		switch (sequence)
		{
		case SEQUENCE_DEFAULT_IDLE2:
			return SEQUENCE_DAGGERS_IDLE1;
		case SEQUENCE_DEFAULT_LIGHT_MISS1:
		case SEQUENCE_DEFAULT_LIGHT_MISS2:
			return random_sequence(SEQUENCE_DAGGERS_LIGHT_MISS1, SEQUENCE_DAGGERS_LIGHT_MISS5);
		case SEQUENCE_DEFAULT_HEAVY_MISS1:
			return random_sequence(SEQUENCE_DAGGERS_HEAVY_MISS2, SEQUENCE_DAGGERS_HEAVY_MISS1);
		case SEQUENCE_DEFAULT_HEAVY_HIT1:
		case SEQUENCE_DEFAULT_HEAVY_BACKSTAB:
		case SEQUENCE_DEFAULT_LOOKAT01:
			return sequence + 3;
		case SEQUENCE_DEFAULT_DRAW:
		case SEQUENCE_DEFAULT_IDLE1:
			return sequence;
		default:
			return sequence + 2;
		}
	} },
	{ "models/weapons/v_knife_survival_bowie.mdl", [](int sequence) -> int
	{
		switch (sequence)
		{
		case SEQUENCE_DEFAULT_DRAW:
		case SEQUENCE_DEFAULT_IDLE1:
			return sequence;
		case SEQUENCE_DEFAULT_IDLE2:
			return SEQUENCE_BOWIE_IDLE1;
		default:
			return sequence - 1;
		}
	} },
	{ "models/weapons/v_knife_ursus.mdl", [](int sequence) -> int
	{
		switch (sequence)
		{
		case SEQUENCE_DEFAULT_DRAW:
			return random_sequence(SEQUENCE_BUTTERFLY_DRAW, SEQUENCE_BUTTERFLY_DRAW2);
		case SEQUENCE_DEFAULT_LOOKAT01:
			return random_sequence(SEQUENCE_BUTTERFLY_LOOKAT01, 14);
		default:
			return sequence + 1;
		}
	} },
	{ "models/weapons/v_knife_stiletto.mdl", [](int sequence) -> int
	{
		switch (sequence)
		{
		case SEQUENCE_DEFAULT_LOOKAT01:
			return random_sequence(12, 13);
		}
	} },
	{ "models/weapons/v_knife_widowmaker.mdl", [](int sequence) -> int
	{
		switch (sequence)
		{
		case SEQUENCE_DEFAULT_LOOKAT01:
			return random_sequence(14, 15);
		}
	} },
	{ "models/weapons/v_knife_cord.mdl", [](int sequence) -> int
	{
		switch (sequence)
		{
		case SEQUENCE_DEFAULT_DRAW:
			return random_sequence(SEQUENCE_BUTTERFLY_DRAW, SEQUENCE_BUTTERFLY_DRAW2);
		case SEQUENCE_DEFAULT_LOOKAT01:
			return random_sequence(SEQUENCE_BUTTERFLY_LOOKAT01, 14);
		default:
			return sequence + 1;
		}
	} },
	{ "models/weapons/v_knife_canis.mdl", [](int sequence) -> int
	{
	switch (sequence)
	{
	case SEQUENCE_DEFAULT_DRAW:
		return random_sequence(SEQUENCE_BUTTERFLY_DRAW, SEQUENCE_BUTTERFLY_DRAW2);
	case SEQUENCE_DEFAULT_LOOKAT01:
		return random_sequence(SEQUENCE_BUTTERFLY_LOOKAT01, 14);
	default:
		return sequence + 1;
	}
	} },
	{ "models/weapons/v_knife_outdoor.mdl", [](int sequence) -> int
	{
	switch (sequence)
	{
	case SEQUENCE_DEFAULT_DRAW:
		return random_sequence(SEQUENCE_BUTTERFLY_DRAW, SEQUENCE_BUTTERFLY_DRAW2);
	case SEQUENCE_DEFAULT_LOOKAT01:
		return random_sequence(SEQUENCE_BUTTERFLY_LOOKAT01, 14);
	default:
		return sequence + 1;
	}
	} },
	{ "models/weapons/v_knife_skeleton.mdl", [](int sequence) -> int
	{
	switch (sequence)
	{
	case SEQUENCE_DEFAULT_DRAW:
		return random_sequence(SEQUENCE_BUTTERFLY_DRAW, SEQUENCE_BUTTERFLY_DRAW2);
	case SEQUENCE_DEFAULT_LOOKAT01:
		return random_sequence(SEQUENCE_BUTTERFLY_LOOKAT01, 14);
	default:
		return sequence + 1;
	}
	} }
		// end
};

void sequence_proxy(const c_recv_proxy_data* p_data, void* p_struct, void* p_out)
{
	const auto local = static_cast<player_t*>(interfaces::entity_list->get_client_entity(interfaces::engine->get_local_player()));
	
	if (!local || !local->is_alive())
		return original_fn(p_data, p_struct, p_out);
	
	const auto proxy_data = const_cast<c_recv_proxy_data*>(p_data);
	const auto view_model = static_cast<base_view_model_t*>(p_struct);
	
	if (view_model && view_model->m_howner() && view_model->m_howner() != NULL)
	{
		const auto owner = static_cast<player_t*>(interfaces::entity_list->get_client_entity_handle(view_model->m_howner()));

		if (owner == local)
		{
			// Get the filename of the current view model.
			auto knife_model = interfaces::model_info->get_model(view_model->model_index());
			auto model_name = interfaces::model_info->get_model_name(knife_model);

			if (animation_fix_map.count(model_name))
				proxy_data->value.m_int = animation_fix_map.at(model_name)(proxy_data->value.m_int);
		}
	}
	
	original_fn(proxy_data, p_struct, p_out);
}

void knife_sequence() 
{
	for (auto client_class = interfaces::client->get_client_classes(); client_class; client_class = client_class->next_ptr) {

		if (!strcmp(client_class->network_name, "CBaseViewModel")) {
			const auto table = reinterpret_cast<recv_table*>(client_class->recvtable_ptr);
			for (int nIndex = 0; nIndex < table->props_count; nIndex++) {
				recv_prop* pProp = &table->props[nIndex];
				if (!pProp || strcmp(pProp->prop_name, "m_nSequence"))
					continue;
				original_fn = static_cast<recv_var_proxy_fn>(pProp->proxy_fn);
				pProp->proxy_fn = static_cast<recv_var_proxy_fn>(sequence_proxy);
				break;
			}
			break;
		}
	}
}

void changers::knife_proxy::initialize() 
{
	knife_sequence();

	auto client_class = interfaces::client->get_client_classes();
	while (client_class)
		client_class = client_class->next_ptr;
}
















































































































































































































