#include "common/terrains/maps/hills_map.hpp"

#include "common/terrains/hill.hpp"
#include "common/terrains/value_noise.hpp"
#include "common/terrains/wave_x.hpp"
#include "common/terrains/wave_z.hpp"

#include <memory>

namespace Common::Terrains
{
	constexpr float lengthX = 20000;
	constexpr float lengthZ = 20000;
	constexpr float spacingX = 50;
	constexpr float spacingZ = 50;

	HillsMap::HillsMap() :
		Map{lengthX, lengthZ, spacingX, spacingZ}
	{
		m_elements.push_back(std::make_unique<ValueNoise>(0.00012f, 2048, 0));
		m_elements.push_back(std::make_unique<ValueNoise>(0.00025f, 1024, 0));
		m_elements.push_back(std::make_unique<ValueNoise>(0.0005f, 512, 0));
		m_elements.push_back(std::make_unique<ValueNoise>(0.001f, 256, 0));
		m_elements.push_back(std::make_unique<ValueNoise>(0.002f, 128, 0));
		m_elements.push_back(std::make_unique<ValueNoise>(0.004f, 64, 0));
		m_elements.push_back(std::make_unique<ValueNoise>(0.008f, 32, 0));
	}
	
	float HillsMap::getMaxHeight() const
	{
		return 4100;
	}
};
