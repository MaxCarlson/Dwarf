#include "DesignationHandler.h"
#include "../../Messages/designation_message.h"
#include "../../World.h"

void DesignationHandler::init()
{
	subscribe<designation_message>([this](designation_message &msg)
	{
		designate(msg);
	});

}

void DesignationHandler::designate(const designation_message & msg)
{

}
