#pragma once

#include "common/airplane_info.hpp"
#include "common/bullet_info.hpp"

#include <unordered_map>
#include <vector>

namespace Common
{
	struct SceneInfo
	{
		std::unordered_map<int, AirplaneInfo> airplaneInfos{};
		std::vector<BulletInfo> bulletInfos{};
		int day{};
		float timeOfDay{};
	};
};
