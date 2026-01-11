#pragma once
#include <vector>
#include <string>
#include <thread>
#include <iostream>
#include <sstream>
#include <fstream>

#include <imgui.h>
#include <nlohmann/json.hpp>
#include "imgui_stdlib.h"
#include "../../Utils/Utils.hpp"

namespace ljl
{
	class cmdparser;
}

namespace App
{
	namespace Console
	{
		struct Message
		{
			std::string text;
			ImVec4 color;
			float msgDuration;
		};

		class ChatConsole
		{
		public:
			ChatConsole();
			~ChatConsole();

			void print(const std::string& msg, ImVec4 color = ImVec4(1, 1, 1, 1));

			void loadJson(const std::string& path);

			ljl::cmdparser* draw();

			void clear();

			void removeLine(size_t elem);

			Message& getMessage(size_t elem);

			std::pair<std::vector<Message>::const_iterator, 
				      std::vector<Message>::const_iterator> getMessageIterators() const;

		private:
			ljl::cmdparser* m_cmdParser = nullptr;
			std::vector<Message> m_messages;
			std::string m_input;
			nlohmann::json m_args;
			bool m_scrollToBottom = false;
			bool m_zsh = false;
		};
	}
}