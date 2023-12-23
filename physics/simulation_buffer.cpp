#include "physics/simulation_buffer.hpp"

#include "common/airplane_info.hpp"
#include "physics/player_info.hpp"
#include "physics/player_input.hpp"
#include "physics/player_state.hpp"
#include "physics/simulation_buffer_element.hpp"
#include "physics/simulation_buffer_player.hpp"
#include "physics/timestep.hpp"

#include <array>
#include <unordered_map>
#include <utility>
#include <vector>

namespace Physics
{
	SimulationBuffer::SimulationBuffer(int ownId) :
		m_ownId{ownId}
	{ }

	void SimulationBuffer::writeInitFrame(const Timestep& timestep, int playerId,
		const PlayerInfo& playerInfo)
	{
		m_buffer[timestep.frame].mutex.lock();

		bool isSecondOdd = timestep.second % 2;
		if (!m_buffer[timestep.frame].players.contains(playerId))
		{
			m_buffer[timestep.frame].players.insert({playerId,
				SimulationBufferPlayer
				{
					std::array<bool, 2>
					{
						!isSecondOdd,
						isSecondOdd
					},
					std::array<bool, 2>
					{
						false,
						false
					},
					playerInfo
				}});
		}

		m_buffer[timestep.frame].mutex.unlock();
	}

	void SimulationBuffer::writeControlFrame(const Timestep& timestep, int playerId,
		const PlayerInput& playerInput)
	{
		m_buffer[timestep.frame].mutex.lock();

		bool isSecondOdd = timestep.second % 2;
		if (m_buffer[timestep.frame].players.contains(playerId) && !m_buffer[timestep.frame].lock &&
			((!isSecondOdd && !m_buffer[timestep.frame].players.at(playerId).lockInput[0]) ||
			(isSecondOdd && !m_buffer[timestep.frame].players.at(playerId).lockInput[1])))
		{
			m_buffer[timestep.frame].players.at(playerId).lockInput[isSecondOdd] = true;
			m_buffer[timestep.frame].players.at(playerId).info.input = playerInput;
		}

		m_buffer[timestep.frame].mutex.unlock();
	}

	void SimulationBuffer::writeStateFrame(const Timestep& timestep,
		const std::unordered_map<int, PlayerInfo>& playerInfos)
	{
		m_buffer[timestep.frame].mutex.lock();

		removePlayers(timestep, playerInfos);
		addAndUpdatePlayers(timestep, playerInfos);
		m_buffer[timestep.frame].lock = true;

		m_buffer[timestep.frame].mutex.unlock();
	}

	void SimulationBuffer::writeOwnInput(const Timestep& timestep, const PlayerInput& ownInput)
	{
		writeControlFrame(timestep, m_ownId, ownInput);
	}

	void SimulationBuffer::update(const Timestep& timestep)
	{
		Timestep previousTimestep = timestep.previous();

		m_buffer[previousTimestep.frame].mutex.lock();
		m_buffer[timestep.frame].mutex.lock();
		
		if (!m_buffer[timestep.frame].lock)
		{
			removePlayers(previousTimestep, timestep);
			addAndUpdatePlayers(previousTimestep, timestep);
		}

		bool isSecondOdd = timestep.second % 2;
		std::unordered_map<int, PlayerInfo> playerInfos;
		std::unordered_map<int, bool> stateLocks;
		for (const std::pair<const int, SimulationBufferPlayer>& player :
			m_buffer[timestep.frame].players)
		{
			playerInfos.insert({player.first, player.second.info});
			stateLocks.insert({player.first,
				m_buffer[timestep.frame].lock ||
				m_buffer[timestep.frame].players[player.first].lockState[isSecondOdd]});
		}

		clearLocks(timestep);
		
		m_buffer[previousTimestep.frame].mutex.unlock();
		m_buffer[timestep.frame].mutex.unlock();

		updateScene(previousTimestep, timestep, playerInfos, stateLocks);
	}

	std::unordered_map<int, Common::AirplaneInfo> SimulationBuffer::getAirplaneInfos(
		const Timestep& timestep) const
	{
		m_buffer[timestep.frame].mutex.lock();

		std::unordered_map<int, Common::AirplaneInfo> airplaneInfos =
			m_buffer[timestep.frame].scene.getAirplaneInfos();

		m_buffer[timestep.frame].mutex.unlock();

		return airplaneInfos;
	}

