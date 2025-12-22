#pragma once
#include <filesystem>

#include <imgui.h>
#include <nlohmann/json.hpp>

#include "../Console/ChatStream.h"

namespace App
{
	namespace Shop
	{
		class Shop
		{
		public:
			Shop() = default;

			static void Render();
		};
	}
}