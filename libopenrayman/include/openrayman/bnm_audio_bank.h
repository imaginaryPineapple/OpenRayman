#ifndef OPENRAYMAN_BNM_AUDIO_BANK_H
#define OPENRAYMAN_BNM_AUDIO_BANK_H

#include <openrayman/common_audio.h>
#include <cstdint>
#include <istream>
#include <string>
#include <vector>
#include <memory>

namespace openrayman
{
	class bnm_audio_bank;

	class bnm_audio_bank_entry
	{
public:
		bnm_audio_bank_entry(const std::string& name, std::uint16_t channels, std::uint32_t sample_rate, std::uint32_t samples) :
			name(name),
			channels(channels),
			sample_rate(sample_rate),
			samples(samples)
		{
		}

		virtual ~bnm_audio_bank_entry();

		// The name of this entry.
		std::string name;

		// The number of channels in this entry.
		std::uint16_t channels;

		// The sample rate of this entry.
		std::uint32_t sample_rate;

		// The total number of samples in this entry.
		std::uint32_t samples;

		// Decodes one sample from this entry.
		// This returns [0, 0] when no more samples are available.
		virtual common::audio_sample decode_one() = 0;

		// Decodes a maximum of "samples" samples from this entry.
		virtual std::vector<common::audio_sample> decode(std::size_t samples) = 0;

		// Returns true if more samples are available.
		virtual bool can_decode_more() const = 0;

		// The amount of samples left before this entry is completely decoded.
		virtual std::uint32_t decode_left() const = 0;

		// Resets the decoding position to the start.
		virtual void reset() = 0;
	};

	class bnm_audio_bank_entry_wave : public bnm_audio_bank_entry
	{
public:
		bnm_audio_bank_entry_wave(bnm_audio_bank& bank, std::string& name, common::WAVEFORMAT waveformat, std::streampos start, std::size_t size) :
			m_bank(bank),
			m_start(start),
			bnm_audio_bank_entry::bnm_audio_bank_entry(
				name,
				waveformat.nChannels,
				waveformat.nSamplesPerSec,
				size / (waveformat.wBitsPerSample / 8) / channels)
		{
		}

		common::audio_sample decode_one() override;
		std::vector<common::audio_sample> decode(std::size_t samples) override;

		bool can_decode_more() const override;
		std::uint32_t decode_left() const override;

		void reset() override;
private:
		bnm_audio_bank& m_bank;
		std::streampos m_start;
	};

	// BNM audio banks are used for storing collections of WAVE files and associated events.
	// TODO: events and more (unknown fields)
	class bnm_audio_bank
	{
		friend bnm_audio_bank_entry_wave;
		// friend bnm_audio_bank_entry_apm;
public:
		bnm_audio_bank(std::istream& stream);

		// Returns true if the stream data could be read as a valid BNM audio bank.
		inline bool valid() const
		{
			return m_valid;
		}

		// Returns a reference to all binary events found in this audio bank.
		inline const std::vector<std::vector<char>>& binary_events() const
		{
			return m_binary_events;
		}

		// Returns a reference to all entries found in this audio bank.
		inline std::vector<std::unique_ptr<bnm_audio_bank_entry>>& entries()
		{
			return m_entries;
		}

		// Returns a reference to the found entry, or nullptr if it doesn't exist.
		inline bnm_audio_bank_entry* find_by_name(const std::string& name)
		{
			for(std::unique_ptr<bnm_audio_bank_entry>& entry : entries())
			{
				if(entry->name == name)
					return entry.get();
			}
			return nullptr;
		}
private:
		std::istream& m_stream;
		bool m_valid;

		std::vector<std::vector<char>> m_binary_events;
		std::vector<std::unique_ptr<bnm_audio_bank_entry>> m_entries;
	};
}

#endif
