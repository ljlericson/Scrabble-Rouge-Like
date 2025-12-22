#pragma once
#include <memory>
#include <vector>

#include "Modifier.hpp"

namespace App
{
	namespace Shop
	{
		class ModifierMananger
		{
		public:
			ModifierMananger() = default;

		private:
			std::vector<std::unique_ptr<Modifier>> m_modifiers;
		};
	}
}