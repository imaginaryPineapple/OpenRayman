#ifndef OPENRAYMAN_COMMON_AUDIO_H
#define OPENRAYMAN_COMMON_AUDIO_H

#include <cstdint>

namespace openrayman
{
	namespace common
	{
		// https://msdn.microsoft.com/en-us/library/windows/desktop/dd390970(v=vs.85).aspx
		struct WAVEFORMAT
		{
  			std::uint16_t wFormatTag;
  			std::uint16_t nChannels;
  			std::uint32_t nSamplesPerSec;
  			std::uint32_t nAvgBytesPerSec;
  			std::uint16_t nBlockAlign;
  			std::uint16_t wBitsPerSample;
		};

		struct WAVEFORMATEX
		{
  			WAVEFORMAT waveformat;
  			std::uint16_t cbSize;
		};

		struct audio_sample
		{
			audio_sample(std::int16_t left, std::int16_t right) :
				left(left), right(right)
			{
			}

			// The left and right audio samples, in signed 16-bit PCM.
			// Left and right are equal when the track is mono.
			std::int16_t left, right;
		};
	}
}

#endif
