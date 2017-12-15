#pragma once

#include "../../Systems.h"
#include "../../Components/MovementComponent.h"
#include <unordered_map>

#include "../../../Map/Tile.h"

using namespace region;

// Handles standard movement for
// Entities
class MovementAiSystem : public System<Requires<MovementComponent>>
{
public:
	MovementAiSystem() = default;
	
	void update();

	// Search for path with A*, if there is a path return true
	// else return false. 
	// Path is located in path and needs to be reconstructed
	bool aStar(Coordinates start, Coordinates end, std::unordered_map<Coordinates, Coordinates, CoordinateHash, CoordinateHashEqual> &path);

	// Returns a vector of the path in reverse
	// order which will be stored in an entitys movementComponent
	std::vector<Coordinates> reconstructPath(Coordinates start, Coordinates end, std::unordered_map<Coordinates, Coordinates, CoordinateHash, CoordinateHashEqual>& cameFrom);

private:
};

