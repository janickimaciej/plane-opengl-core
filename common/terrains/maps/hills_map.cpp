#include "common/terrains/maps/hills_map.hpp"

#include "common/terrains/hill.hpp"
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
		m_elements.push_back(std::make_unique<Hill>(2000.0f, 2000.0f, lengthX / 2, lengthZ / 2));
		m_elements.push_back(std::make_unique<Hill>(1000.0f, 1000.0f, lengthX / 2,
			lengthZ / 2 - 2000));
		m_elements.push_back(std::make_unique<Hill>(750.0f, 1500.0f, lengthX / 2 - 1500,
			lengthZ / 2 - 1500));
		m_elements.push_back(std::make_unique<Hill>(500.0f, 750.0f, lengthX / 2 + 1000,
			lengthZ / 2 - 1000));
		m_elements.push_back(std::make_unique<Hill>(1500.0f, 1500.0f, lengthX / 2 + 1500,
			lengthZ / 2));
		m_elements.push_back(std::make_unique<WaveX>(50.0f, 3000.0f, 0.0f));
		m_elements.push_back(std::make_unique<WaveZ>(50.0f, 3000.0f, 0.0f));
	}
};
