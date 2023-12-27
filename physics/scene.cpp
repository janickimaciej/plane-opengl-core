#include "scene.hpp"

#include "physics/airplane_params_database/airplane_params_database.hpp"
#include "physics/models/airplane.hpp"
#include "physics/player_info.hpp"

#include <cstddef>
#include <unordered_map>
#include <utility>

namespace Physics
{
	void Scene::update(const Scene& previousScene,
		const std::unordered_map<int, PlayerInfo>& playerInfos,
		const std::unordered_map<int, bool>& stateLocks)
	{
		removeAirplanes(previousScene, stateLocks);
		addAndUpdateAirplanes(previousScene, playerInfos, stateLocks);
		updateBullets();
		detectCollisions();
		m_dayNightCycle.updateTime(previousScene.m_dayNightCycle);
	}

	std::unordered_map<int, Common::AirplaneInfo> Scene::getAirplaneInfos() const
	{
		std::unordered_map<int, Common::AirplaneInfo> airplaneInfos;
		for (const std::pair<const int, Airplane>& airplane : m_airplanes)
		{
			airplaneInfos.insert({airplane.first,
				Common::AirplaneInfo
				{
					airplane.second.getState(),
					airplane.second.getCtrl(),
					airplane.second.getAirplaneTypeName()
				}});
		}
		return airplaneInfos;
	}

	std::unordered_map<int, PlayerInfo> Scene::getPlayerInfos() const
	{
		std::unordered_map<int, PlayerInfo> playerInfos{};
		for (const std::pair<const int, Airplane>& airplane : m_airplanes)
		{
			playerInfos.insert({airplane.first,
				PlayerInfo
				{
					PlayerInput
					{},
					PlayerState
					{
						100, //tmp
						airplane.second.getState(),
						airplane.second.getAirplaneTypeName()
					}
				}});
		}
		return playerInfos;
	}

	void Scene::removeAirplanes(const Scene& previousScene,
		const std::unordered_map<int, bool>& stateLocks)
	{
		std::vector<int> keysToBeDeleted;
		for (const std::pair<const int, Airplane>& airplane : m_airplanes)
		{
			if (!stateLocks.contains(airplane.first) ||
				(!stateLocks.at(airplane.first) &&
				!previousScene.m_airplanes.contains(airplane.first)))
			{
				keysToBeDeleted.push_back(airplane.first);
			}
		}
		for (int key : keysToBeDeleted)
		{
			m_airplanes.erase(key);
		}
	}

	void Scene::addAndUpdateAirplanes(const Scene& previousScene,
		const std::unordered_map<int, PlayerInfo>& playerInfos,
		const std::unordered_map<int, bool>& stateLocks)
	{
		for (const std::pair<const int, bool>& stateLock : stateLocks)
		{
			int index = stateLock.first;
			if (!m_airplanes.contains(index))
			{
				m_airplanes.insert({index,
					Airplane{playerInfos.at(stateLock.first).state.airplaneTypeName}});
			}
			
			m_airplanes.at(index).setPlayerInput(playerInfos.at(index).input);
			if (stateLocks.at(index))
			{
				m_airplanes.at(index).setState(playerInfos.at(index).state.state);
			}
			else if (previousScene.m_airplanes.contains(index))
			{
				m_airplanes.at(index).update(previousScene.m_airplanes.at(index));
			}
		}
	}

	void Scene::updateBullets(/*const Scene& previousScene*/)
	{
		// TODO (with bullet generation)
	}

	void Scene::detectCollisions()
	{
		// TODO
	}
};
