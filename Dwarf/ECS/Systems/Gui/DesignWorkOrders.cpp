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

	ImGui::Begin("WorkOrders", nullptr); // ImGuiWindowFlags_AlwaysAutoResize

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

inline void formatWorkOrders(std::vector<std::string> &in, std::vector<WorkOrderDesignation> &wod)
{
	for (const auto& j : wod)
	{
		auto* reaction = getReaction(j.tag);

		std::stringstream ss;

		ss << "Reaction: " << reaction->name << " #: " << j.count;

		ss << " Material: ";
		if (j.material != 0)
		{
			ss << getMaterial(j.material)->name; // TODO: FIX, Error prone. 
		}
		else
			ss << "N/A";

		ss << "\n";

		
		ImGui::Text(ss.str().c_str());
	}
}

inline void loopBothWorkOrderSets(std::function<void(const WorkOrderDesignation &d, bool queued)> func)
{
	for (const auto& i : workOrders.active)
		func(i, false);
	for (const auto& i : workOrders.queued)
		func(i, true);
}

inline void drawWorkOrderNames()
{
	ImGui::Text("WorkOrder Names");

	static std::vector<bool> bools;

	if (bools.size() < workOrders.active.size() + workOrders.queued.size())
		bools.resize(workOrders.active.size() + workOrders.queued.size(), false);

	int i = 0;
	loopBothWorkOrderSets([&](const WorkOrderDesignation &d, bool q)
	{
		auto * reaction = getReaction(d.tag);
		ImGui::Selectable(reaction->name.c_str(), &bools[i]); // TODO: ERROR PRONE

		++i;
	});
}

inline void drawWorkOrderMaterial()
{
	ImGui::Text("Materials");

	loopBothWorkOrderSets([&](const WorkOrderDesignation &d, bool q)
	{
		std::stringstream ss;

		if (d.material)
		{
			const auto& matName = getMaterial(d.material)->name;
			ss << matName;
		}
		else
			ss << "Not Specified";

		ImGui::Text(ss.str().c_str());
	});
}

inline void drawWorkOrderQty()
{
	ImGui::Text("Qty | Active(A)");
	loopBothWorkOrderSets([](const WorkOrderDesignation &d, bool q)
	{
		std::stringstream ss;

		ss << d.count << "   " << (q ? 'Q' : 'A');

		ImGui::Text(ss.str().c_str());
	});
}

void DesignWorkOrders::drawJobs()
{
	static int selectedWo = 0;
	static int selectedQWo = 0;
	ImGui::Text("Work Orders");

	ImGui::Columns(3);
	ImGui::Separator();

	drawWorkOrderNames();

	ImGui::NextColumn();

	drawWorkOrderMaterial();

	ImGui::NextColumn();

	drawWorkOrderQty();

	ImGui::NextColumn();


	//std::vector<std::string> workOrderNames;
	//formatWorkOrders(workOrderNames, workOrders.active);
	//ImGui::ListBox("Current Jobs", &selectedWo, workOrderNames);

	//std::vector<std::string> qWorkOrderNames;
	//formatWorkOrders(qWorkOrderNames, workOrders.queued);
	//ImGui::ListBox("Queued Jobs:", &selectedQWo, qWorkOrderNames);
}