	std::unordered_map<int, Physics::PlayerInfo> SimulationBuffer::getPlayerInfos(
		const Timestep& timestep) const
	{
		std::unordered_map<int, PlayerInfo> playerInfos =
			m_buffer[timestep.frame].scene.getPlayerInfos();

		m_buffer[timestep.frame].mutex.lock();
		
		for (const std::pair<const int, SimulationBufferPlayer>& playerInfo :
			m_buffer[timestep.frame].players)
		{
			playerInfos.at(playerInfo.first).input = playerInfo.second.info.input;
		}

		m_buffer[timestep.frame].mutex.unlock();

		return playerInfos;
	}

	void SimulationBuffer::removePlayers(const Timestep& timestep,
		const std::unordered_map<int, PlayerInfo>& playerInfos)
	{
		std::vector<int> keysToBeDeleted;
		for (const std::pair<const int, SimulationBufferPlayer>& player :
			m_buffer[timestep.frame].players)
		{
			if (!playerInfos.contains(player.first))
			{
				keysToBeDeleted.push_back(player.first);
			}
		}
		for (int key : keysToBeDeleted)
		{
			m_buffer[timestep.frame].players.erase(key);
		}
	}

	void SimulationBuffer::addAndUpdatePlayers(const Timestep& timestep,
		const std::unordered_map<int, PlayerInfo>& playerInfos)
	{
		for (const std::pair<const int, PlayerInfo>& playerInfo : playerInfos)
		{
			if (!m_buffer[timestep.frame].players.contains(playerInfo.first))
			{
				m_buffer[timestep.frame].players.insert({playerInfo.first,
					SimulationBufferPlayer
					{
						std::array<bool, 2>
						{
							false,
							false
						},
						std::array<bool, 2>
						{
							false,
							false
						},
						playerInfo.second
					}});
			}
			else
			{
				m_buffer[timestep.frame].players[playerInfo.first].info = playerInfo.second;
			}
		}
	}

	void SimulationBuffer::removePlayers(const Timestep& previousTimestep,
		const Timestep& timestep)
	{
		std::vector<int> keysToBeDeleted;
		bool isSecondOdd = timestep.second % 2;
		for (std::pair<const int, SimulationBufferPlayer>& player :
			m_buffer[timestep.frame].players)
		{
			if (!player.second.lockState[isSecondOdd] &&
				!m_buffer[previousTimestep.frame].players.contains(player.first))
			{
				keysToBeDeleted.push_back(player.first);
			}
		}
		for (int key : keysToBeDeleted)
		{
			m_buffer[timestep.frame].players.erase(key);
		}
	}

	void SimulationBuffer::addAndUpdatePlayers(const Timestep& previousTimestep,
		const Timestep& timestep)
	{
		bool isSecondOdd = timestep.second % 2;
		for (const std::pair<const int, SimulationBufferPlayer>& previousTimestepPlayer :
			m_buffer[previousTimestep.frame].players)
		{
			if (!m_buffer[timestep.frame].players.contains(previousTimestepPlayer.first))
			{
				m_buffer[timestep.frame].players.insert({previousTimestepPlayer.first,
					SimulationBufferPlayer
					{
						std::array<bool, 2>
						{
							false,
							false
						},
						std::array<bool, 2>
						{
							false,
							false
						},
						PlayerInfo
						{
							previousTimestepPlayer.second.info.input,
							PlayerState{}
						}
					}});
			}
			else if (!m_buffer[timestep.frame].players[previousTimestepPlayer.first].
				lockState[isSecondOdd] &&
				!m_buffer[timestep.frame].players[previousTimestepPlayer.first].
				lockInput[isSecondOdd])
			{
				m_buffer[timestep.frame].players[previousTimestepPlayer.first].info.input =
					previousTimestepPlayer.second.info.input;
			}
		}
	}

	void SimulationBuffer::clearLocks(const Timestep& timestep)
	{
		m_buffer[timestep.frame].lock = false;

		bool isSecondOdd = timestep.second % 2;
		for (std::pair<const int, SimulationBufferPlayer>& player :
			m_buffer[timestep.frame].players)
		{
			player.second.lockState[!isSecondOdd] = false;
			player.second.lockInput[!isSecondOdd] = false;
		}
	}

	void SimulationBuffer::updateScene(const Timestep& previousTimestep,
		const Timestep& timestep, const std::unordered_map<int, PlayerInfo>& playerInfos,
		const std::unordered_map<int, bool>& stateLocks)
	{
		m_buffer[timestep.frame].scene.update(m_buffer[previousTimestep.frame].scene, playerInfos,
			stateLocks);
	}
};
