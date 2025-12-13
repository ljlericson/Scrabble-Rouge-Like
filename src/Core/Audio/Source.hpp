#pragma once
#include <iostream>
#include <AL/al.h>
#include <dr_wav.h>
// for position
#include <glm/glm.hpp>
#include "../AssetManager/AudioBufferManager.hpp"

namespace Core
{
	namespace Audio
	{
		class Source
		{
		public:
			Source() = default;
			~Source();
			Source(AssetManager::AudioBufferManager* bufManager, const char* fpath, glm::vec3 pos);

			void play(uint8_t volume) const;

		public:
			glm::vec3 pos;
		private:


			ALuint m_bufferID = 0;
			ALuint m_id = 0;

			// IMPORTANT: just a reference pointer, not owned 
			AssetManager::AudioBufferManager* mrp_bufferManager = nullptr; // not owned
		};
	}
}