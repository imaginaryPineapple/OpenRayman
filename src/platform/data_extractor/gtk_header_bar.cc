#ifndef _WIN32
#include <platform/data_extractor/gtk_header_bar.h>

namespace openrayman
{
	gtk_header_bar::gtk_header_bar() :
		m_cancel("Cancel"),
		wants_cancel(false)
	{
		set_show_close_button(false);
		pack_end(m_cancel);

		m_cancel.signal_clicked().connect(sigc::mem_fun(this, &gtk_header_bar::on_btn_cancel));
	}
}
#endif
