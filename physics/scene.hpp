#pragma once

#include "common/scene_info.hpp"
#include "physics/day_night_cycle.hpp"
#include "physics/models/airplane.hpp"
#include "physics/models/bullet.hpp"
#include "physics/player_info.hpp"
#include "physics/timestep.hpp"

#include <list>
#include <unordered_map>

namespace Physics
{
	class Scene
	{
	public:
		void update(const Timestep& timestep, const Scene& previousScene,
			const std::unordered_map<int, PlayerInfo>& playerInfos,
			const std::unordered_map<int, bool>& stateLocks);
		Common::SceneInfo getSceneInfo() const;
		std::unordered_map<int, PlayerInfo> getPlayerInfos() const;

	private:
		std::unordered_map<int, Airplane> m_airplanes{};
		std::unordered_map<int, std::list<Bullet>> m_bullets{};

		DayNightCycle m_dayNightCycle{};
		
		void addAndUpdateAirplanes(const Scene& previousScene,
			const std::unordered_map<int, PlayerInfo>& playerInfos,
			const std::unordered_map<int, bool>& stateLocks);
		void removeAirplanes(const Scene& previousScene,
			const std::unordered_map<int, bool>& stateLocks);
		void updateBullets(const Timestep& timestep, const Scene& previousScene);
		void detectCollisions();
	};
};
