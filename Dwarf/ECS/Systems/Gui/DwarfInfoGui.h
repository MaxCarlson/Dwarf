#pragma once
#include "ECS\Systems.h"
#include "ECS\Components\Sentients\AiWorkComponent.h"

struct Needs;

class DwarfInfoGui : public System<Requires<AiWorkComponent>>
{
public:
	void init();
	void update(const double duration);

	int dwarfSelected = 0;
private:
	void drawNeeds(const std::string &Name, const Entity& e);
	void drawStats(const Entity &e);
	void drawInventory(const Entity &e);
};
