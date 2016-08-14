#ifndef _WIN32
#ifndef DATA_EXTRACTOR_DIALOG_GTK_HEADER_BAR_H
#define DATA_EXTRACTOR_DIALOG_GTK_HEADER_BAR_H

#include <gtkmm.h>

namespace openrayman
{
	class gtk_header_bar : public Gtk::HeaderBar
	{
public:
		gtk_header_bar();

		inline void on_btn_cancel()
		{
			wants_cancel = true;
		}

		bool wants_cancel;
private:
		Gtk::Button m_cancel;
	};
}

#endif
#endif
