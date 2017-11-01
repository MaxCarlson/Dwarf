#include "Gui.h"
#include "Engine.h"
#include "Actor.h"
#include "Destructible.h"
#include "Map.h"

#include <stdio.h>
#include <stdarg.h>
#include <string>

static const int PANEL_HEIGHT = 7;
static const int BAR_WIDTH = 20;
static const int MSG_X = BAR_WIDTH + 2;
static const int MSG_HEIGHT = PANEL_HEIGHT - 1;

Gui::Gui()
{
	con = new TCODConsole(engine.screenWidth, PANEL_HEIGHT);
}


Gui::~Gui()
{
	delete con;
	log.clearAndDelete();
}

void Gui::render()
{
	con->setDefaultBackground(TCODColor::black); // Erase previous frame
	con->clear();

	// Draw health bar
	renderBar(1, 1, BAR_WIDTH, "HP", engine.player->destructible->hp, engine.player->destructible->maxHp, TCODColor::red, TCODColor::darkerRed);

	// Draw log
	int y = 1;
	float colorCoef = 0.4f;
	for (Message * message : log) 
	{
		con->setDefaultForeground(message->col * colorCoef);
		con->print(MSG_X, y, message->text);
		++y;
		if (colorCoef < 1.0f)
			colorCoef += 0.3f;
	}


	renderMouseLook();

	// Blit console onto game window TEST THIS!
	TCODConsole::blit(con, 0, 0, engine.screenWidth, PANEL_HEIGHT, TCODConsole::root, 0, engine.screenHeight - PANEL_HEIGHT);
}

void Gui::renderBar(int x, int y, int width, const char * name, int val, int maxVal, const TCODColor & barColor, const TCODColor & backColor)
{
	// Fill bar background color
	con->setDefaultBackground(backColor);
	con->rect(x, y, width, 1, false, TCOD_BKGND_SET);

	int barWidth = (int) (float(val) / float(maxVal) * width);

	// Compute how much of health bar should be full and draw.
	if (barWidth > 0) {
		con->setDefaultBackground(barColor);
		con->rect(x, y, barWidth, 1, false, TCOD_BKGND_SET);
	}

	con->setDefaultBackground(TCODColor::white);
	con->printEx(x + width / 2, y, TCOD_BKGND_NONE, TCOD_CENTER, "%s : %i/%i", name, val, maxVal);
}

void Gui::renderMouseLook() 
{
	// If mouse is out of FOV nothing to render
	if ( ! engine.map->isInFov(engine.mouse.cx, engine.mouse.cy))
		return;

	char buf[128] = "";

	int first = 0;
	for (Actor * actor : engine.actors)
		if (actor->co.x == engine.mouse.cx && actor->co.y == engine.mouse.cy) 
		{
			if (first > 0)
				strcat_s(buf, ", ");

			strcat_s(buf, actor->name);
			++first;
		}

	con->setDefaultForeground(TCODColor::lightGrey);
	con->print(1, 0, buf);
}

void Gui::message(const TCODColor & col, const char * text, ...)
{
	va_list ap;
	char buf[128];
	va_start(ap, text);
	vsprintf_s(buf, text, ap);
	va_end(ap);

	char *lineBegin = buf;
	char *lineEnd;

	do {
		// Make room for new mesage
		if (log.size() == MSG_HEIGHT) {
			Message * toRemove = log.get(0);
			log.remove(toRemove);
			delete toRemove;
		}

		// Look for a \n char in our message line
		lineEnd = strchr(lineBegin, '\n');

		// If one is found split the log
		if (lineEnd)
			*lineEnd = '\0';

		// Add message to the log
		Message * msg = new Message(lineBegin, col);
		log.push(msg);

		// Next line
		lineBegin = lineEnd + 1;

	} while (lineEnd);
}

Gui::Message::Message(const char * text, const TCODColor & col) : text(_strdup(text)), col(col)
{
}

Gui::Message::~Message()
{
	free(text);
}
