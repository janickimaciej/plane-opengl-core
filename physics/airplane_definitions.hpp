#pragma once

#include "common/airplane_type_name.hpp"
#include "common/airplane_centers_of_mass.hpp"
#include "common/config.hpp"
#include "physics/airplane_definition.hpp"
#include "physics/airplane_params/airplane_params.hpp"
#include "physics/airplane_params_database/jw1.hpp"
#include "physics/airplane_params_database/mustang.hpp"
#include "physics/collisions/airplane.hpp"

#include <glm/glm.hpp>

#include <array>

namespace Physics
{
	inline const std::array<AirplaneDefinition, Common::airplaneTypeCount> airplaneDefinitions
	{
		AirplaneDefinition
		{
			mustangParams,
			glm::vec3{0, 0, -100},
			-Common::airplaneCentersOfMass[toSizeT(Common::AirplaneTypeName::mustang)],
			glm::vec3{0, 0, -200},
			Collisions::Airplane
			{
				Collisions::Ellipsoid
				{
					glm::vec3{0, -0.18, 4.91} -
						Common::airplaneCentersOfMass[toSizeT(Common::AirplaneTypeName::mustang)],
					5.48f,
					0.96f,
					4.9f
				},
				std::array<glm::vec3, 6>
				{
					-Common::airplaneCentersOfMass[toSizeT(Common::AirplaneTypeName::mustang)],
					glm::vec3{0, 0.08, 9.84} -
						Common::airplaneCentersOfMass[toSizeT(Common::AirplaneTypeName::mustang)],
					glm::vec3{5.49, -0.15, 3.64} -
						Common::airplaneCentersOfMass[toSizeT(Common::AirplaneTypeName::mustang)],
					glm::vec3{-5.49, -0.15, 3.64} -
						Common::airplaneCentersOfMass[toSizeT(Common::AirplaneTypeName::mustang)],
					glm::vec3{0, -2.1, 2.78} -
						Common::airplaneCentersOfMass[toSizeT(Common::AirplaneTypeName::mustang)],
					glm::vec3{0, 1.61, 9.07} -
						Common::airplaneCentersOfMass[toSizeT(Common::AirplaneTypeName::mustang)]
				}
			},
			100
		},
		AirplaneDefinition
		{
			jw1Params,
			glm::vec3{0, 0, -343},
			-Common::airplaneCentersOfMass[toSizeT(Common::AirplaneTypeName::jw1)] +
				glm::vec3{0.6, 0.63, 3.14},
			glm::vec3{0, 0, -343},
			Collisions::Airplane
			{
				Collisions::Ellipsoid
				{
					glm::vec3{0, 0.39, 10.07} -
						Common::airplaneCentersOfMass[toSizeT(Common::AirplaneTypeName::jw1)],
					6.63f,
					1.58f,
					10.02f
				},
				std::array<glm::vec3, 6>
				{
					-Common::airplaneCentersOfMass[toSizeT(Common::AirplaneTypeName::jw1)],
					glm::vec3{0, 0, 19.98} -
						Common::airplaneCentersOfMass[toSizeT(Common::AirplaneTypeName::jw1)],
					glm::vec3{6.5, 0.45, 14.2} -
						Common::airplaneCentersOfMass[toSizeT(Common::AirplaneTypeName::jw1)],
					glm::vec3{-6.5, 0.45, 14.2} -
						Common::airplaneCentersOfMass[toSizeT(Common::AirplaneTypeName::jw1)],
					glm::vec3{0, -0.69, 17.04} -
						Common::airplaneCentersOfMass[toSizeT(Common::AirplaneTypeName::jw1)],
					glm::vec3{0, 2.58, 19.39} -
						Common::airplaneCentersOfMass[toSizeT(Common::AirplaneTypeName::jw1)]
				}
			},
			100
		}
	};
};
