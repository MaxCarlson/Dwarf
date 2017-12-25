#pragma once
#include "../Systems.h"

class InputHandler : public System<Requires<>>
{
public:
	void update();

private:
	enum IO_STATE
	{
		MAIN,
		ESC_MENU,
		BUILD,
		DESIGNATE,
		ORDERS,
	};
};

