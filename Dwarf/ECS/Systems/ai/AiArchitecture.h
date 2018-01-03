#pragma once
#include "../../Systems.h"
#include "../../Components/Tags/ArchitectTag.h"

class AiArchitecture : public System<Requires<ArchitectTag>>
{
public:
	void init();
	void update(double duration);


private:
	void doWork(Entity e);
};

