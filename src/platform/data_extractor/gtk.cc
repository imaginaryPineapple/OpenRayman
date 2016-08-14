#ifndef _WIN32
#include <platform/data_extractor/gtk.h>
#include <platform/gtk.h>
#include <thread>

namespace openrayman
{
	data_extractor_dialog_gtk::data_extractor_dialog_gtk() :
		m_progress_float(0),
		m_box(Gtk::Orientation::ORIENTATION_VERTICAL, 10),
		m_message(""),
		m_main(0, nullptr),
		m_wants_cancel(false),
		m_dirty(false)
	{
		if(!can_use_gtk())
			return;

		set_position(Gtk::WindowPosition::WIN_POS_CENTER);
		set_titlebar(m_header_bar);
		set_border_width(20);
		set_default_size(380, 65);
		set_resizable(false);

		m_box.pack_start(m_message);
		m_box.pack_end(m_progress);

		add(m_box);

		show_all_children(true);
		show();
	}

	data_extractor_dialog_gtk::~data_extractor_dialog_gtk()
	{
		hide();
		m_main.iteration(true);
		m_main.quit();
	}

	void data_extractor_dialog_gtk::do_events()
	{
		m_wants_cancel = m_header_bar.wants_cancel;
		if(m_dirty.load())
		{
			m_header_bar.set_title(m_title_str);
			m_header_bar.set_subtitle(m_subtitle_str);
			m_message.set_text(m_message_str);
			m_progress.set_fraction(m_progress_float);
			m_dirty.store(false);
		}
		m_main.iteration(false);
		std::this_thread::sleep_for(std::chrono::milliseconds(1));
	}

	void data_extractor_dialog_gtk::set_title(const std::string& title)
	{
		m_title_str = title;
		m_dirty.store(true);
	}

	void data_extractor_dialog_gtk::set_subtitle(const std::string& subtitle)
	{
		m_subtitle_str = subtitle;
		m_dirty.store(true);
	}

	void data_extractor_dialog_gtk::set_message(const std::string& msg)
	{
		m_message_str = msg;
		m_dirty.store(true);
	}

	void data_extractor_dialog_gtk::set_progress(float progress)
	{
		m_progress_float = (float)std::fmod(progress, 1.0);
		m_dirty.store(true);
	}
}
#endif
