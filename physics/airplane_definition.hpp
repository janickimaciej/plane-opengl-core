#pragma once

#include "physics/airplane_params/airplane_params.hpp"

#include <glm/glm.hpp>

namespace Physics
{
	struct AirplaneDefinition
	{
		AirplaneParams params;
		glm::vec3 initialVelocity{};
		glm::vec3 muzzlePosition{};
		glm::vec3 muzzleVelocity{};
		int initialHP{};
	};
};
