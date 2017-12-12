#pragma once
#include "../Component.h"
#include "../Coordinates.h"

// Holds positions of Entities
class PositionComponent : public Component
{
public:
	Coordinates co;

	PositionComponent() = default;
	PositionComponent(Coordinates co) : co(co) {};
	~PositionComponent() {};

	template<class Archive>
	void serialize(Archive& archive)
	{
		archive(cereal::base_class<Component>(this), co);
	}
};