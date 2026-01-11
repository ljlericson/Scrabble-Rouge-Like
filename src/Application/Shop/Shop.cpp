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

		void Shop::render(UIDisabled active)
		{
			if(active == UIDisabled::false_)
				ImGui::Begin("SHOP");
			else
				ImGui::Begin("SHOP", nullptr, ImGuiWindowFlags_NoInputs);
			ImGui::BeginDisabled(active == UIDisabled::true_);

			for (size_t i = 0; i < m_modifierInfo.size(); i++)
			{
				ImGui::PushID(static_cast<int>(i));
				auto& modr = m_modifierInfo[i].get();
				if (ImGui::Button(modr.id.c_str()))
				{
					mrp_manager->selectOption(modr.id);
					m_modifierInfo.clear();
					ImGui::PopID();
					break;
				}
				if(ImGui::IsItemHovered())
				{
					ImGui::BeginTooltip();
					ImGui::PushTextWrapPos(ImGui::GetCursorPos().x + 400.0f);

					ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.0f, 0.0f, 255.0f, 255.0f));
					ImGui::TextWrapped("Name: "); ImGui::PopStyleColor(); ImGui::TextWrapped("%s\n\n", modr.id.c_str());

					ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.0f, 0.0f, 255.0f, 255.0f));
					ImGui::TextWrapped("Description: ");  ImGui::PopStyleColor(); ImGui::TextWrapped("%s\n\n", modr.description.c_str());

					ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.0f, 0.0f, 255.0f, 255.0f));
					ImGui::TextWrapped("Price: "); ImGui::PopStyleColor(); ImGui::TextWrapped("%d\n\n", modr.cost);

					ImGui::PopTextWrapPos();
					ImGui::EndTooltip();
					
				}
				ImGui::PopID();
			}
			if(ImGui::Button("Done Shopping"))
				m_modifierInfo.clear();

			ImGui::EndDisabled();
			ImGui::End();
		}

		void Shop::onInput(const bool* keyboardState, EventType e, const std::vector<uint32_t>& events)
		{
			switch (e)
			{
			case EventType::roundStart:
				break;
			case EventType::roundEnd:
				break;
			case EventType::wordConfirmed:
				break;
			case EventType::screenShake:
				break;
			case EventType::enterDevMode:
				break;
			case EventType::gameEnd:
				break;
			default:
				break;
			}
		}
	}
}