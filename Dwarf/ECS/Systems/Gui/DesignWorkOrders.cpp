#include "stdafx.h"
#include "DesignWorkOrders.h"
#include "mouse.h"
#include "KeyDampener.h"
#include "Raws\DefInfo.h"
#include "Designations\Designations.h"
#include "Raws\ReadReactions.h"
#include <imgui.h>
#include <imgui_tabs.h>

void DesignWorkOrders::init()
{

}

void DesignWorkOrders::update(const double duration) // TODO: Sort by workshop; Sort by skill type; etc
{

	ImGui::Begin("WorkOrders", nullptr, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoCollapse);

	ImGui::BeginTabBar("Tabs##WorkOrders");

	if (ImGui::AddTab("Orders"))
	{
		giveOrder();
	}

	if (ImGui::AddTab("Jobs"))
	{
		drawJobs();
	}

	ImGui::EndTabBar();
	ImGui::End();
}

void DesignWorkOrders::giveOrder()
{
	ImGui::Text("Give Orders");

	static int orderQty = 1;
	static int orderIdx = 0; 

	ImGui::SliderInt("Quantity##WorkOrdersReactions", &orderQty, 1, 100);

	ImGui::Text("Search For Reactions: ");
	static ImGuiTextFilter reactFilter;
	reactFilter.Draw();

	// Draw a filtered list of reaction names
	// and build the list of tags based off filtered 
	// names so we can index properly. Is there a better way? std::vector<std::pair<string, string>> ListBox Vec Getter?
	std::vector<std::string> reactPassedTags;
	std::vector<std::string> reactPassedFilter; 
	for (const auto& i : defInfo->availibleReactions)
	{
		auto rname = getReaction(i)->name;
		if (reactFilter.PassFilter(rname.c_str()))
		{
			reactPassedTags.emplace_back(i);
			reactPassedFilter.emplace_back(rname);
		}
	}

	ImGui::ListBox("Availible Reactions", &orderIdx, reactPassedFilter); 
	
	if (ImGui::Button("Give Order##WorkOrderReactions"))
	{
		const std::string& tag = reactPassedTags[orderIdx];

		// Search work orders
		for(auto& o : designations->workOrders)
			if (o.second == tag)
			{
				o.first += orderQty;
				return;
			}
		// Search Queued work orders
		for (auto& q : designations->queuedWorkOrders)
			if (q.second == tag)
			{
				q.first += orderQty;
				return;
			}

		// Not found so let's add it to queued

		designations->queuedWorkOrders.emplace_back(std::make_pair(orderQty, tag));
	}
}

void DesignWorkOrders::drawJobs()
{
	static int selectedWo = 0;
	static int selectedQWo = 0;
	ImGui::Text("Work Orders");

	std::vector<std::string> currWos;

	for (const auto& j : designations->workOrders)
		currWos.emplace_back(j.second);

	ImGui::ListBox("Current Jobs", &selectedWo, currWos);

	std::vector<std::string> currQWos;
	for (const auto& j : designations->queuedWorkOrders)
		currQWos.emplace_back(j.second);

	ImGui::ListBox("Queued Jobs:", &selectedQWo, currQWos);
}
