#pragma once

#include "common/airplane_info.hpp"
#include "physics/day_night_cycle.hpp"
#include "physics/models/airplane.hpp"
#include "physics/player_info.hpp"

#include <unordered_map>

namespace Physics
{
	class Scene
	{
	public:
		void update(const Scene& previousScene,
			const std::unordered_map<int, PlayerInfo>& playerInfos,
			const std::unordered_map<int, bool>& stateLocks);
		std::unordered_map<int, Common::AirplaneInfo> getAirplaneInfos() const;

	private:
		std::unordered_map<int, Airplane> m_airplanes{};
		// TODO: m_bullets

		DayNightCycle m_dayNightCycle{};

		void removeAirplanes(const Scene& previousScene,
			const std::unordered_map<int, bool>& stateLocks);
		void addAndUpdateAirplanes(const Scene& previousScene,
			const std::unordered_map<int, PlayerInfo>& playerInfos,
			const std::unordered_map<int, bool>& stateLocks);
		void updateBullets(/*const Scene& previousScene*/);
		void detectCollisions();
	};
};
