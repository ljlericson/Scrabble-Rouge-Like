#include "../InputSystem/BasicInputObserver.hpp"
#include "../../Core/SDL2Backend/Renderer.hpp"

namespace App
{
	namespace GameComponents
	{
		class BasicGameComponent : public InputSystem::BasicInputObserver
		{
		public:
			virtual ~BasicGameComponent() = default;

			virtual void render(const Core::SDLBackend::Renderer& renderer) const = 0;

			virtual void onInput(const bool* keyboardState) override = 0;
		};
	}
}