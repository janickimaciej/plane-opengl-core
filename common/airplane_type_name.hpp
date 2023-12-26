#pragma once

namespace Common
{
	inline constexpr int airplaneTypeCount = 2;

	enum class AirplaneTypeName
	{
		mustang,
		jw1
	};

	unsigned char toUChar(AirplaneTypeName airplaneTypeName);
	AirplaneTypeName fromUChar(unsigned char airplaneTypeName);
};
