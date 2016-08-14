#ifndef DATA_EXTRACTOR_H
#define DATA_EXTRACTOR_H

#include <platform/backend_specifics.h>
#include <platform/data_extractor/platform.h>
#include <openrayman/bnm_audio_bank.h>
#include <string>
#include <memory>

namespace openrayman
{
    class cnt_directory_node;

    // Extracts data from a valid Rayman 2: The Great Escape installation.
    class data_extractor
    {
public:
        data_extractor(const backend_specifics& backend_specifics) :
            m_backend_specifics(backend_specifics),
			m_succeeded(false),
			m_thread_finished(false),
			m_has_scheduled_message_box(false),
			m_message_box_title(""),
			m_message_box_str(""),
			m_message_box_error(false)
        {
        }

        // Starts the extraction process.
        // Returns true if the extraction succeeded.
        bool extract(const std::string& install_folder);
private:
        void extract_threaded();

        bool check_prerequisites(const std::string& install_folder, data_extractor_dialog& dialog);
        bool create_base();

        bool decompile_game_dsb(const std::string& install_folder, data_extractor_dialog& dialog);

        bool extract_textures_recursive(std::string base, cnt_directory_node &parent, data_extractor_dialog &dialog, std::size_t *counted, std::size_t num_files);
        bool extract_textures(const std::string &install_folder, data_extractor_dialog &dialog);

		bool decode_apm_audio(const std::string& in, const std::string& out, const std::string& no_ext, data_extractor_dialog &dialog);
		bool decode_apm_audio_files(const std::string &install_folder, data_extractor_dialog &dialog);

		bool write_bnm_events(bnm_audio_bank& in, const std::string& out, data_extractor_dialog& dialog);
		bool extract_bnm_bank_entry(std::unique_ptr<bnm_audio_bank_entry>& in, const std::string& out, data_extractor_dialog &dialog);
		bool extract_bnm_bank_files(const std::string &install_folder, data_extractor_dialog &dialog);

		inline void schedule_message_box(const std::string& title, const std::string& str, bool error)
		{
			m_has_scheduled_message_box = true;
			m_message_box_title = title;
			m_message_box_str = str;
			m_message_box_error = error;
		}

        const backend_specifics& m_backend_specifics;
		bool m_succeeded, m_thread_finished;

		const std::string* m_thread_arg_install_folder;
		data_extractor_dialog* m_thread_arg_dialog;

		bool m_has_scheduled_message_box;
		std::string m_message_box_title, m_message_box_str;
		bool m_message_box_error;
	};
}

#endif
