#pragma once
#include "../../Systems.h"

struct designation_message;
struct Coordinates;

class DesignationHandler : public System<Requires<>>
{
public:
	void init();

private:
	void designate(const designation_message &msg);
	void designateMining(const int type, const Coordinates co1, const Coordinates co2);
	void designateChopping(const Coordinates co1, const Coordinates co2);
	void designateHarvest(const Coordinates co1, const Coordinates co2);
};

