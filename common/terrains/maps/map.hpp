#pragma once

#include "common/terrains/composite_terrain.hpp"

#include <glm/glm.hpp>

namespace Common::Terrains
{
	class Map : public CompositeTerrain
	{
	public:
		float getLengthX() const;
		float getLengthZ() const;
		float getSpacingX() const;
		float getSpacingZ() const;
		bool isOutside(const glm::vec3& position) const;
		virtual ~Map() = default;

	protected:
		Map(float lengthX, float lengthZ, float spacingX, float spacingZ);

	private:
		float m_lengthX{};
		float m_lengthZ{};
		float m_spacingX{};
		float m_spacingZ{};
	};
};
