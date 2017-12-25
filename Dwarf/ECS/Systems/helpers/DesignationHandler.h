#pragma once
#include "../../Systems.h"

struct designation_message;

class DesignationHandler : public System<Requires<>>
{
public:
	void init();

private:
	void designate(const designation_message &msg);
};

