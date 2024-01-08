#include "physics/simulation_buffer_element.hpp"

#include "common/map_name.hpp"

namespace Physics
{
	SimulationBufferElement::SimulationBufferElement(Common::MapName mapName) :
		scene{mapName}
	{ }
};
