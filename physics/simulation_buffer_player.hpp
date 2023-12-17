#pragma once

#include "physics/player_info.hpp"

#include <array>

namespace Physics
{
	struct SimulationBufferPlayer
	{
		std::array<bool, 2> lockState{};
		std::array<bool, 2> lockInput{};
		PlayerInfo info{};
	};
};
