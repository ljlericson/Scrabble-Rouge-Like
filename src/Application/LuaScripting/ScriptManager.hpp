#pragma once
#include <vector>
#include <memory>

#include <sol/sol.hpp>

#include "Script.hpp"

namespace App
{
	namespace LuaScripting
	{
		class ScriptManager
		{
		public:
			ScriptManager();

			

		private:
			sol::state m_lua;
			std::vector<std::unique_ptr<Script>> m_scripts;
		};
	}
}