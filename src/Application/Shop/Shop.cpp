#include "Shop.hpp"


namespace App
{
	namespace Shop
	{
		void Shop::Render()
		{
            namespace fs = std::filesystem;

            std::string path = "./config/modifiers"; // Path to the directory (e.g., current directory)
            ImGui::Begin("Shop");
            try 
            {
                for (const auto& entry : fs::directory_iterator(path)) 
                {
                    ImGui::Text("%s", entry.path().c_str());
                    std::cout << entry.path() << '\n';
                }
            }
            catch (const fs::filesystem_error& e) 
            {
                Console::ccout << "Filesystem error: " << e.what() << std::endl;
                auto [begin, end] = Console::cchat.getMessageIterators();
                size_t elem = std::distance(begin, end) - 1;
                Console::Message& mes = Console::cchat.getMessage(elem);
                mes.color = ImVec4(255.0f, 0.0f, 0.0f, 255.0f);
            }
            ImGui::End();
		}
	}
}