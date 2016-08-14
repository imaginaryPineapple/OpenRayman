#include <openrayman/apm_audio_track.h>
#include <iostream>
#include <iomanip>

namespace openrayman
{
	const std::uint16_t _apm_wave_format_ubisoft_adpcm = 0x2000;
	const char _apm_signature[4] = { 'v', 's', '1', '2' };
	const char _apm_data_chunk[4] = { 'D', 'A', 'T', 'A' };

	struct _apm_wave_format_ex
	{
		// microsoft
		common::WAVEFORMATEX base;

		// ubisoft adpcm
		char signature[4];
		char unknown_big_blob0[20];
		std::int32_t initial_predictor_r;
		std::int16_t initial_step_index_r;
		char unknown_blob0[6];
		std::int32_t initial_predictor_l;
		std::int16_t initial_step_index_l;
	};

	static int _apm_ima_index_table[16] =
	{
		-1, -1, -1, -1, 2, 4, 6, 8,
		-1, -1, -1, -1, 2, 4, 6, 8
	};

	static int _apm_ima_step_table[89] =
	{
		7, 8, 9, 10, 11, 12, 13, 14, 16, 17,
		19, 21, 23, 25, 28, 31, 34, 37, 41, 45,
		50, 55, 60, 66, 73, 80, 88, 97, 107, 118,
		130, 143, 157, 173, 190, 209, 230, 253, 279, 307,
		337, 371, 408, 449, 494, 544, 598, 658, 724, 796,
		876, 963, 1060, 1166, 1282, 1411, 1552, 1707, 1878, 2066,
		2272, 2499, 2749, 3024, 3327, 3660, 4026, 4428, 4871, 5358,
		5894, 6484, 7132, 7845, 8630, 9493, 10442, 11487, 12635, 13899,
		15289, 16818, 18500, 20350, 22385, 24623, 27086, 29794, 32767
	};

	apm_audio_track::apm_audio_track(std::istream& stream) :
		m_stream(stream), m_valid(false), m_info(0, 0, 0)
	{
		m_stream.seekg(0);
		if(m_stream.fail())
		{
#ifdef LIBOPENRAYMAN_COUT
			std::cout << "[openrayman::apm_audio_track::apm_audio_track (constructor)] m_stream.fail()" << std::endl;
#endif
			return;
		}

		_apm_wave_format_ex wave_format_ex;
		// TODO: not very endian safe....
		m_stream.read((char*)&wave_format_ex, sizeof(_apm_wave_format_ex));

		if(wave_format_ex.base.waveformat.wFormatTag != _apm_wave_format_ubisoft_adpcm)
		{
#ifdef LIBOPENRAYMAN_COUT
			std::cout << "[openrayman::apm_audio_track::apm_audio_track (constructor)] WAVEFORMATEX had invalid format tag " << std::hex << "0x" << wave_format_ex.base.waveformat.wFormatTag << std::endl;
#endif
			return;
		}

#ifdef LIBOPENRAYMAN_COUT
		std::cout << "[openrayman::apm_audio_track::apm_audio_track (constructor)] sizeof _apm_wave_format_ex " << sizeof(_apm_wave_format_ex) << std::endl;
#endif

#ifdef LIBOPENRAYMAN_COUT
		std::cout << "[openrayman::apm_audio_track::apm_audio_track (constructor)] " << std::endl <<
			"    format_tag: " << std::hex << "0x" << wave_format_ex.base.waveformat.wFormatTag << std::endl <<
			"    is ubisoft adpcm: "<< (wave_format_ex.base.waveformat.wFormatTag == _apm_wave_format_ubisoft_adpcm) << std::endl <<
			"    channels: " << std::dec << wave_format_ex.base.waveformat.nChannels << std::endl <<
			"    samples_per_sec: " << wave_format_ex.base.waveformat.nSamplesPerSec << "hz" << std::endl <<
			"    avg_bytes_per_sec: " << wave_format_ex.base.waveformat.nAvgBytesPerSec << "b" << std::endl <<
			"    block_align: " << wave_format_ex.base.waveformat.nBlockAlign << std::endl <<
			"    bits_per_sample: " << wave_format_ex.base.waveformat.wBitsPerSample << std::endl <<
			"    size: " << wave_format_ex.base.cbSize << std::endl;
#endif

		m_info.sample_rate = wave_format_ex.base.waveformat.nSamplesPerSec;
		m_info.channels = wave_format_ex.base.waveformat.nChannels;

		m_start_predictor_l = wave_format_ex.initial_predictor_l;
		m_start_predictor_r = wave_format_ex.initial_predictor_r;

		m_start_step_index_l = wave_format_ex.initial_step_index_l;
		m_start_step_index_r = wave_format_ex.initial_step_index_r;

#ifdef LIBOPENRAYMAN_COUT
		std::cout << "[openrayman::apm_audio_track::apm_audio_track (constructor)] " << std::endl <<
			"    m_start_predictor_l: " << m_start_predictor_l << std::endl <<
			"    m_start_predictor_r: "<< m_start_predictor_l << std::endl <<
			"    m_start_step_index_l: " << m_start_step_index_l << std::endl <<
			"    m_start_step_index_r: "<< m_start_step_index_r << std::endl;
#endif

		char chunk_identifer[4];
		while(!m_stream.eof())
		{
			m_stream.read(chunk_identifer, 4);
			bool was_data_chunk = true;
			for(int n = 0; n < 4; n++)
			{
				if(chunk_identifer[n] != _apm_data_chunk[n])
					was_data_chunk = false;
			}
			if(was_data_chunk)
				break;
		}

		m_samples_start = m_stream.tellg();
		m_stream.seekg(0, std::ios::end);
		m_info.samples = (std::uint32_t)(m_stream.tellg() - m_samples_start);

#ifdef LIBOPENRAYMAN_COUT
		std::cout << "[openrayman::apm_audio_track::apm_audio_track (constructor)] samples: " << m_info.samples << std::endl;
#endif

		m_valid = true;
		reset();
	}

