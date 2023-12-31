#pragma once

#include "common/terrains/maps/map.hpp"

namespace Common::Terrains
{
	class HillsMap : public Map
	{
	public:
		HillsMap();
		virtual ~HillsMap() = default;
	};
};
