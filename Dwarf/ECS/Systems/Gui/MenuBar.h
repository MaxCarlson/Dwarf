#pragma once
#include <ECS\Systems.h>

class MenuBar : public System<Requires<>>
{
public:
	MenuBar() = default;
	
	void init();
	void update(const double duration);
};

