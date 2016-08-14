#include <openrayman/bnm_audio_bank.h>
#include <iostream>

namespace openrayman
{
	struct bnm_header_start
	{
		std::int32_t unknown_value0, unknown_value1;
		std::int32_t event_count;
		std::int32_t unknown_value2;
		std::int32_t file_count;
		std::int32_t length, additional_length;
		std::int32_t unknown_value4, unknown_value5, unknown_value6, unknown_value7;
	};

	struct bnm_header_file
	{
		char id;
		char unknown_value0, unknown_value1, unknown_value2;
		std::int32_t type;
		char unknown_value3, unknown_value4, unknown_value5, unknown_value6;
		std::int32_t length, a_length;
		char unknown_blob0[40];
		std::uint32_t sample_rate;
		char unknown_blob0[8];
		char name[20];
	};

	bnm_audio_bank::bnm_audio_bank(std::istream& stream) :
		m_stream(stream), m_valid()
	{
		m_stream.seekg(0);
		if(m_stream.fail())
		{
#ifdef LIBOPENRAYMAN_COUT
			std::cout << "[openrayman::bnm_audio_bank::bnm_audio_bank (constructor)] m_stream.fail()" << std::endl;
#endif
			return;
		}

		// TODO: this is all from rayman2lib, the unknown fields might need to be resolved.
		bnm_header_start header_start;
		m_stream.read((char*)&header_start, sizeof(bnm_header_start));

		std::streampos data_at = header_start.length;
		if(header_start.additional_length - header_start.length > 0)
			data_at += header_start.additional_length - header_start.length;

#ifdef LIBOPENRAYMAN_COUT
		std::cout << "[openrayman::bnm_audio_bank::bnm_audio_bank (constructor)] Read start header: " << std::endl <<
			"    Event count: " << header_start.event_count << std::endl <<
			"    File count: " << header_start.file_count << std::endl <<
			"    Length: " << header_start.length << " (+" << header_start.additional_length << ")" << std::endl;
#endif

		for(int n = 0; n < header_start.event_count; n++)
		{
			char event[32];
			m_stream.read(event, 32);
			m_binary_events.push_back(std::vector<char>(event, event + 32));
		}
		for(int n = 0; n < header_start.file_count; n++)
		{
			bnm_header_file header_file;
			m_stream.read((char*)&header_file, sizeof(bnm_header_file));
#ifdef LIBOPENRAYMAN_COUT
			std::cout << "[openrayman::bnm_audio_bank::bnm_audio_bank (constructor)] Read file header: " << std::endl <<
				"    ID: 0x" << std::hex << (std::int32_t)header_file.id << std::endl <<
				"    Type: 0x" << std::hex << header_file.type << std::dec << std::endl <<
				"    Length: " << header_file.length << " (a+" << header_file.a_length << ")" << std::endl <<
				"    Sample rate: " << header_file.sample_rate << std::endl <<
				"    Name: " << header_file.name << std::endl;
#endif
			//if(std::string(header_file.name).length() > 0)
				//m_entries.push_back(bnm_audio_bank_entry(*this, header_file.name, data_at, header_file.type == 0xa ? header_file.a_length : header_file.length, header_file.sample_rate));
			data_at += header_file.type == 0xa ? header_file.a_length : header_file.length;
		}

		m_valid = true;
	}
}
