#include "ChatConsole.h"
#include "../../Utils/CmdParser/include/ljl/Cmd.hpp"

namespace App
{
    namespace Console
    {
        ChatConsole::ChatConsole()
        {
            m_input.reserve(256);
        }

        void ChatConsole::print(const std::string& msg, ImVec4 color)
        {
            m_messages.push_back({ msg, color, (float)ImGui::GetTime() });
            m_scrollToBottom = true;
        }

        void ChatConsole::loadJson(const std::string& path)
        {
            std::ifstream file(path);
            file >> m_args;
        }

        ljl::cmdparser* ChatConsole::draw()
        {
            ImGui::SetNextWindowBgAlpha(0.4f);

            ImGui::Begin("Chat", nullptr,
                ImGuiWindowFlags_NoTitleBar |
                ImGuiWindowFlags_NoResize |
                ImGuiWindowFlags_AlwaysAutoResize |
                ImGuiWindowFlags_NoMove);
            auto& io = ImGui::GetIO();
            ImGui::SetWindowPos(ImVec2(io.DisplaySize.x - 420 , io.DisplaySize.y - 350));
            

            ImGui::BeginChild("ScrollingRegion", ImVec2(400, 200), true);

            for (auto& msg : m_messages)
            {
                ImGui::PushStyleColor(ImGuiCol_Text, msg.color);
                ImGui::TextWrapped("%s", msg.text.c_str());
                ImGui::PopStyleColor();
            }

            if (m_scrollToBottom)
            {
                ImGui::SetScrollHereY(1.0f);
                m_scrollToBottom = false;
            }

            ImGui::EndChild();

            ImGui::PushItemWidth(400);

            if (ImGui::InputText("##ChatInput", &m_input,
                ImGuiInputTextFlags_EnterReturnsTrue))
            {
                if (!m_input.empty())
                {
                    std::vector<std::string> tokens;
                    std::stringstream ss(m_input);
                    std::string token;

                    while (ss >> token)             // simpler than getline for space-splitting
                        tokens.push_back(token);

                    // Build argv-style array
                    std::vector<char*> argv;
                    argv.reserve(tokens.size());

                    for (auto& s : tokens) 
                        argv.push_back(s.data());       // C++17+: writable, null-terminated

                    // argc = number of tokens
                    int argc = static_cast<int>(argv.size());

                    m_cmdParser = new ljl::cmdparser( argc, argv.data(), m_args );
                    m_input.clear();
                }
                ImGui::SetKeyboardFocusHere(-1);
            }
            else
            {
                if (m_cmdParser)
                {
                    delete m_cmdParser;
                    m_cmdParser = nullptr;
                }
            }

            ImGui::PopItemWidth();


            ImGui::End();

            return m_cmdParser;
        }

        void ChatConsole::clear()
        {
            m_messages.clear();
        }

        void ChatConsole::removeLine(size_t elem)
        {
            m_messages.erase(m_messages.begin() + elem);
        }

        Message& ChatConsole::getMessage(size_t elem)
        {
            return m_messages.at(elem);
        }

        std::pair<std::vector<Message>::const_iterator, 
                  std::vector<Message>::const_iterator> ChatConsole::getMessageIterators() const
        {
            return {
                m_messages.begin(),
                m_messages.end()
            };
        }

        ChatConsole::~ChatConsole()
        {
            if (m_cmdParser)
                delete m_cmdParser;
        }
	}
}