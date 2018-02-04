#include "DrawLayers.h"
#include <vector>
#include <utility>

namespace dfr
{
	std::vector<std::pair<int, int>> def;
	std::vector<std::pair<int, Layer *>> renderSeq;

	void DrawLayers::onResize(const int w, const int h)
	{
		width = w;
		height = h;

		for (auto it = layers.begin(); it != layers.end(); ++it)
			it->second.onResize(w, h);
	}

	void DrawLayers::render(sf::RenderWindow & window)
	{
		for (auto id : renderSeq)
			id.second->render(window);
	}

	void DrawLayers::clearAllLayers()
	{
		// Clear all layers that have a terminal
		for (auto id : renderSeq)
		{
			if(id.second->terminal)
				id.second->terminal->clear();
		}
			
	}

	void DrawLayers::addLayer(const int id, const int x, const int y, const int w, const int h, std::string fontTag, std::function<void(Layer*, int, int)> resizeFunc, bool hasBackgound, int order)
	{
		checkUniqueLayer(id);

		layers.emplace(std::make_pair(id, Layer{ x, y, w, h, fontTag, resizeFunc, hasBackgound }));

		if (order == -1)
		{
			order = renderOrder;
			++renderOrder;
		}

		renderSeq.push_back(std::make_pair(order, getLayer(id)));

		std::sort(renderSeq.begin(), renderSeq.end(), 
			[](auto l, auto l2) 
			{ 
				return l.first < l2.first; 
			});
	}

	void DrawLayers::deleteLayer(const int id)
	{
		renderSeq.erase(std::remove_if(renderSeq.begin(), renderSeq.end(), 
			[&id, this](auto pair)
			{
				if (pair.second == this->getLayer(id))
					return true;

				return false;
			}), renderSeq.end());

		layers.erase(id);
	}

	Layer * DrawLayers::getLayer(const int id)
	{
		auto find = layers.find(id);

		if (find != layers.end())
			return &find->second;

		throw std::runtime_error("Non existant layer!");
		return nullptr;
	}
}

