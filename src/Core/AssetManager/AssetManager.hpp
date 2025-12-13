#include <memory>
#include "TextureManager.hpp"
#include "AudioBufferManager.hpp"

namespace Core
{
	namespace AssetManager
	{
		inline std::unique_ptr<TextureManager> textureManager = std::make_unique<TextureManager>();
		inline std::unique_ptr<AudioBufferManager> audioBufferManager = std::make_unique<AudioBufferManager>();
	}
}