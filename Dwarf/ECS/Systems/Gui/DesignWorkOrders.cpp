#include "stdafx.h"
#include "DesignWorkOrders.h"
#include "mouse.h"
#include "KeyDampener.h"
#include "Raws\DefInfo.h"
#include "Designations\WorkOrderDesignation.h"
#include "Raws\ReadReactions.h"
#include "Raws\Materials.h"
#include "Raws\Defs\MaterialDef.h"
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

void specifyMaterial(size_t &mat, const Reaction *react) // TODO: Use a list of already encountered materials only
{
	ImGui::Begin("SpecifyMaterial##WorkOrderDesign");

	ImGui::Text("Search For Material: ");
	static ImGuiTextFilter matFilter;
	matFilter.Draw();

	std::vector<std::string> matTags = { "Any" };
	std::vector<std::string> matNames = { "Any" };
	for (const auto& t : defInfo->materialTags)
	{
		const auto* mat = getMaterial(t);

		if (mat && matFilter.PassFilter(mat->name.c_str()) && mat->matType == react->inputs[0].req_material_type)
		{
			matNames.emplace_back(mat->name);
			matTags.emplace_back(t);
		}
	}

	static int selected = 0;

	if (selected > matNames.size() - 1)
		selected = matNames.size() - 1;

	ImGui::ListBox("Materials", &selected, matNames);

	if (selected > 0)
	{
		mat = getMaterialIdx(matTags[selected]);
	}
	else
		mat = 0;

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

	static size_t specifiedMaterial = 0;

	if (reactPassedFilter.size() <= 0)
		return;

	const std::string& tag = reactPassedTags[orderIdx];

	const auto* reaction = getReaction(tag);

	if (reaction->specifyMaterial)
	{
		specifyMaterial(specifiedMaterial, reaction);
	}

	if (ImGui::Button("Give Order##WorkOrderReactions"))
	{
		if (!reaction->specifyMaterial)
			specifiedMaterial = 0;

		auto inputs = reaction->inputs;

		// Create reaction inputs using required material
		if (specifiedMaterial)
			for (auto& i : inputs)
				i.req_material = specifiedMaterial;

		// Search work orders
		for(auto& o : workOrders.active)
			if (o.tag == tag && o.material == specifiedMaterial)
			{
				o.count += orderQty;
				return;
			}
		// Search Queued work orders
		for (auto& q : workOrders.queued)
			if (q.tag == tag && q.material == specifiedMaterial)
			{
				q.count += orderQty;
				return;
			}

		// Not found so let's add it to queued
		workOrders.queued.emplace_back(WorkOrderDesignation{tag, orderQty, specifiedMaterial, inputs, 0, 0});
	}
}

void DesignWorkOrders::drawJobs()
{
	static int selectedWo = 0;
	static int selectedQWo = 0;
	ImGui::Text("Work Orders");

	std::vector<std::string> currWos;

	for (const auto& j : workOrders.active)
		currWos.emplace_back(j.tag);

	ImGui::ListBox("Current Jobs", &selectedWo, currWos);

	std::vector<std::string> currQWos;
	for (const auto& j : workOrders.queued)
		currQWos.emplace_back(j.tag);

	ImGui::ListBox("Queued Jobs:", &selectedQWo, currQWos);
}
