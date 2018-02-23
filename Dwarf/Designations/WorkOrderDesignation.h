#pragma once
#include <memory>
#include <cereal\types\vector.hpp>

struct WorkOrderDesignation
{
	WorkOrderDesignation() = default;
	WorkOrderDesignation(const std::string &tag, int count, size_t material, size_t workerId = 0, int minSkillLevel = 0)
		: tag(tag), count(count), material(material), workerId(workerId), minSkillLevel(minSkillLevel) {}

	// Reaction tag
	std::string tag;

	// How many times should we do this?
	int count = 1;

	// What material should we make it out of?
	// 0 is does not matter. This is used for customizing the material of the major output
	size_t material = 0;

	// Is there a particular worker we'd like to 
	// have doing this?
	size_t workerId = 0;

	// Do we want workers of greater than this level
	// doing this?
	int minSkillLevel = 0;

	// TODO: Worth it to add a max skill level?

	// This is for customizing material inputs
	// within reason, IE make a titanium sword, etc
	std::vector<ReactionInput> inputs;

	template<class Archive>
	void serialize(Archive &archive)
	{
		archive(tag, count, material, workerId, minSkillLevel, inputs);
	}
};

struct AQWorkOrders
{
	AQWorkOrders() = default;

	std::vector<WorkOrderDesignation> active;
	std::vector<WorkOrderDesignation> queued;

	template<class Archive>
	void serialize(Archive &archive)
	{
		archive(active, queued);
	}
};

extern AQWorkOrders workOrders;