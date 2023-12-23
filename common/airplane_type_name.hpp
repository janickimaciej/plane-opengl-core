#pragma once

namespace Common
{
	inline constexpr int airplaneTypeCount = 1;

	enum class AirplaneTypeName
	{
		mustang
	};

	unsigned char toUChar(AirplaneTypeName airplaneTypeName);
	AirplaneTypeName fromUChar(unsigned char airplaneTypeName);
};
