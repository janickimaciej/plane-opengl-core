#pragma once

#include "common/airplane_info.hpp"
#include "common/airplane_type_name.hpp"
#include "physics/airplane_params/airplane_params.hpp"
#include "physics/flight_ctrl.hpp"
#include "physics/model_dynamics/airplane_dynamics.hpp"
#include "physics/models/model.hpp"
#include "physics/player_input.hpp"

#include <memory>

namespace Physics
{
	class Airplane : public Model
	{
	public:
		Airplane(const Common::AirplaneTypeName& airplaneTypeName, int hp);
		Airplane(const Airplane& airplane);
		Airplane(Airplane&& airplane) noexcept;
		void update(const Airplane& previousAirplane);
		Common::AirplaneTypeName getAirplaneTypeName() const;
		PlayerInput getPlayerInput() const;
		void setPlayerInput(const PlayerInput& input);
		Common::AirplaneCtrl getCtrl() const;
		int getHP() const;
		virtual ~Airplane() = default;

	private:
		Common::AirplaneTypeName m_airplaneTypeName;
		const AirplaneParams& m_airplaneParams;
		FlightCtrl m_flightCtrl;
		AirplaneDynamics m_dynamics;
		int m_hp{};
	};
};
