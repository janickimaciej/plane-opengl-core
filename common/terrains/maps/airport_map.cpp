#include "common/terrains/maps/airport_map.hpp"

namespace Common::Terrains
{
	// TODO: correct values
	constexpr float lengthX = 20000;
	constexpr float lengthZ = 20000;
	constexpr float spacingX = 50;
	constexpr float spacingZ = 50;

	AirportMap::AirportMap() :
		Map{lengthX, lengthZ, spacingX, spacingZ}
	{ }

	float AirportMap::getMaxHeight() const
	{
		return 0;
	}
};
