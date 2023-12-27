#include "physics/models/airplane.hpp"

#include "physics/airplane_params/airplane_params.hpp"
#include "physics/airplane_params_database/airplane_params_database.hpp"
#include "physics/flight_ctrl.hpp"
#include "physics/model_dynamics/airplane_dynamics.hpp"
#include "physics/models/model.hpp"
#include "physics/player_input.hpp"

#include <glm/glm.hpp>

#include <cstddef>
#include <memory>

namespace Physics
{
	Airplane::Airplane(const Common::AirplaneTypeName& airplaneTypeName) :
		m_airplaneTypeName{airplaneTypeName},
		m_airplaneParams{airplaneParamsDatabase[static_cast<std::size_t>(airplaneTypeName)]},
		m_flightCtrl{m_airplaneParams},
		m_dynamics{m_airplaneParams, m_flightCtrl}
	{ }

	Airplane::Airplane(const Airplane& airplane) :
		m_airplaneTypeName{airplane.m_airplaneTypeName},
		m_airplaneParams{airplane.m_airplaneParams},
		m_flightCtrl{airplane.m_flightCtrl},
		m_dynamics{m_airplaneParams, m_flightCtrl}
	{ }

	Airplane::Airplane(Airplane&& airplane) noexcept :
		m_airplaneTypeName{airplane.m_airplaneTypeName},
		m_airplaneParams{airplane.m_airplaneParams},
		m_flightCtrl{airplane.m_flightCtrl},
		m_dynamics{m_airplaneParams, m_flightCtrl}
	{ }

	void Airplane::update(const Airplane& previousAirplane)
	{
		m_flightCtrl.update(previousAirplane.m_flightCtrl);
		setState(m_dynamics.computeNewState(previousAirplane.getState()));
	}

	Common::AirplaneTypeName Airplane::getAirplaneTypeName() const
	{
		return m_airplaneTypeName;
	}

	Common::AirplaneCtrl Airplane::getCtrl() const
	{
		return m_flightCtrl.getCtrl();
	}

	void Airplane::setPlayerInput(const PlayerInput& input)
	{
		m_flightCtrl.setPlayerInput(input);
	}
};
