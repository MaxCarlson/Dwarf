#include "Layer.h"


namespace dfr
{
	void Layer::render(sf::RenderWindow & window)
	{
		if (terminal)
		{
			terminal->render(window);
		}
	}
	void Layer::onResize(const int width, const int height)
	{
		resizeFunc(this, width, height);

		if (terminal)
		{
			terminal->setOffset(x, y);
			terminal->resizePix(w, h);
			terminal->dirty = true;
		}
	}
}
