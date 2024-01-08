#pragma once

#include "physics/airplane_params/airplane_params.hpp"
#include "physics/collisions/airplane.hpp"

#include <glm/glm.hpp>

namespace Physics
{
	struct AirplaneDefinition
	{
		AirplaneParams params;
		glm::vec3 initialVelocity{};
		glm::vec3 muzzlePosition{};
		glm::vec3 muzzleVelocity{};
		Collisions::Airplane collisionModel{};
		int initialHP{};
	};
};
