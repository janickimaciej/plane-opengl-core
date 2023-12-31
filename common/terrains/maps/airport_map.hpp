#pragma once

#include "common/terrains/maps/map.hpp"

namespace Common::Terrains
{
	class AirportMap : public Map
	{
	public:
		AirportMap();
		virtual ~AirportMap() = default;
	};
};
