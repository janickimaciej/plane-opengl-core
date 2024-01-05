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
		m_buffer[timestep.step].mutex.lock();

		bool isSecondOdd = timestep.second % 2;
		if (!m_buffer[timestep.step].players.contains(playerId))
		{
			m_buffer[timestep.step].players.insert({playerId,
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

		m_buffer[timestep.step].mutex.unlock();
	}

	void SimulationBuffer::writeControlFrame(const Timestep& timestep, int playerId,
		const PlayerInput& playerInput)
	{
		m_buffer[timestep.step].mutex.lock();

		bool isSecondOdd = timestep.second % 2;
		if (m_buffer[timestep.step].players.contains(playerId) && !m_buffer[timestep.step].lock &&
			((!isSecondOdd && !m_buffer[timestep.step].players.at(playerId).lockInput[0]) ||
			(isSecondOdd && !m_buffer[timestep.step].players.at(playerId).lockInput[1])))
		{
			m_buffer[timestep.step].players.at(playerId).info.input = playerInput;
			m_buffer[timestep.step].players.at(playerId).lockInput[isSecondOdd] = true;
		}

		m_buffer[timestep.step].mutex.unlock();
	}

	void SimulationBuffer::writeStateFrame(const Timestep& timestep,
		const std::unordered_map<int, PlayerInfo>& playerInfos)
	{
		m_buffer[timestep.step].mutex.lock();
		
		addAndUpdatePlayers(timestep, playerInfos);
		removePlayers(timestep, playerInfos);
		m_buffer[timestep.step].lock = true;

		m_buffer[timestep.step].mutex.unlock();
	}

	void SimulationBuffer::writeOwnInput(const Timestep& timestep, const PlayerInput& ownInput)
	{
		writeControlFrame(timestep, m_ownId, ownInput);
	}

	void SimulationBuffer::removeInactivePlayers(const std::vector<int>& removedPlayers,
		const Physics::Timestep& timestep)
	{
		m_buffer[timestep.step].mutex.lock();
		
		bool isSecondOdd = timestep.second % 2;
		m_buffer[timestep.step].removedPlayers[isSecondOdd] = removedPlayers;

		m_buffer[timestep.step].mutex.unlock();
	}

	void SimulationBuffer::update(const Timestep& timestep)
	{
		Timestep previousTimestep = timestep.previous();

		m_buffer[previousTimestep.step].mutex.lock();
		m_buffer[timestep.step].mutex.lock();
		
		if (!m_buffer[timestep.step].lock)
		{
			addAndUpdatePlayers(previousTimestep, timestep);
			removePlayers(previousTimestep, timestep);
		}

		bool isSecondOdd = timestep.second % 2;
		std::unordered_map<int, PlayerInfo> playerInfos;
		std::unordered_map<int, bool> stateLocks;
		for (const std::pair<const int, SimulationBufferPlayer>& player :
			m_buffer[timestep.step].players)
		{
			playerInfos.insert({player.first, player.second.info});
			stateLocks.insert({player.first,
				m_buffer[timestep.step].lock ||
				m_buffer[timestep.step].players[player.first].lockState[isSecondOdd]});
		}

		clearLocks(timestep);
		
		m_buffer[previousTimestep.step].mutex.unlock();
		m_buffer[timestep.step].mutex.unlock();

		m_buffer[timestep.step].scene.update(timestep, m_buffer[previousTimestep.step].scene,
			playerInfos, stateLocks);
	}

	Common::SceneInfo SimulationBuffer::getSceneInfo(
		const Timestep& timestep) const
	{
		m_buffer[timestep.step].mutex.lock();

		Common::SceneInfo sceneInfo = m_buffer[timestep.step].scene.getSceneInfo();

		m_buffer[timestep.step].mutex.unlock();

		return sceneInfo;
	}

	std::unordered_map<int, Physics::PlayerInfo> SimulationBuffer::getPlayerInfos(
		const Timestep& timestep) const
	{
		return m_buffer[timestep.step].scene.getPlayerInfos();
	}

	void SimulationBuffer::addAndUpdatePlayers(const Timestep& timestep,
		const std::unordered_map<int, PlayerInfo>& playerInfos)
	{
		for (const std::pair<const int, PlayerInfo>& playerInfo : playerInfos)
		{
			if (!m_buffer[timestep.step].players.contains(playerInfo.first))
			{
				m_buffer[timestep.step].players.insert({playerInfo.first,
					SimulationBufferPlayer
					{
						std::array<bool, 2>{},
						std::array<bool, 2>{},
						playerInfo.second
					}});
			}
			else
			{
				m_buffer[timestep.step].players[playerInfo.first].info = playerInfo.second;
			}
		}
	}

	void SimulationBuffer::removePlayers(const Timestep& timestep,
		const std::unordered_map<int, PlayerInfo>& playerInfos)
	{
		std::vector<int> keysToBeDeleted;
		for (const std::pair<const int, SimulationBufferPlayer>& player :
			m_buffer[timestep.step].players)
		{
			if (!playerInfos.contains(player.first))
			{
				keysToBeDeleted.push_back(player.first);
			}
		}
		for (int key : keysToBeDeleted)
		{
			m_buffer[timestep.step].players.erase(key);
		}
	}

	void SimulationBuffer::addAndUpdatePlayers(const Timestep& previousTimestep,
		const Timestep& timestep)
	{
		bool isSecondOdd = timestep.second % 2;
		for (const std::pair<const int, SimulationBufferPlayer>& previousTimestepPlayer :
			m_buffer[previousTimestep.step].players)
		{
			if (!m_buffer[timestep.step].players.contains(previousTimestepPlayer.first))
			{
				m_buffer[timestep.step].players.insert({previousTimestepPlayer.first,
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
						previousTimestepPlayer.second.info
					}});
			}
			else if (!m_buffer[timestep.step].players[previousTimestepPlayer.first].
				lockState[isSecondOdd] &&
				!m_buffer[timestep.step].players[previousTimestepPlayer.first].
				lockInput[isSecondOdd])
			{
				m_buffer[timestep.step].players[previousTimestepPlayer.first].info.input =
					previousTimestepPlayer.second.info.input;
			}
		}
	}

	void SimulationBuffer::removePlayers(const Timestep& previousTimestep,
		const Timestep& timestep)
	{
		bool isSecondOdd = timestep.second % 2;

		for (int player : m_buffer[timestep.step].removedPlayers[isSecondOdd])
		{
			m_buffer[timestep.step].players.erase(player);
		}
		
		std::vector<int> keysToBeDeleted;
		for (std::pair<const int, SimulationBufferPlayer>& player :
			m_buffer[timestep.step].players)
		{
			if (!player.second.lockState[isSecondOdd] &&
				!m_buffer[previousTimestep.step].players.contains(player.first))
			{
				keysToBeDeleted.push_back(player.first);
			}
		}
		for (int key : keysToBeDeleted)
		{
			m_buffer[timestep.step].players.erase(key);
		}
	}

	void SimulationBuffer::clearLocks(const Timestep& timestep)
	{
		m_buffer[timestep.step].lock = false;

		bool isSecondOdd = timestep.second % 2;
		m_buffer[timestep.step].removedPlayers[!isSecondOdd].clear();
		for (std::pair<const int, SimulationBufferPlayer>& player :
			m_buffer[timestep.step].players)
		{
			player.second.lockState[!isSecondOdd] = false;
			player.second.lockInput[!isSecondOdd] = false;
		}
	}
};