	common::audio_sample apm_audio_track::decode_one()
	{
		if(!can_decode_more())
			return common::audio_sample(0, 0);

		if(m_current_sample == 0)
			m_stream.seekg(m_samples_start);

		std::int8_t nibble_l, nibble_r;
		if(m_current_sample % 2 == 0)
		{
			m_stream.read(&m_read_nibble_r, 1);
			m_stream.read(&m_read_nibble_l, 1);
		}
		if(m_current_sample % 2 == 0)
		{
			nibble_l = m_read_nibble_l >> 4;
			nibble_r = m_read_nibble_r >> 4;
		}
		else
		{
			nibble_l = (std::int8_t)(m_read_nibble_l << 4) >> 4;
			nibble_r = (std::int8_t)(m_read_nibble_r << 4) >> 4;
		}

		m_current_sample++;
		return common::audio_sample(
				decode_with(nibble_l, m_predictor_l, m_step_index_l, m_step_l),
				decode_with(nibble_r, m_predictor_r, m_step_index_r, m_step_r));
	}

	// Mostly from Ray2Get and https://wiki.multimedia.cx/index.php?title=IMA_ADPCM
	std::int16_t apm_audio_track::decode_with(std::int8_t nibble, std::int32_t& predictor, std::int16_t& step_index, std::int32_t& step)
	{
		step_index += _apm_ima_index_table[nibble & 7];
		if(step_index < 0)
			step_index = 0;
		if(step_index > 88)
			step_index = 88;

		std::int32_t diff = step >> 3;
		if(nibble & 1)
			diff += (step >> 2);
		if(nibble & 2)
			diff += (step >> 1);
		if(nibble & 4)
			diff += step;
		if(nibble & 8)
			diff = -diff;
		predictor += diff;

		step = _apm_ima_step_table[step_index];

		if(predictor > 32767)
			predictor = 32767;
		if(predictor < -32768)
			predictor = -32768;

		return (std::int16_t)predictor;
	}

	std::vector<common::audio_sample> apm_audio_track::decode(std::size_t samples)
	{
		std::vector<common::audio_sample> result;
		for(std::size_t n = 0; n < samples; n++)
		{
			if(!can_decode_more())
				return result;
			result.push_back(decode_one());
		}
		return result;
	}

	bool apm_audio_track::can_decode_more() const
	{
		return m_current_sample < m_info.samples;
	}

	std::uint32_t apm_audio_track::decode_left() const
	{
		return (std::uint32_t)(m_info.samples - (m_current_sample + 1));
	}

	void apm_audio_track::reset()
	{
		m_step_index_l = m_start_step_index_l;
		m_step_index_r = m_start_step_index_r;
		m_predictor_l = m_start_predictor_l;
		m_predictor_r = m_start_predictor_r;
		m_step_l = _apm_ima_step_table[m_step_index_l];
		m_step_r = _apm_ima_step_table[m_step_index_r];
		m_read_nibble_l = m_read_nibble_r = 0;
		m_current_sample = 0;
	}
}
