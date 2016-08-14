#include <data_extractor.h>
#include <openrayman/common_audio.h>
#include <openrayman/dsb_script.h>
#include <openrayman/cnt_archive.h>
#include <openrayman/gf_texture.h>
#include <openrayman/apm_audio_track.h>
#include <platform/file.h>
#include <platform/message_box.h>
#include <json.hpp>
#ifdef _WIN32
#include <mingw.thread.h>
#else
#include <thread>
#endif
#include <FLAC/stream_encoder.h>
#include <algorithm>

namespace openrayman
{
    bool data_extractor::extract(const std::string& install_folder)
    {
		data_extractor_dialog* dialog = new data_extractor_dialog();
		m_thread_arg_install_folder = &install_folder;
		m_thread_arg_dialog = dialog;
        std::thread thread(&data_extractor::extract_threaded, this);
		thread.detach();
		while(!m_thread_finished)
			dialog->do_events();
		if(m_has_scheduled_message_box)
			message_box::display(m_message_box_title, m_message_box_str, m_message_box_error);
		if(!m_succeeded)
			file::delete_directory(m_backend_specifics.storage_path() + "/games/rayman2");
		delete dialog;
		return m_succeeded;
    }

	void data_extractor::extract_threaded()
	{
		m_succeeded =
			check_prerequisites(*m_thread_arg_install_folder, *m_thread_arg_dialog) &&
		    create_base() &&
		    decompile_game_dsb(*m_thread_arg_install_folder, *m_thread_arg_dialog) &&
			extract_textures(*m_thread_arg_install_folder, *m_thread_arg_dialog) &&
			decode_apm_audio_files(*m_thread_arg_install_folder, *m_thread_arg_dialog) &&
			extract_bnm_bank_files(*m_thread_arg_install_folder, *m_thread_arg_dialog);
		m_thread_finished = true;
	}

    bool data_extractor::check_prerequisites(const std::string& install_folder, data_extractor_dialog& dialog)
    {
        std::cout << "[openrayman::data_extractor::check_prerequisites] Checking prerequisites" << std::endl;
        if(file::exists(install_folder))
        {
            std::cout << "[openrayman::data_extractor::check_prerequisites] " << install_folder << " exists " << std::endl;
            if(file::exists(install_folder + "/Data"))
            {
                std::cout << "[openrayman::data_extractor::check_prerequisites] Data/ exists " << std::endl;
                if(file::exists(install_folder + "/Data/Game.dsb"))
                {
                    std::cout << "[openrayman::data_extractor::check_prerequisites] Game.dsb exists, all good " << std::endl;
                    return true;
                }
                else
                {
					schedule_message_box("Error!", "The installation folder is not valid."
                        "\nReason: The \"Game.dsb\" file doesn't exist."
                        "\nThis file is the database for all dependencies"
                        "\nand is required for extracting the relevant data.", true);
                    return false;
                }
            }
            else
            {
				schedule_message_box("Error!", "The installation folder is not valid."
                    "\nReason: The \"Data\" folder doesn't exist.", true);
                return false;
            }
        }
		schedule_message_box("Error!", "The installation folder specified doesn't exist.", true);
        return false;
    }

    bool data_extractor::create_base()
    {
        std::cout << "[openrayman::data_extractor::create_base] Creating \"rayman2\" at "
                  << file::fix_string(m_backend_specifics.storage_path() + "/games/rayman2") << std::endl;
        file::create_directory(m_backend_specifics.storage_path() + "/games/rayman2");

        nlohmann::json manifest;
        manifest["info"] = nlohmann::json::object();
        manifest["info"]["name"] = "Rayman 2: The Great Escape";
        manifest["info"]["description"] = "Developed by Ubisoft Montpellier, released in 1999";
        manifest["dependencies"] = nlohmann::json::array();

        std::cout << "[openrayman::data_extractor::create_base] Creating default manifest" << std::endl;
        std::cout << "                             info/name = " << manifest["info"]["name"] << std::endl;
        std::cout << "                             info/description = " << manifest["info"]["description"] << std::endl;
        std::cout << "                             dependencies = []" << std::endl;
        std::ofstream manifest_stream(file::fix_string(m_backend_specifics.storage_path() + "/games/rayman2/manifest.json"));
        if(manifest_stream.is_open())
            manifest_stream << std::setw(4) << manifest;
        else
			schedule_message_box("Error!", "The manifest for \"rayman2\" could not be created.", true);
        return manifest_stream.is_open();
    }

