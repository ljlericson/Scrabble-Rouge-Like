#include "Script.hpp"

namespace App
{
	namespace LuaScripting
	{
		Script::Script(const std::string& file)
			: m_file(file)
		{
			sol::load_result chunk = sm_lua.load_file(file);
			if (!chunk.valid())
			{
				sol::error err = chunk;
				Console::ccout << "[Lua] Load error: " << err.what() << std::endl;
				auto [begin, end] = Console::cchat.getMessageIterators();
				size_t elem = std::distance(begin, end) - 1;
				Console::Message& mes = Console::cchat.getMessage(elem);
				mes.color = ImVec4(255.0f, 0.0f, 0.0f, 255.0f);
				return;
			}

			sol::protected_function_result exec = chunk();
			if (!exec.valid())
			{
				sol::error err = exec;
				Console::ccout << "[Lua] Runtime error: " << err.what() << std::endl;
				auto [begin, end] = Console::cchat.getMessageIterators();
				size_t elem = std::distance(begin, end) - 1;
				Console::Message& mes = Console::cchat.getMessage(elem);
				mes.color = ImVec4(255.0f, 0.0f, 0.0f, 255.0f);
				return;
			}

			m_entry = sm_lua["onTrigger"];
		}

		sol::optional<sol::table> Script::run(const sol::table& context)
		{
			if (!m_entry.valid())
				return std::nullopt;

			sol::protected_function_result result = m_entry(context);
			if (!result.valid())
			{
				sol::error err = result;
				Console::ccout << "[Lua] Script error in " << m_file << ": " << err.what() << std::endl;
				auto [begin, end] = Console::cchat.getMessageIterators();
				size_t elem = std::distance(begin, end) - 1;
				Console::Message& mes = Console::cchat.getMessage(elem);
				mes.color = ImVec4(255.0f, 0.0f, 0.0f, 255.0f);

				return std::nullopt;
			}

			sol::object obj = result;
			if (obj.is<sol::table>())
				return obj.as<sol::table>();

			return std::nullopt;
		}

		bool Script::isValid() const
		{
			return m_entry.valid();
		}
		
		sol::state& Script::getState()
		{
			return sm_lua;
		}
	}
}