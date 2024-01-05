#include "scene.hpp"

#include "common/airplane_centers_of_mass.hpp"
#include "common/airplane_info.hpp"
#include "common/airplane_type_name.hpp"
#include "common/bullet_info.hpp"
#include "common/config.hpp"
#include "common/scene_info.hpp"
#include "physics/airplane_definitions.hpp"
#include "physics/models/airplane.hpp"
#include "physics/player_info.hpp"
#include "physics/timestep.hpp"

#include <cstddef>
#include <list>
#include <unordered_map>
#include <utility>

namespace Physics
{
	void Scene::update(const Timestep& timestep, const Scene& previousScene,
		const std::unordered_map<int, PlayerInfo>& playerInfos,
		const std::unordered_map<int, bool>& stateLocks)
	{
		addAndUpdateAirplanes(previousScene, playerInfos, stateLocks);
		removeAirplanes(previousScene, stateLocks);
		updateBullets(timestep, previousScene);
		detectCollisions();
		m_dayNightCycle.updateTime(previousScene.m_dayNightCycle);
	}

	Common::SceneInfo Scene::getSceneInfo() const
	{
		Common::SceneInfo sceneInfo{};
		for (const std::pair<const int, Airplane>& airplane : m_airplanes)
		{
			sceneInfo.airplaneInfos.insert({airplane.first,
				Common::AirplaneInfo
				{
					airplane.second.getState(),
					airplane.second.getCtrl(),
					airplane.second.getAirplaneTypeName()
				}});
		}
		for (const std::pair<const int, std::list<Bullet>>& bullets : m_bullets)
		{
			for (const Bullet& bullet : bullets.second)
			{
				sceneInfo.bulletInfos.push_back(Common::BulletInfo{bullet.getState()});
			}
		}
		sceneInfo.day = m_dayNightCycle.getDay();
		sceneInfo.timeOfDay = m_dayNightCycle.getTimeOfDay();
		return sceneInfo;
	}

	std::unordered_map<int, PlayerInfo> Scene::getPlayerInfos() const
	{
		std::unordered_map<int, PlayerInfo> playerInfos{};
		for (const std::pair<const int, Airplane>& airplane : m_airplanes)
		{
			playerInfos.insert({airplane.first,
				PlayerInfo
				{
					airplane.second.getPlayerInput(),
					PlayerState
					{
						airplane.second.getHP(),
						airplane.second.getState(),
						airplane.second.getAirplaneTypeName()
					}
				}});
		}
		return playerInfos;
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
				Common::AirplaneTypeName airplaneTypeName =
					playerInfos.at(stateLock.first).state.airplaneTypeName;
				m_airplanes.insert({index,
					Airplane
					{
						airplaneTypeName,
						airplaneDefinitions[toSizeT(airplaneTypeName)].initialHP
					}});
				m_bullets.insert({index, std::list<Bullet>{}});
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
			m_bullets.erase(key);
		}
	}

	void Scene::updateBullets(const Timestep& timestep, const Scene& previousScene)
	{
		for (std::pair<const int, std::list<Bullet>>& bullets : m_bullets)
		{
			int id = bullets.first;
			if (previousScene.m_bullets.contains(id))
			{
				bullets.second = previousScene.m_bullets.at(id);
			}

			static constexpr Timestep bulletLifetime{5, 0};
			while (!bullets.second.empty() &&
				timestep - bullets.second.back().getSpawnTimestep() > bulletLifetime)
			{
				bullets.second.pop_back();
			}

			for (Bullet& bullet : bullets.second)
			{
				bullet.update(bullet);
			}

			static constexpr Timestep bulletCooldown{0, Common::stepsPerSecond / 2};
			if (m_airplanes.at(id).getCtrl().gunfire && (bullets.second.empty() ||
				timestep - bullets.second.front().getSpawnTimestep() > bulletCooldown))
			{
				Common::State airplaneState = m_airplanes.at(id).getState();
				Common::AirplaneTypeName airplaneTypeName =
					m_airplanes.at(id).getAirplaneTypeName();
				glm::vec3 initialPositionLocal =
					airplaneDefinitions[toSizeT(airplaneTypeName)].muzzlePosition +
					glm::vec3{0, 0, -Common::tracerLength};
				glm::vec3 initialVelocityLocal =
					airplaneDefinitions[toSizeT(airplaneTypeName)].muzzleVelocity;

				Common::State state{};
				state.position =
					glm::vec3{airplaneState.matrix() * glm::vec4{initialPositionLocal, 1}};
				state.orientation = airplaneState.orientation;
				state.velocity = airplaneState.velocity + initialVelocityLocal;

				bullets.second.push_front(Bullet{state, timestep});
			}
		}
	}

	void Scene::detectCollisions()
	{
		// TODO
	}
};
