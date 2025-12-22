#pragma once
#include <optional>

#include <sol/sol.hpp>

#include "../Console/ChatStream.h"

namespace App
{
	namespace LuaScripting
	{
		class Script
		{
		public:
			Script(const std::string& file);

			sol::optional<sol::table> run(const sol::table& context);

			bool isValid() const;

			static sol::state& getState();

		private:
			std::string m_file;
			sol::protected_function m_entry;
			static inline sol::state sm_lua;
		};
	}
}