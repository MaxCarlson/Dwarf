#pragma once
#include "ECS\Systems.h"
#include "ECS\Components\Sentients\AiWorkComponent.h"

struct Needs;

class DwarfInfoGui : public System<Requires<AiWorkComponent>>
{
public:
	void init();
	void update(const double duration);

	int numberShown = 10;

private:
	void drawNeeds(std::vector<Needs>& needs, std::vector<std::string> names);
};
