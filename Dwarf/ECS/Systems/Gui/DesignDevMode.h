#pragma once
#include "ECS\Systems.h"

class DesignDevMode : public System<Requires<>>
{
public:
	void init();
	void update(const double duration);

private:
	bool chooseLocation(const std::string& itemTag, const std::string& matTag, const int qty);
};

