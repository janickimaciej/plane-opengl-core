#pragma once

#include "common/airplane_info.hpp"
#include "physics/player_info.hpp"
#include "physics/player_input.hpp"
#include "physics/simulation_buffer_element.hpp"
#include "physics/simulation_buffer_player.hpp"
#include "physics/timestep.hpp"

#include <array>
#include <unordered_map>

namespace Physics
{
	class SimulationBuffer
	{
	public:
		static constexpr int simulationBufferSize = framesPerSecond;

		SimulationBuffer(int ownId);
		
		void writeInitFrame(const Timestep& timestep, int playerId, const PlayerInfo& playerInfo);
		void writeControlFrame(const Timestep& timestep, int playerId,
			const PlayerInput& playerInput);
		void writeStateFrame(const Timestep& timestep,
			const std::unordered_map<int, PlayerInfo>& playerInfos);
		void writeOwnInput(const Timestep& timestep, const PlayerInput& ownInput);
		void update(const Timestep& timestep);
		std::unordered_map<int, Common::AirplaneInfo> getAirplaneInfos(const Timestep& timestep);
		std::unordered_map<int, Physics::PlayerInfo> getPlayerInfos(const Timestep& timestep);

	private:
		std::array<SimulationBufferElement, simulationBufferSize> m_buffer{};
		int m_ownId{};

		void removePlayers(const Timestep& timestep,
			const std::unordered_map<int, PlayerInfo>& playerInfos);
		void addAndUpdatePlayers(const Timestep& timestep,
			const std::unordered_map<int, PlayerInfo>& playerInfos);

		void removePlayers(const Timestep& previousTimestep, const Timestep& timestep);
		void addAndUpdatePlayers(const Timestep& previousTimestep, const Timestep& timestep);

		void clearLocks(const Timestep& timestep);
		void updateScene(const Timestep& previousTimestep, const Timestep& timestep,
			const std::unordered_map<int, PlayerInfo>& playerInfos,
			const std::unordered_map<int, bool>& stateLocks);
	};
};
