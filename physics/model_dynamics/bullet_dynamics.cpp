#include "physics/model_dynamics/bullet_dynamics.hpp"

#include "common/state.hpp"
#include "physics/simulation_clock.hpp"

namespace Physics
{
	Common::State BulletDynamics::computeNewState(const Common::State& oldState) const
	{
		Common::State newState{};
		
		static constexpr float g = 9.81f;
		glm::vec3 oldVelocityGlobal = oldState.orientation * oldState.velocity;
		newState.position = oldState.position + oldVelocityGlobal * physicsTimeStep +
			glm::vec3{0, -g, 0} * physicsTimeStep * physicsTimeStep / 2.0f;
		glm::vec3 newVelocityGlobal = oldVelocityGlobal + glm::vec3{0, -g, 0} * physicsTimeStep;
		newState.setOrientation(-glm::normalize(newVelocityGlobal));
		newState.velocity = glm::inverse(newState.orientation) * newVelocityGlobal;

		return newState;
	}
};