    bool data_extractor::decompile_game_dsb(const std::string& install_folder, data_extractor_dialog& dialog)
    {
        std::cout << "[openrayman::data_extractor::decompile_game_dsb] Decompiling Game.dsb" << std::endl;
		dialog.set_message("Decompiling game dsb....");
		dialog.set_progress(0);
		std::cout << "Here then?" << std::endl;
        std::ifstream script_stream(install_folder + "/Data/Game.dsb", std::ios::binary);
        dsb_script script(script_stream);
        if(!script.valid())
            return false;
        return script.decompile(file::fix_string(m_backend_specifics.storage_path() + "/games/rayman2"));
    }

    bool data_extractor::extract_textures_recursive(std::string base, cnt_directory_node &parent, data_extractor_dialog &dialog, std::size_t *counted, std::size_t num_files)
    {
		if(dialog.wants_cancel())
			return false;

		dialog.set_subtitle(parent.full_path());

        for(cnt_file& file : parent.local_files())
        {
			if(dialog.wants_cancel())
				return false;

			// RAII copy
			std::string no_ext = file.name;
			no_ext.erase(no_ext.length() - 3);

			dialog.set_message(no_ext + " (" + std::to_string((int)(file.size / 1024.0)) + " KB)");
			//dialog.set_message(std::to_string((int)n) + "/" + std::to_string((int)parent.local_files().size()));
			dialog.set_progress((float)(*counted) / num_files);

            std::cout << "[openrayman::data_extractor::extract_textures_recursive] Extracting " << file.name << " which is " << (file.size / 1024.0) << " KB... ";

            std::vector<char> array;
            array.reserve(file.size);
            std::size_t at = 0;
            while(at < file.size)
            {
                std::vector<char> read = file.read(at, 1024 * 4096);
                for(char c : read)
                    array.push_back(c);
                at += read.size();
                if(read.size() == 0)
                    break;
            }
            std::cout << "OK" << std::endl;
            std::cout << "[openrayman::data_extractor::extract_textures_recursive] Converting " << file.name << " to PNG... ";
            common::in_mem_istream texture_stream(array);
            gf_texture texture(texture_stream);

			common::in_mem_ostream rgba_data(texture.required_size_rgba());
            if(!texture.convert_to_rgba(rgba_data))
                return false;

			unsigned int error = lodepng::encode(
				file::fix_string(base + "/" + file.name.substr(0, file.name.length() - 3) + ".png"),
				(unsigned char*)rgba_data.mem(),
				texture.info().w,
				texture.info().h);
			if(error)
			{
				std::cout << "[openrayman::data_extractor::extract_textures_recursive] LodePNG error: " << lodepng_error_text(error) << std::endl;
				return false;
			}

            std::cout << "OK" << std::endl;
			(*counted)++;
        }
        for(std::unique_ptr<cnt_directory_node>& child : parent.local_children())
        {
            file::create_directory(base + "/" + child->name);
            if(!extract_textures_recursive(base + "/" + child->name, *child, dialog, counted, num_files))
                return false;
        }
        return true;
    }

