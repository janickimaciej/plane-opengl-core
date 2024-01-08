#pragma once

#include "common/airplane_type_name.hpp"
#include "common/state.hpp"

namespace Physics
{
	struct PlayerState
	{
		Common::AirplaneTypeName airplaneTypeName;
		int hp{};
		Common::State state{};
	};
};
