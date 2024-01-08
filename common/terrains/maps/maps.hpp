#pragma once

#include "common/map_name.hpp"
#include "common/terrains/maps/airport_map.hpp"
#include "common/terrains/maps/hills_map.hpp"

#include <array>
#include <memory>

namespace Common::Terrains
{
	inline const std::array<std::unique_ptr<Map>, Common::mapCount> maps
	{
		std::make_unique<AirportMap>(),
		std::make_unique<HillsMap>()
	};
};
