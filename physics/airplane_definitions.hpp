#pragma once

#include "common/airplane_type_name.hpp"
#include "common/airplane_centers_of_mass.hpp"
#include "common/config.hpp"
#include "physics/airplane_definition.hpp"
#include "physics/airplane_params/airplane_params.hpp"
#include "physics/airplane_params_database/jw1.hpp"
#include "physics/airplane_params_database/mustang.hpp"

#include <glm/glm.hpp>

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
			100
		},
		AirplaneDefinition
		{
			jw1Params,
			glm::vec3{0, 0, -343},
			-Common::airplaneCentersOfMass[toSizeT(Common::AirplaneTypeName::jw1)] +
				glm::vec3{0.6, 0.63, 3.14},
			glm::vec3{0, 0, -343},
			100
		}
	};
};
