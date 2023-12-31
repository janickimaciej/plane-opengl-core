#include "common/terrains/maps/map.hpp"

namespace Common::Terrains
{
	float Map::getLengthX() const
	{
		return m_lengthX;
	}

	float Map::getLengthZ() const
	{
		return m_lengthZ;
	}

	float Map::getSpacingX() const
	{
		return m_spacingX;
	}

	float Map::getSpacingZ() const
	{
		return m_spacingZ;
	}

	Map::Map(float lengthX, float lengthZ, float spacingX, float spacingZ) :
		m_lengthX{lengthX},
		m_lengthZ{lengthZ},
		m_spacingX{spacingX},
		m_spacingZ{spacingZ}
	{ }
};
