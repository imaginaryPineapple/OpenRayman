#ifndef _WIN32
#ifndef DATA_EXTRACTOR_DIALOG_GTK_H
#define DATA_EXTRACTOR_DIALOG_GTK_H

#include <platform/data_extractor/data_extractor_dialog.h>
#include <platform/data_extractor/gtk_header_bar.h>
#include <gtkmm.h>
#include <string>
#include <iostream>
#include <atomic>

namespace openrayman
{
	class data_extractor_dialog_gtk : public data_extractor_dialog_base, public Gtk::Window
	{
public:
		data_extractor_dialog_gtk();
		~data_extractor_dialog_gtk();

		void do_events() override;
		void set_title(const std::string& title) override;
		void set_subtitle(const std::string& subtitle) override;
		void set_message(const std::string& msg) override;
		void set_progress(float progress) override;

		inline bool wants_cancel() const override
		{
			return m_wants_cancel;
		}
private:
		Gtk::Main m_main;
		gtk_header_bar m_header_bar;
		Gtk::Box m_box;
		Gtk::Label m_message;
		Gtk::ProgressBar m_progress;

		std::string m_message_str, m_title_str, m_subtitle_str;
		float m_progress_float;
		std::atomic<bool> m_dirty;
		bool m_wants_cancel;
	};
}

#endif
#endif
