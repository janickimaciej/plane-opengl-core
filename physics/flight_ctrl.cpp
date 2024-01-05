#include "flight_ctrl.hpp"

#include "common/config.hpp"
#include "physics/airplane_params/airplane_params.hpp"
#include "physics/airplane_params/surface_params.hpp"

#include <algorithm>

namespace Physics
{
	FlightCtrl::FlightCtrl(const AirplaneParams& airplaneParams) :
		m_airplaneParams{airplaneParams}
	{ }

	void FlightCtrl::update(const FlightCtrl& previousAirplaneFlightCtrl)
	{
		updateElevator(previousAirplaneFlightCtrl.m_airplaneCtrl.elevatorAngleRad);
		updateRudder(previousAirplaneFlightCtrl.m_airplaneCtrl.rudderAngleRad);
		updateAilerons(previousAirplaneFlightCtrl.m_airplaneCtrl.aileronsAngleRad);
		updateThrust(previousAirplaneFlightCtrl.m_airplaneCtrl.thrustRelative);
		updateGunfire();
	}

	float FlightCtrl::getElevatorAngleRad() const
	{
		return m_airplaneCtrl.elevatorAngleRad;
	}

	float FlightCtrl::getRudderAngleRad() const
	{
		return m_airplaneCtrl.rudderAngleRad;
	}

	float FlightCtrl::getAileronsAngleRad() const
	{
		return m_airplaneCtrl.aileronsAngleRad;
	}

	float FlightCtrl::getThrustRelative() const
	{
		return m_airplaneCtrl.thrustRelative;
	}

	Common::AirplaneCtrl FlightCtrl::getCtrl() const
	{
		return m_airplaneCtrl;
	}
	
	PlayerInput FlightCtrl::getPlayerInput() const
	{
		return m_playerInput;
	}

	void FlightCtrl::setPlayerInput(const PlayerInput& playerInput)
	{
		m_playerInput = playerInput;
	}

	void FlightCtrl::updateElevator(float previousElevatorAngleRad)
	{
		constexpr float angVelocityRad = glm::radians(120.0f);
		constexpr float maxChange = angVelocityRad / Common::stepsPerSecond;
		float diff = relativeToAbs(m_playerInput.pitch,
			m_airplaneParams.hStab.criticalAngleNegativeRad,
			m_airplaneParams.hStab.criticalAnglePositiveRad) - previousElevatorAngleRad;
		float change = std::clamp(diff, -maxChange, maxChange);
		m_airplaneCtrl.elevatorAngleRad = previousElevatorAngleRad + change;
	}

	void FlightCtrl::updateRudder(float previousRudderAngleRad)
	{
		constexpr float angVelocityRad = glm::radians(120.0f);
		constexpr float maxChange = angVelocityRad / Common::stepsPerSecond;
		float diff = relativeToAbs(m_playerInput.yaw,
			m_airplaneParams.hStab.criticalAngleNegativeRad,
			m_airplaneParams.hStab.criticalAnglePositiveRad) - previousRudderAngleRad;
		float change = std::clamp(diff, -maxChange, maxChange);
		m_airplaneCtrl.rudderAngleRad = previousRudderAngleRad + change;
	}

	void FlightCtrl::updateAilerons(float previousAileronsAngleRad)
	{
		constexpr float angVelocityRad = glm::radians(120.0f);
		constexpr float maxChange = angVelocityRad / Common::stepsPerSecond;
		float diff = relativeToAbs(m_playerInput.roll,
			m_airplaneParams.hStab.criticalAngleNegativeRad,
			m_airplaneParams.hStab.criticalAnglePositiveRad) - previousAileronsAngleRad;
		float change = std::clamp(diff, -maxChange, maxChange);
		m_airplaneCtrl.aileronsAngleRad = previousAileronsAngleRad + change;
	}

	void FlightCtrl::updateThrust(float previousThrustRelative)
	{
		constexpr float velocity = 1;
		constexpr float maxChange = velocity / Common::stepsPerSecond;
		float diff = m_playerInput.thrust - previousThrustRelative;
		float change = std::clamp(diff, -maxChange, maxChange);
		m_airplaneCtrl.thrustRelative = previousThrustRelative + change;
	}

	void FlightCtrl::updateGunfire()
	{
		m_airplaneCtrl.gunfire = m_playerInput.trigger;
	}

	float FlightCtrl::relativeToAbs(float relative, float min, float max)
	{
		return (min + max + relative * (max - min)) / 2;
	}
};
