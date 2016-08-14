#ifndef OPENRAYMAN_APM_AUDIO_TRACK_H
#define OPENRAYMAN_APM_AUDIO_TRACK_H

#include <openrayman/common_audio.h>
#include <cstdint>
#include <istream>
#include <vector>

namespace openrayman
{
	struct apm_audio_track_info
	{
		apm_audio_track_info(std::uint32_t channels, std::uint16_t sample_rate, std::uint32_t samples) :
			channels(channels), sample_rate(sample_rate), samples(samples)
		{
		}

		// The number of channels in this audio track.
		// Only 2 channels (stereo) are supported for now, as some functions naively expect stereo input/output.
		std::uint16_t channels;

		// The sample rate of the audio track.
		std::uint32_t sample_rate;

		// The total number of samples in this audio track.
		std::uint32_t samples;
	};

	// APM audio tracks are audio tracks encoded with ADPCM used for music.
	class apm_audio_track
	{
public:
		apm_audio_track(std::istream& stream);

		// Returns true if the stream data could be read as a valid APM audio track.
		inline bool valid() const
		{
			return m_valid;
		}

		// Returns a reference to information about this APM audio track.
		inline const apm_audio_track_info& info() const
		{
			return m_info;
		}

		// Decodes one sample from the source audio track.
		// This returns [0, 0] when no more samples are available.
		common::audio_sample decode_one();

		// Decodes a maximum of "samples" samples from the source audio track.
		std::vector<common::audio_sample> decode(std::size_t samples);

		// Returns true if more samples are available.
		bool can_decode_more() const;

		// The amount of samples left before the audio track is completely decoded.
		std::uint32_t decode_left() const;

		// Resets the decoding position to the start.
		void reset();
	private:
		std::int16_t decode_with(std::int8_t nibble, std::int32_t& predictor, std::int16_t& step_index, std::int32_t& step);

		std::istream& m_stream;
		bool m_valid;
		apm_audio_track_info m_info;

		std::streampos m_samples_start;

		std::size_t m_current_sample;

		std::int32_t m_predictor_l, m_predictor_r;
		std::int16_t m_step_index_l, m_step_index_r;
		std::int32_t m_step_l, m_step_r;

		std::int32_t m_start_predictor_l, m_start_predictor_r;
		std::int16_t m_start_step_index_l, m_start_step_index_r;

		char m_read_nibble_r, m_read_nibble_l;
	};
}

#endif
