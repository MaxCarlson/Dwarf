#include "include/libtcod.hpp"

#include "Actor.h"
#include "Map.h"
#include "Engine.h"

/*/
class Actor {
public:

};
*/

Engine engine;

int main()
{
	int playerX = 40, playerY = 25;
	

	while (!TCODConsole::isWindowClosed())
	{
		engine.update();
		engine.render();

		TCODConsole::flush();
	}

	//TCODConsole::root->printEx(40, 25, TCOD_BKGND_NONE, TCOD_CENTER, "Hello world");
	//TCODConsole::flush();
	//TCODConsole::waitForKeypress(true);

	return 0;
}