    bool data_extractor::extract_textures(const std::string &install_folder, data_extractor_dialog &dialog)
    {
		dialog.set_title("Extracting textures...");
		dialog.set_subtitle("");
		dialog.set_progress(0);
        file::create_directory(m_backend_specifics.storage_path() + "/games/rayman2/textures");
        file::create_directory(m_backend_specifics.storage_path() + "/games/rayman2/textures/vignettes");

        std::ifstream textures_stream(install_folder + "/Data/Textures.cnt", std::ios::binary);
        std::ifstream vignettes_stream(install_folder + "/Data/Vignette.cnt", std::ios::binary);
        cnt_archive textures(textures_stream);
        cnt_archive vignettes(vignettes_stream);
        if(!textures.valid())
        {
			schedule_message_box("Error!", "The texture archive could not be read.", true);
            return false;
        }
        if(!vignettes.valid())
        {
			schedule_message_box("Error!", "The texture archive containing vignettes could not be read.", true);
            return false;
        }

		std::size_t counted = 0;
		std::size_t num_files =
				textures.archive_node().count_num_files(true);
		if(!extract_textures_recursive(m_backend_specifics.storage_path() + "/games/rayman2/textures", textures.archive_node(), dialog, &counted, num_files))
        {
			if(!dialog.wants_cancel())
				schedule_message_box("Error!", "A texture file could not be extracted or read.", true);
            return false;
        }
		dialog.set_title("Extracting vignettes...");
		counted = 0;
		num_files = vignettes.archive_node().count_num_files(true);
		if(!extract_textures_recursive(m_backend_specifics.storage_path() + "/games/rayman2/textures/vignettes", vignettes.archive_node(), dialog, &counted, num_files))
        {
			if(!dialog.wants_cancel())
				schedule_message_box("Error!", "A texture file could not be extracted or read.", true);
            return false;
        }

        return true;
    }

    bool data_extractor::decode_apm_audio(const std::string& in, const std::string& out, const std::string& no_ext, data_extractor_dialog &dialog)
	{
		std::ifstream in_stream(in, std::ios::binary);
		apm_audio_track track(in_stream);

		if(!track.valid())
			return false;

		dialog.set_message(no_ext + " (" + std::to_string(track.info().samples / 1000) + "K samples to decode)");

		FLAC__StreamEncoder* encoder = FLAC__stream_encoder_new();
		FLAC__stream_encoder_set_bits_per_sample(encoder, 16);
		FLAC__stream_encoder_set_channels(encoder, track.info().channels);
		FLAC__stream_encoder_set_sample_rate(encoder, track.info().sample_rate);
		FLAC__stream_encoder_set_total_samples_estimate(encoder, track.info().samples * track.info().channels);
		FLAC__StreamEncoderInitStatus status = FLAC__stream_encoder_init_file(encoder, out.c_str(), nullptr, nullptr);

		if(status != FLAC__STREAM_ENCODER_INIT_STATUS_OK)
		{
			FLAC__stream_encoder_delete(encoder);
			return false;
		}

		while(track.can_decode_more())
		{
			if(dialog.wants_cancel())
				return false;
			common::audio_sample sample = track.decode_one();
			std::int32_t l = sample.left;
			std::int32_t r = sample.right;
			std::int32_t* samples_flac[2] = { &l, &r };
			FLAC__stream_encoder_process(encoder, samples_flac, 1);
		}

		bool did_finish = FLAC__stream_encoder_finish(encoder) != 0;
		FLAC__stream_encoder_delete(encoder);
		return did_finish;
	}

	bool data_extractor::decode_apm_audio_files(const std::string &install_folder, data_extractor_dialog &dialog)
	{
		dialog.set_title("Decoding audio...");
		dialog.set_subtitle("Music");
		dialog.set_progress(0);
		file::create_directory(m_backend_specifics.storage_path() + "/games/rayman2/audio");

		std::vector<std::string> all_unfiltered = file::enumerate_files_in_directory(install_folder + "/Data/World/Sound");
		std::vector<std::string> all;
		for(std::string& str : all_unfiltered)
		{
			if(str.find(".apm") != std::string::npos || str.find(".APM") != std::string::npos)
				all.push_back(str);
		}
		std::sort(all.begin(), all.end());

		for(int n = 0; n < all.size(); n++)
		{
			if(dialog.wants_cancel())
				return false;

			// RAII copy
			std::string no_ext = all[n];
			no_ext.erase(no_ext.length() - 4);
			dialog.set_progress((n + 1) / (float)all.size());

			if(!decode_apm_audio(
				file::fix_string(install_folder + "/Data/World/Sound/" + all[n]),
				file::fix_string(m_backend_specifics.storage_path() + "/games/rayman2/audio/" + no_ext + ".flac"), no_ext, dialog))
			{
				if(!dialog.wants_cancel())
					schedule_message_box("Error!", "An APM audio file could not be decoded or read.", true);
				return false;
			}
		}
		return true;
	}

