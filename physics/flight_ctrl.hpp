#pragma once

#include "common/airplane_ctrl.hpp"
#include "physics/airplane_params/airplane_params.hpp"
#include "physics/player_input.hpp"

namespace Physics
{
	class FlightCtrl
	{
	public:
		FlightCtrl(const AirplaneParams& airplaneParams);
		void update(const FlightCtrl& previousAirplaneFlightCtrl);
		float getElevatorAngleRad() const;
		float getRudderAngleRad() const;
		float getAileronsAngleRad() const;
		float getThrustRelative() const;
		void setPlayerInput(const PlayerInput& playerInput);
		Common::AirplaneCtrl getCtrl() const;
	
	private:
		const AirplaneParams& m_airplaneParams;
		PlayerInput m_playerInput{};
		Common::AirplaneCtrl m_airplaneCtrl{};
		
		void updateElevator(float previousElevatorAngleRad);
		void updateRudder(float previousRudderAngleRad);
		void updateAilerons(float previousAileronsAngleRad);
		void updateThrust(float previousThrustRelative);
		void updateGunfire();
		// converts linearly from [-1, 1] to [min, max]
		static float relativeToAbs(float relative, float min, float max);
	};
};
