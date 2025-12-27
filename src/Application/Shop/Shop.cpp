#include "Shop.hpp"


namespace App
{
	namespace Shop
	{
		Shop::Shop(ModifierManager* manager)
			: mrp_manager(manager)
		{
			
		}

		void Shop::populateShop()
		{
			m_modifierInfo = mrp_manager->getShopOptions(3);
		}

		void Shop::render()
		{
			ImGui::Begin("SHOP");
			for (size_t i = 0; i < m_modifierInfo.size(); i++)
			{
				ImGui::PushID(static_cast<int>(i));
				auto& modr = m_modifierInfo[i].get();
				if(ImGui::CollapsingHeader(modr.id.c_str()))
				{
					/*ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.0f, 0.0f, 255.0f, 255.0f));
					ImGui::TextWrapped("Name: %s\nDescription: %s\nPrice: %d\n", modr.id.c_str(), modr.description.c_str(), modr.cost);*/

					ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.0f, 0.0f, 255.0f, 255.0f));
					ImGui::TextWrapped("Name: "); ImGui::PopStyleColor(); ImGui::TextWrapped("%s\n\n", modr.id.c_str());

					ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.0f, 0.0f, 255.0f, 255.0f));
					ImGui::TextWrapped("Description: ");  ImGui::PopStyleColor(); ImGui::TextWrapped("%s\n\n", modr.description.c_str());

					ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.0f, 0.0f, 255.0f, 255.0f));
					ImGui::TextWrapped("Price: "); ImGui::PopStyleColor(); ImGui::TextWrapped("%d\n\n", modr.cost);


					if (ImGui::Button("Purchase"))
					{
						mrp_manager->selectOption(modr.id);
						m_modifierInfo.clear();
					}
				}
				ImGui::PopID();
			}
			if(ImGui::Button("Done Shopping"))
				m_modifierInfo.clear();
			ImGui::End();
		}
	}
}