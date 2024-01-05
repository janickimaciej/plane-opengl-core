#pragma once

#include "common/config.hpp"
#include "common/scene_info.hpp"
#include "physics/player_info.hpp"
#include "physics/player_input.hpp"
#include "physics/simulation_buffer_element.hpp"
#include "physics/simulation_buffer_player.hpp"
#include "physics/timestep.hpp"

#include <array>
#include <unordered_map>
#include <vector>

namespace Physics
{
	class SimulationBuffer
	{
	public:
		static constexpr int simulationBufferSize = Common::stepsPerSecond;

		SimulationBuffer(int ownId);
		
		void writeInitFrame(const Timestep& timestep, int playerId, const PlayerInfo& playerInfo);
		void writeControlFrame(const Timestep& timestep, int playerId,
			const PlayerInput& playerInput);
		void writeStateFrame(const Timestep& timestep,
			const std::unordered_map<int, PlayerInfo>& playerInfos);
		void writeOwnInput(const Timestep& timestep, const PlayerInput& ownInput);
		void removeInactivePlayers(const std::vector<int>& removedPlayers,
			const Physics::Timestep& timestep);
		void update(const Timestep& timestep);
		Common::SceneInfo getSceneInfo(const Timestep& timestep) const;
		std::unordered_map<int, Physics::PlayerInfo> getPlayerInfos(const Timestep& timestep) const;

	private:
		std::array<SimulationBufferElement, simulationBufferSize> m_buffer{};
		int m_ownId{};
		
		void addAndUpdatePlayers(const Timestep& timestep,
			const std::unordered_map<int, PlayerInfo>& playerInfos);
		void removePlayers(const Timestep& timestep,
			const std::unordered_map<int, PlayerInfo>& playerInfos);
		
		void addAndUpdatePlayers(const Timestep& previousTimestep, const Timestep& timestep);
		void removePlayers(const Timestep& previousTimestep, const Timestep& timestep);

		void clearLocks(const Timestep& timestep);
	};
};
