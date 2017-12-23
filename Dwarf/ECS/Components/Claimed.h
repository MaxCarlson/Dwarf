#pragma once
#include "../Component.h"

struct Claimed : public Component
{
	Claimed() = default;
	Claimed(std::size_t eid) : claimedBy(eid) {}

	std::size_t claimedBy = 0;

	template<class Archive>
	void serialize(Archive& archive)
	{
		archive(claimedBy);
	}
};

CEREAL_REGISTER_TYPE(Claimed);
CEREAL_REGISTER_POLYMORPHIC_RELATION(Component, Claimed);