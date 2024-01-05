#include "common/airplane_type_name.hpp"

#include <cstddef>

namespace Common
{
	unsigned char toUChar(AirplaneTypeName airplaneTypeName)
	{
		return static_cast<unsigned char>(airplaneTypeName);
	}

	AirplaneTypeName fromUChar(unsigned char airplaneTypeName)
	{
		return static_cast<AirplaneTypeName>(airplaneTypeName);
	}
};
