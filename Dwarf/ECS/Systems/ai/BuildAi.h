#pragma once
#include "../../Systems.h"
#include "../../Components/Tags/BuilderTag.h"

class BuildAi : public System<Requires<BuilderTag>>
{
public:
	void init();
	void update(double duration);

private:

	void doBuild(const Entity & e, const double & duration);
};