	bool data_extractor::write_bnm_events(bnm_audio_bank& in, const std::string& out, data_extractor_dialog& dialog)
	{
		file::create_directory(out);
		nlohmann::json json;
		json["events"] = nlohmann::json::array();
		for(int n = 0; n < in.binary_events().size(); n++)
		{
			json["events"].push_back(nlohmann::json::array());
			for(char c : in.binary_events()[n])
				json["events"][n].push_back(c);
		}

		std::ofstream json_stream(file::fix_string(out + "/events.json"));
		if(!json_stream.is_open())
			return false;
		json_stream << std::setw(4) << json;
		return true;
	}

	bool data_extractor::extract_bnm_bank_entry(std::unique_ptr<bnm_audio_bank_entry>& in, const std::string& out, data_extractor_dialog &dialog)
	{
		return true;
	}

	bool data_extractor::extract_bnm_bank_files(const std::string &install_folder, data_extractor_dialog &dialog)
	{
		dialog.set_title("Decoding audio...");
		dialog.set_subtitle("Audio banks");
		dialog.set_progress(0);
		file::create_directory(m_backend_specifics.storage_path() + "/games/rayman2/audio");
		file::create_directory(m_backend_specifics.storage_path() + "/games/rayman2/audio/bank");

		std::vector<std::string> all_unfiltered = file::enumerate_files_in_directory(install_folder + "/Data/World/Sound");
		std::vector<std::string> all;
		for(std::string& str : all_unfiltered)
		{
			if(str.find(".bnm") != std::string::npos || str.find(".BNM") != std::string::npos)
				all.push_back(str);
		}
		std::sort(all.begin(), all.end());

		for(int n = 0; n < all.size(); n++)
		{
			if(dialog.wants_cancel())
				return false;

			// RAII copy
			std::string no_ext = all[n];
			// remove .bnm
			no_ext.erase(no_ext.length() - 4);
			// remove Bnk_
			no_ext.erase(0, 4);
			dialog.set_progress((n + 1) / (float)all.size());

			std::ifstream in_stream(file::fix_string(install_folder + "/Data/World/Sound/" + all[n]), std::ios::binary);
			bnm_audio_bank bank(in_stream);

			if(!bank.valid())
			{
				if(!dialog.wants_cancel())
					schedule_message_box("Error!", "An BNM audio bank was not valid.", true);
				return false;
			}

			if(!write_bnm_events(
				bank,
				file::fix_string(m_backend_specifics.storage_path() + "/games/rayman2/audio/bank/" + no_ext), dialog))
			{
				if(!dialog.wants_cancel())
					schedule_message_box("Error!", "BNM events couldn't be written.", true);
				return false;
			}

			for(std::unique_ptr<bnm_audio_bank_entry>& entry : bank.entries())
			{
				std::string name = entry->name;
				std::transform(name.begin(), name.end(), name.begin(), tolower);
				dialog.set_message(no_ext + "/" + name + " (" + std::to_string(entry->samples / 1000) + "K samples to decode)");
				if(!extract_bnm_bank_entry(
					entry,
					file::fix_string(m_backend_specifics.storage_path() + "/games/rayman2/audio/bank/" + no_ext), dialog))
				{
					if(!dialog.wants_cancel())
						schedule_message_box("Error!", "An BNM audio bank could not be (fully) extracted or read.", true);
					return false;
				}
			}
		}

		dialog.set_progress(0);

		return true;
	}
}
