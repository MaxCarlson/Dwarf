#pragma once
#include "ECS\Systems.h"
#include "Coordinates.h"

class ClickSystemGui : public System<Requires<>>
{
public:
	void init();
	void update(const double duration);

private:
	Coordinates click;
};

namespace ClickSystem
{
	void update(const double duration);
}